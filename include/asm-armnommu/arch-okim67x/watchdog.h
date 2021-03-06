/* (c) 2005 Ben Dooks */

#define WDTCON_REG(x)		((x) + 0xB7E00000)

#define WDTCON			WDTCON_REG(0x00)
#define WDTCON_START		0x3C

#define WDTBCON			WDTCON_REG(0x04)
#define WDTBCON_PROTECT		(0x5A)
#define WDTBCON_WDCLK_MASK	(0x03)
#define WDTBCON_WDCLK_DIV32	(0x00)
#define WDTBCON_WDCLK_DIV64	(0x01)
#define WDTBCON_WDCLK_DIV128	(0x02)
#define WDTBCON_WDCLK_DIV256	(0x03)
#define WDTBCON_ITM		(0x08)
#define WDTBCON_ITEN		(0x10)
#define WDTBCON_SYSRST		(0x40)
#define WDTBCON_HALT		(0x80)

#define WDSTAT			WDTCON_REG(0x14)
#define WDSTAT_WDRST		(0x01)
#define WDSTAT_IST		(0x10)
#define WDSTAT_IVIST		(0x20)
