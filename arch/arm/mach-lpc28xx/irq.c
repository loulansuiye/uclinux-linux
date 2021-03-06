/*
 *  linux/arch/arm/mach-lpc28xx/irq.c
 *
 *	Copyright (C) 2007 Siemens Building Technologies
 *	                   mailto:philippe.goetz@siemens.com
 *
 *      Copyright (C) 2004 Philips Semiconductors
 *
 *      Based on linux/arch/arm/mach-pxa/irq.c (C) 2001 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/string.h>

#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

/** interrupt handling ***************************************************/
void __inline__ lpc28xx_mask_irq(struct irq_data *d)
{
	volatile unsigned int *reg;
	INT_PRIOMASK0 = 0x1;
	reg = (volatile unsigned int *) (INT_REQBASE + d->irq * 4);
	*reg = (1<<28)|(1<<27)|(1<<26)|0x1;
}

void __inline__ lpc28xx_unmask_irq(struct irq_data *d)
{
        volatile unsigned int *reg;
	INT_PRIOMASK0 = 0x0;
        reg = (volatile unsigned int *) (INT_REQBASE + d->irq * 4);
	*reg = (1<<28)|(1<<27)|(1<<26)|(1<<16)|0x1;
}

void __inline__ lpc28xx_mask_ack_irq(struct irq_data *d)
{
	lpc28xx_mask_irq(d);
}

/* YOU CAN CHANGE THIS ROUTINE FOR SPEED UP */
__inline__ unsigned int fixup_irq (int irq )
{
	return(irq);
}

static struct irq_chip lpc28xx_chip = {
	.name		= "lpc28xx-irq-chip",
	.irq_ack	= lpc28xx_mask_ack_irq,
	.irq_mask	= lpc28xx_mask_irq,
	.irq_unmask	= lpc28xx_unmask_irq,
};

/** PM management ************************************************************/

#ifdef CONFIG_PM
static unsigned long ic_irq_enable;

static int irq_suspend(struct sys_device *dev, u32 state)
{
	return 0;
}

static int irq_resume(struct sys_device *dev)
{
	/* disable all irq sources */
	return 0;
}
#else
#define irq_suspend NULL
#define irq_resume NULL
#endif

static struct bus_type irq_subsys = {
	.name		= "irq",
	.dev_name	= "irq",
};

static struct device irq_device = {
	.id	= 0,
	.bus	= &irq_subsys,
};

static int __init irq_init_sysfs(void)
{
	int ret = subsys_system_register(&irq_subsys, NULL);
	if (ret == 0)
		ret = device_register(&irq_device);
	return ret;
}

device_initcall(irq_init_sysfs);

/** IRQ Initialization *******************************************************/

void __init lpc28xx_init_irq(void)
{
	int irq;

	for (irq = 0; irq < NR_IRQS; irq++) {
		irq_set_chip_and_handler(irq, &lpc28xx_chip, handle_level_irq);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}

	/* mask and disable all further interrupts set all to IRQ mode*/
	
	INT_REQ1 = (1<<28)|(1<<27)|(1<<26)|0x1;
	INT_REQ2 = (1<<28)|(1<<27)|(1<<26)|0x1;
	INT_REQ3 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ4 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ5 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ6 = (1<<28)|(1<<27)|(1<<26)|0x1;        
	INT_REQ7 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ8 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ9 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ10 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ11 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ12 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ13 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ14 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ15 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ16 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ17 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ18 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ19 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ20 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ21 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ22 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ23 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ24 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ25 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ26 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ27 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ28 = (1<<28)|(1<<27)|(1<<26)|0x1;
        INT_REQ29 = (1<<28)|(1<<27)|(1<<26)|0x1;
	
	//INT_PRIOMASK0 = 0x0;
	//INT_REQ5 = (1<<28)|(1<<27)|(1<<26)|(1<<16)|0x1;

}

