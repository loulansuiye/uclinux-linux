/*
 *	xt_time
 *	Copyright © CC Computer Consultants GmbH, 2007
 *
 *	based on ipt_time by Fabrice MARIE <fabrice@netfilter.org>
 *	This is a module which is used for time matching
 *	It is using some modified code from dietlibc (localtime() function)
 *	that you can find at http://www.fefe.de/dietlibc/
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from gnu.org/gpl.
 */
#include <linux/version.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_time.h>

struct xtm {
	u_int8_t month;    /* (1-12) */
	u_int8_t monthday; /* (1-31) */
	u_int8_t weekday;  /* (1-7) */
	u_int8_t hour;     /* (0-23) */
	u_int8_t minute;   /* (0-59) */
	u_int8_t second;   /* (0-59) */
	unsigned int dse;
};

extern struct timezone sys_tz; /* ouch */

struct xt_time_priv {
	time_t tz_from;
	time_t tz_to;
	time_t tz_change[2];
};

static DEFINE_SPINLOCK(time_lock);

static const u_int16_t days_since_year[2][13] = {
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 },
};

/*
 * Since time progresses forward, it is best to organize this array in reverse,
 * to minimize lookup time.
 */
enum {
	DSE_FIRST = 2039,
};
static const u_int16_t days_since_epoch[] = {
	/* 2039 - 2030 */
	25202, 24837, 24472, 24106, 23741, 23376, 23011, 22645, 22280, 21915,
	/* 2029 - 2020 */
	21550, 21184, 20819, 20454, 20089, 19723, 19358, 18993, 18628, 18262,
	/* 2019 - 2010 */
	17897, 17532, 17167, 16801, 16436, 16071, 15706, 15340, 14975, 14610,
	/* 2009 - 2000 */
	14245, 13879, 13514, 13149, 12784, 12418, 12053, 11688, 11323, 10957,
	/* 1999 - 1990 */
	10592, 10227, 9862, 9496, 9131, 8766, 8401, 8035, 7670, 7305,
	/* 1989 - 1980 */
	6940, 6574, 6209, 5844, 5479, 5113, 4748, 4383, 4018, 3652,
	/* 1979 - 1970 */
	3287, 2922, 2557, 2191, 1826, 1461, 1096, 730, 365, 0,
};

static unsigned int year_from_dse(unsigned int dse)
{
	unsigned int i;

	/*
	 * In each year, a certain number of days-since-the-epoch have passed.
	 * Find the year that is closest to said days.
	 *
	 * Consider, for example, w=21612 (2029-03-04). Loop will abort on
	 * dse[i] <= w, which happens when dse[i] == 21550. This implies
	 * year == 2009. w will then be 62.
	 */
	for (i = 0; days_since_epoch[i] > dse; ++i)
		/* just loop */;

	return i;
}

static inline bool is_leap(unsigned int y)
{
	return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

/*
 * Each network packet has a (nano)seconds-since-the-epoch (SSTE) timestamp.
 * Since we match against days and daytime, the SSTE value needs to be
 * computed back into human-readable dates.
 *
 * This is done in three separate functions so that the most expensive
 * calculations are done last, in case a "simple match" can be found earlier.
 */
static inline unsigned int localtime_1(struct xtm *r, time_t time)
{
	unsigned int v, w;

	/* Each day has 86400s, so finding the hour/minute is actually easy. */
	v         = time % 86400;
	r->second = v % 60;
	w         = v / 60;
	r->minute = w % 60;
	r->hour   = w / 60;
	return v;
}

static inline void localtime_2(struct xtm *r, time_t time)
{
	/*
	 * Here comes the rest (weekday, monthday). First, divide the SSTE
	 * by seconds-per-day to get the number of _days_ since the epoch.
	 */
	r->dse = time / 86400;

	/*
	 * 1970-01-01 (w=0) was a Thursday (4).
	 * -1 and +1 map Sunday properly onto 7.
	 */
	r->weekday = (4 + r->dse - 1) % 7 + 1;
}

static void localtime_3(struct xtm *r, time_t time)
{
	unsigned int year, i, w = r->dse;
	const u_int16_t *dsy;

	year = year_from_dse(w);
	w -= days_since_epoch[year];

	/*
	 * By now we have the current year, and the day of the year.
	 * r->yearday = w;
	 *
	 * On to finding the month (like above). In each month, a certain
	 * number of days-since-New Year have passed, and find the closest
	 * one.
	 *
	 * Consider w=62 (in a non-leap year). Loop will abort on
	 * dsy[i] < w, which happens when dsy[i] == 31+28 (i == 2).
	 * Concludes i == 2, i.e. 3rd month => March.
	 *
	 * (A different approach to use would be to subtract a monthlength
	 * from w repeatedly while counting.)
	 */
	dsy = days_since_year[is_leap(DSE_FIRST - year)];
	for (i = ARRAY_SIZE(days_since_year[0]) - 1; i > 0 && dsy[i] > w; --i)
		/* just loop */;
	r->monthday = w - dsy[i] + 1;
	r->month    = i + 1;
	return;
}

static time_t compute_offset_m(unsigned int year, bool leap,
		u_int8_t month, u_int8_t week, u_int8_t day)
{
	int dow, dom;
	const u_int16_t *dsy = &days_since_year[leap][month];

	/*
	 * Determine what day of the week the first day of the month is using
	 * Zeller's congruence (0 = Sunday)
	 */
	month++;
	if (month <= 3) {
		month += 12;
		year--;
	}
	dow = ((month * 26) / 10 +
	       year + year / 4 + 6 * (year / 100) + year / 400) % 7;

	/*
	 * Determine what 0-based day of the month the first 'day' is
	 */
	dom = (7 + day - dow) % 7;

	/*
	 * Add the weeks, week 5 means last week of month
	 */
	dom += (week - 1) * 7;
	if (dom >= dsy[0] - dsy[-1])
		dom -= 7;

	return (dsy[-1] + dom) * 86400;
}

static void compute_offset(time_t stamp, const struct xt_time_info1 *info)
{
	struct xt_time_priv *priv = info->master;
	unsigned int year;
	bool leap;
	time_t change;
	int rule;

	year = year_from_dse(stamp / 86400);
	leap = is_leap(DSE_FIRST - year);
	priv->tz_from = days_since_epoch[year] * 86400;
	priv->tz_to = priv->tz_from + (leap ? 366 : 365) * 86400;

	for (rule = 0; rule < 2; rule++) {
		switch (info->tz[rule].type) {
		case XT_TIME_TZ_TYPE_J0:
			change = info->tz[rule].day * 86400;
			break;

		case XT_TIME_TZ_TYPE_J1:
			change = (info->tz[rule].day - 1) * 86400;
			if (leap && info->tz[rule].day > 31 + 28)
				change += 86400;
			break;

		case XT_TIME_TZ_TYPE_M:
			change = compute_offset_m(DSE_FIRST - year, leap,
						  info->tz[rule].month,
						  info->tz[rule].week,
						  info->tz[rule].day);
			break;

		default:
			change = 0;
			break;
		}
		priv->tz_change[rule] = priv->tz_from + change +
					info->tz[rule].secs +
					info->tz[rule].offset;
	}
}

static s64 offset_time(s64 stamp, const struct xt_time_info1 *info)
{
	struct xt_time_priv *priv = info->master;
	int isdst;

	if (info->tz[0].offset == info->tz[1].offset)
		return stamp - info->tz[0].offset;

	spin_lock_bh(&time_lock);

	if (unlikely(stamp < priv->tz_from || stamp >= priv->tz_to))
		compute_offset(stamp, info);

	if (priv->tz_change[0] > priv->tz_change[1])
		isdst = stamp < priv->tz_change[1] ||
			stamp >= priv->tz_change[0];
	else
		isdst = stamp >= priv->tz_change[0] &&
			stamp < priv->tz_change[1];
	stamp -= info->tz[isdst].offset;

	spin_unlock_bh(&time_lock);
	return stamp;
}

static bool
time_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_time_info1 *info = par->matchinfo;
	unsigned int packet_time;
	struct xtm current_time;
	s64 stamp;

	/*
	 * We cannot use get_seconds() instead of __net_timestamp() here.
	 * Suppose you have two rules:
	 * 	1. match before 13:00
	 * 	2. match after 13:00
	 * If you match against processing time (get_seconds) it
	 * may happen that the same packet matches both rules if
	 * it arrived at the right moment before 13:00.
	 */
	if (skb->tstamp.tv64 == 0)
		__net_timestamp((struct sk_buff *)skb);

	stamp = ktime_to_ns(skb->tstamp);
	stamp = div_s64(stamp, NSEC_PER_SEC);

	if (info->flags & XT_TIME_LOCAL_TZ)
		/* Adjust for local timezone */
		stamp -= 60 * sys_tz.tz_minuteswest;
	else if (info->flags & XT_TIME_TZ)
		stamp = offset_time(stamp, info);

	/*
	 * xt_time will match when _all_ of the following hold:
	 *   - 'now' is in the global time range date_start..date_end
	 *   - 'now' is in the monthday mask
	 *   - 'now' is in the weekday mask
	 *   - 'now' is in the daytime range time_start..time_end
	 * (and by default, libxt_time will set these so as to match)
	 */

	if (stamp < info->date_start || stamp > info->date_stop)
		return false;

	packet_time = localtime_1(&current_time, stamp);

	if (info->daytime_start < info->daytime_stop) {
		if (packet_time < info->daytime_start ||
		    packet_time > info->daytime_stop)
			return false;
	} else {
		if (packet_time < info->daytime_start &&
		    packet_time > info->daytime_stop)
			return false;
	}

	localtime_2(&current_time, stamp);

	if (!(info->weekdays_match & (1 << current_time.weekday)))
		return false;

	/* Do not spend time computing monthday if all days match anyway */
	if (info->monthdays_match != XT_TIME_ALL_MONTHDAYS) {
		localtime_3(&current_time, stamp);
		if (!(info->monthdays_match & (1 << current_time.monthday)))
			return false;
	}

	return true;
}

static int time_mt_check(const struct xt_mtchk_param *par)
{
	const struct xt_time_info *info = par->matchinfo;

	if (info->daytime_start > XT_TIME_MAX_DAYTIME ||
	    info->daytime_stop > XT_TIME_MAX_DAYTIME) {
		pr_info("invalid argument - start or "
			"stop time greater than 23:59:59\n");
		return -EDOM;
	}

	return 0;
}

static int time_mt_check_v0(const struct xt_mtchk_param *par)
{
	const struct xt_time_info *info = par->matchinfo;

	if (info->flags & ~XT_TIME_LOCAL_TZ) {
		pr_info("invalid flags for version 0\n");
		return -EINVAL;
	}

	return time_mt_check(par);
}

static int time_mt_check_v1(const struct xt_mtchk_param *par)
{
	struct xt_time_info1 *info = par->matchinfo;
	struct xt_time_priv *priv;
	int err;

	err = time_mt_check(par);
	if (err != 0)
		return err;

	if (info->flags & XT_TIME_TZ) {
		priv = kzalloc(sizeof(*priv), GFP_KERNEL);
		if (priv == NULL)
			return -ENOMEM;

		info->master = priv;
	}

	return 0;
}

static void time_mt_destroy_v1(const struct xt_mtdtor_param *par)
{
	const struct xt_time_info1 *info = par->matchinfo;

	if (info->flags & XT_TIME_TZ)
		kfree(info->master);
}

static struct xt_match xt_time_mt_reg[] __read_mostly = {
	{
		.name       = "time",
		.revision   = 0,
		.family     = NFPROTO_UNSPEC,
		.match      = time_mt,
		.checkentry = time_mt_check_v0,
		.matchsize  = sizeof(struct xt_time_info),
		.me         = THIS_MODULE,
	},
	{
		.name       = "time",
		.revision   = 1,
		.family     = NFPROTO_UNSPEC,
		.match      = time_mt,
		.checkentry = time_mt_check_v1,
		.destroy    = time_mt_destroy_v1,
		.matchsize  = sizeof(struct xt_time_info1),
		.me         = THIS_MODULE,
	},
};

static int __init time_mt_init(void)
{
	int minutes = sys_tz.tz_minuteswest;

	if (minutes < 0) /* east of Greenwich */
		printk(KERN_INFO KBUILD_MODNAME
		       ": kernel timezone is +%02d%02d\n",
		       -minutes / 60, -minutes % 60);
	else /* west of Greenwich */
		printk(KERN_INFO KBUILD_MODNAME
		       ": kernel timezone is -%02d%02d\n",
		       minutes / 60, minutes % 60);

	return xt_register_matches(xt_time_mt_reg, ARRAY_SIZE(xt_time_mt_reg));
}

static void __exit time_mt_exit(void)
{
	xt_unregister_matches(xt_time_mt_reg, ARRAY_SIZE(xt_time_mt_reg));
}

module_init(time_mt_init);
module_exit(time_mt_exit);
MODULE_AUTHOR("Jan Engelhardt <jengelh@medozas.de>");
MODULE_DESCRIPTION("Xtables: time-based matching");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_time");
MODULE_ALIAS("ip6t_time");
