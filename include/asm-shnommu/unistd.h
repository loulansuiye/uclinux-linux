#ifndef _ASM_SH7615_UNISTD_H_
#define _ASM_SH7615_UNISTD_H_

/*Posh2 trap32 used*/
/*
 * This file contains the system call numbers.
 */

#define __NR_setup		  0	/* used only by init, to get system going */
#define __NR_exit		  1
#define __NR_fork		  2
#define __NR_read		  3
#define __NR_write		  4
#define __NR_open		  5
#define __NR_close		  6
#define __NR_waitpid		  7
#define __NR_creat		  8
#define __NR_link		  9
#define __NR_unlink		 10
#define __NR_execve		 11
#define __NR_chdir		 12
#define __NR_time		 13
#define __NR_mknod		 14
#define __NR_chmod		 15
#define __NR_chown		 16
#define __NR_break		 17
#define __NR_oldstat		 18
#define __NR_lseek		 19
#define __NR_getpid		 20
#define __NR_mount		 21
#define __NR_umount		 22
#define __NR_setuid		 23
#define __NR_getuid		 24
#define __NR_stime		 25
#define __NR_ptrace		 26
#define __NR_alarm		 27
#define __NR_oldfstat		 28
#define __NR_pause		 29
#define __NR_utime		 30
#define __NR_stty		 31
#define __NR_gtty		 32
#define __NR_access		 33
#define __NR_nice		 34
#define __NR_ftime		 35
#define __NR_sync		 36
#define __NR_kill		 37
#define __NR_rename		 38
#define __NR_mkdir		 39
#define __NR_rmdir		 40
#define __NR_dup		 41
#define __NR_pipe		 42
#define __NR_times		 43
#define __NR_prof		 44
#define __NR_brk		 45
#define __NR_setgid		 46
#define __NR_getgid		 47
#define __NR_signal		 48
#define __NR_geteuid		 49
#define __NR_getegid		 50
#define __NR_acct		 51
#define __NR_phys		 52
#define __NR_lock		 53
#define __NR_ioctl		 54
#define __NR_fcntl		 55
#define __NR_mpx		 56
#define __NR_setpgid		 57
#define __NR_ulimit		 58
#define __NR_oldolduname	 59
#define __NR_umask		 60
#define __NR_chroot		 61
#define __NR_ustat		 62
#define __NR_dup2		 63
#define __NR_getppid		 64
#define __NR_getpgrp		 65
#define __NR_setsid		 66
#define __NR_sigaction		 67
#define __NR_sgetmask		 68
#define __NR_ssetmask		 69
#define __NR_setreuid		 70
#define __NR_setregid		 71
#define __NR_sigsuspend		 72
#define __NR_sigpending		 73
#define __NR_sethostname	 74
#define __NR_setrlimit		 75
#define __NR_getrlimit		 76
#define __NR_getrusage		 77
#define __NR_gettimeofday	 78
#define __NR_settimeofday	 79
#define __NR_getgroups		 80
#define __NR_setgroups		 81
#define __NR_select		 82
#define __NR_symlink		 83
#define __NR_oldlstat		 84
#define __NR_readlink		 85
#define __NR_uselib		 86
#define __NR_swapon		 87
#define __NR_reboot		 88
#define __NR_readdir		 89
#define __NR_mmap		 90
#define __NR_munmap		 91
#define __NR_truncate		 92
#define __NR_ftruncate		 93
#define __NR_fchmod		 94
#define __NR_fchown		 95
#define __NR_getpriority	 96
#define __NR_setpriority	 97
#define __NR_profil		 98
#define __NR_statfs		 99
#define __NR_fstatfs		100
#define __NR_ioperm		101
#define __NR_socketcall		102
#define __NR_syslog		103
#define __NR_setitimer		104
#define __NR_getitimer		105
#define __NR_stat		106
#define __NR_lstat		107
#define __NR_fstat		108
#define __NR_olduname		109
#define __NR_iopl		/* 110 */ not supported
#define __NR_vhangup		111
#define __NR_idle		112
#define __NR_vm86		/* 113 */ not supported
#define __NR_wait4		114
#define __NR_swapoff		115
#define __NR_sysinfo		116
#define __NR_ipc		117
#define __NR_fsync		118
#define __NR_sigreturn		119
#define __NR_clone		120
#define __NR_setdomainname	121
#define __NR_uname		122
#define __NR_cacheflush		123
#define __NR_adjtimex		124
#define __NR_mprotect		125
#define __NR_sigprocmask	126
#define __NR_create_module	127
#define __NR_init_module	128
#define __NR_delete_module	129
#define __NR_get_kernel_syms	130
#define __NR_quotactl		131
#define __NR_getpgid		132
#define __NR_fchdir		133
#define __NR_bdflush		134
#define __NR_sysfs		135
#define __NR_personality	136
#define __NR_afs_syscall	137 /* Syscall for Andrew File System */
#define __NR_setfsuid		138
#define __NR_setfsgid		139
#define __NR__llseek		140
#define __NR_getdents		141
#define __NR__newselect		142
#define __NR_flock		143
#define __NR_msync		144
#define __NR_readv		145
#define __NR_writev		146
#define __NR_getsid		147
#define __NR_fdatasync		148
#define __NR__sysctl		149
#define __NR_mlock		150
#define __NR_munlock		151
#define __NR_mlockall		152
#define __NR_munlockall		153
#define __NR_sched_setparam		154
#define __NR_sched_getparam		155
#define __NR_sched_setscheduler		156
#define __NR_sched_getscheduler		157
#define __NR_sched_yield		158
#define __NR_sched_get_priority_max	159
#define __NR_sched_get_priority_min	160
#define __NR_sched_rr_get_interval	161
#define __NR_nanosleep		162
#define __NR_mremap		163

/*Posh2 syscall for testing*/
#define __NR_test		164

/* user-visible error numbers are in the range -1 - -122: see
   <asm-sh/errno.h> */

#define __syscall_return(type, res) \
do { \
	if ((unsigned long)(res) >= (unsigned long)(-125)) { \
	/* avoid using res which is declared to be in register d0; \
	   errno might expand to a function call and clobber it.  */ \
		int __err = -(res); \
		errno = __err; \
		res = -1; \
	} \
	return (type) (res); \
} while (0)

		
// Posh2 -Modified with SH command set taking H8 as reference
			
#define _syscall0(type,name) \
type name(void) \
{ \
	register long __res __asm__ ("r0") = __NR_##name; \
	__asm__ __volatile__ ("trapa  #32" \
                      : "=g" (__res) \
		      : "0" (__res) \
		      : "r0"); \
	__syscall_return(type, __res); \
}


//Posh2 % removed from registers 
// Posh2 -Modified with SH command set taking H8 as reference
					
#define _syscall1(type, name, atype, a) \
type name(atype a) \
{ \
	register long __res __asm__ ("r0") = __NR_##name; \
	register long __a __asm__ ("r1") = (long)(a); \
	__asm__ __volatile__ ("trapa  #32" \
		      : "=r" (__res) \
		      : "0" (__res), "r" (__a) \
		      : "r0"); \
	__syscall_return(type,__res); \
}


//Posh2 % removed from registers 
// Posh2 -Modified with SH command set taking H8 as reference

#define _syscall2(type, name, atype, a, btype, b)			\
type name(atype a, btype b)						\
{									\
	register long __res __asm__ ("r0") = __NR_##name; \
	register long __a __asm__ ("r1") = (long)(a); \
	register long __b __asm__ ("r2") = (long)(b); \
	__asm__ __volatile__ ("trapa  #32" \
		      : "=r" (__res) \
              : "0" (__res), "r" (__a), "r" (__b) \
		      : "r0"); \
	__syscall_return(type,__res); \
}

//Posh2 % removed from registers 
// Posh2 -Modified with SH command set taking H8 as reference

#define _syscall3(type, name, atype, a, btype, b, ctype, c)		\
type name(atype a, btype b, ctype c)					\
{									\
	register long __res __asm__ ("r0") = __NR_##name; \
	register long __a __asm__ ("r1") = (long)(a); \
	register long __b __asm__ ("r2") = (long)(b); \
	register long __c __asm__ ("r3") = (long)(c); \
	__asm__ __volatile__ ("trapa  #32" \
		      : "=r" (__res) \
              : "0" (__res), "r" (__a), "r" (__b), \
	        	"r" (__c) \
		      :"r0"); \
	__syscall_return(type,__res); \
}

//Posh2 % removed from registers 

// Posh2 -Modified with SH command set taking H8 as reference

#define _syscall4(type, name, atype, a, btype, b, ctype, c, dtype, d)	\
type name(atype a, btype b, ctype c, dtype d)				\
{									\
	register long __res __asm__ ("r0") = __NR_##name; \
	register long __a __asm__ ("r1") = (long)(a); \
	register long __b __asm__ ("r2") = (long)(b); \
	register long __c __asm__ ("r3") = (long)(c); \
	register long __d __asm__ ("r4") = (long)(d); \
	__asm__ __volatile__ ("trapa  #32" \
                : "=r" (__res) \
                : "0" (__res), "r" (__a), "r" (__b), \
				"r" (__c), "r" (__d)  \
			      : "r0"); \
	__syscall_return(type,__res); \
}
//Posh2 % removed from registers 

//Posh2 -Modified with SH command set taking H8 as reference

#define _syscall5(type, name, atype, a, btype, b, ctype, c, dtype, d, etype, e)\
type name(atype a, btype b, ctype c, dtype d, etype e)			\
{									\
	register long __res __asm__ ("r0") = __NR_##name; \
	register long __a __asm__ ("r1") = (long)(a); \
	register long __b __asm__ ("r2") = (long)(b); \
	register long __c __asm__ ("r3") = (long)(c); \
	register long __d __asm__ ("r4") = (long)(d); \
	register long __e __asm__ ("r5") = (long)(e); \
	__asm__ __volatile__ ("trapa  #32" \
		      : "=r" (__res) \
		      : "0" (__res), "r" (__a), "r" (__b), \
			"r" (__c), "r" (__d), "r" (__e)  \
                      : "r0"); \
	__syscall_return(type,__res); \
}

#ifdef __KERNEL_SYSCALLS__

/*
 * we need this inline - forking from kernel space will result
 * in NO COPY ON WRITE (!!!), until an execve is executed. This
 * is no problem, but for the stack. This is handled by not letting
 * main() use the stack at all after fork(). Thus, no function
 * calls - which means inline code for fork too, as otherwise we
 * would use the stack upon exit from 'fork()'.
 *
 * Actually only pause and fork are needed inline, so that there
 * won't be any messing with the stack from main(), but we define
 * some others too.
 */
#define __NR__exit __NR_exit
static inline _syscall0(int,idle)
static inline _syscall0(int,fork)
static inline _syscall2(int,clone,unsigned long,flags,char *,usp)
static inline _syscall0(int,pause)
static inline _syscall0(int,setup)
static inline _syscall0(int,sync)
static inline _syscall0(pid_t,setsid)
static inline _syscall3(int,write,int,fd,const char *,buf,off_t,count)
static inline _syscall1(int,dup,int,fd)
static inline _syscall3(int,execve,const char *,file,char **,argv,char **,envp)
static inline _syscall3(int,open,const char *,file,int,flag,int,mode)
static inline _syscall1(int,close,int,fd)
static inline _syscall1(int,_exit,int,exitcode)
static inline _syscall3(pid_t,waitpid,pid_t,pid,int *,wait_stat,int,options)
/*Posh2 system call for testing*/
static inline _syscall2(int,test,int,a,void *,p)

/*
 * This is the mechanism for creating a new kernel thread.
 *
 * NOTE! Only a kernel-only process(ie the swapper or direct descendants
 * who haven't done an "execve()") should use this: it will work within
 * a system call from a "real" process, but the process memory space will
 * not be free'd until both the parent and the child have exited.
 */

//Posh2 -Modified with SH command set taking H8 as reference

 //Posh2 nop is required or not? argument pushing to reg ro is correct or not

static inline pid_t kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
{
	register long retval __asm__ ("r0") = __NR_clone;
	register long clone_arg __asm__ ("r1") = flags | CLONE_VM;
	unsigned long fs;

	fs = get_fs();
	set_fs (KERNEL_DS);

	__asm__ __volatile__
	  (	"mov %1,r0 \n\t"		// Posh2 Move __NR_clone to r0
		"mov r15,r2 \n\t"		// Posh2 move SP contents to r5 reg
		"trapa #32 \n\t"		// Posh2 call clone system call
		"cmp/eq r15,r2 \n\t"	// Posh2 if r15 == r5,T ==1 ie parent so do nothing
		"bt 1f \n\t"			// Posh2 if T ==1 branch to 1f.ie parent do nothing
		"nop \n\t"
		"mov %3,r0 \n\t"		// Posh2 push arg 
		"mov %4,r10 \n\t"		//
		"jsr @r10 \n\t"			//Posh2	call fn
		"nop \n\t"
		"mov %2,r0 \n\t"		// Posh2 Move __NR_exit to r0
		"trapa #32 \n\t"		// Posh2 Call exit system call
		"1:"
		: "=r" (retval)
		: "i" (__NR_clone), "i" (__NR_exit),
	     "r" (arg), "r" (fn), "r" (clone_arg)
		: "r0","r2","r10");

	set_fs (fs);
	return retval;
}

static inline pid_t wait(int * wait_stat)
{
	return waitpid(-1,wait_stat,0);
}

#endif

#endif /* ASM_SH7615_UNISTD_H_ */
