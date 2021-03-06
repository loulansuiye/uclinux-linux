/*
 * linux/arch/arm/lib/ll_char_wr.S
 *
 * Copyright (C) 1995, 1996 Russell King.
 *
 * Speedups & 1bpp code (C) 1996 Philip Blundel & Russell King.
 *
 * 10-04-96	RMK	Various cleanups & reduced register usage.
 */

@ Regs: [] = corruptable
@       {} = used
@       () = dont use

#include <asm/assembler.h>
		.text

		.global	ll_write_char

#define BOLD            0x01
#define ITALIC          0x02
#define UNDERLINE       0x04
#define FLASH           0x08
#define INVERSE         0x10

LC0:		.word	bytes_per_char_h
		.word	video_size_row
		.word	cmap_80
		.word	con_charconvtable

ll_write_char:	stmfd	sp!, {r4 - r7, lr}
@
@ Smashable regs: {r0 - r3}, [r4 - r7], (r8 - fp), [ip], (sp), [lr], (pc)
@
		eor	ip, r1, #UNDERLINE << 24
/*
 * calculate colours
 */
		tst	r1, #INVERSE << 24
		moveq	r2, r1, lsr #8
		moveq	r3, r1, lsr #16
		movne	r2, r1, lsr #16
		movne	r3, r1, lsr #8
		and	r3, r3, #255
		and	r2, r2, #255
/*
 * calculate offset into character table
 */
		and	r1, r1, #255
		mov	r1, r1, lsl #3
/*
 * calculate offset required for each row [maybe I should make this an argument to this fn.
 * Have to see what the register usage is like in the calling routines.
 */
		adr	r4, LC0
		ldmia	r4, {r4, r5, r6, lr}
		ldr	r4, [r4]
		ldr	r5, [r5]
/*
 * Go to resolution-dependent routine...
 */
		cmp	r4, #4
		blt	Lrow1bpp
		eor	r2, r3, r2			@ Create eor mask to change colour from bg
		orr	r3, r3, r3, lsl #8		@ to fg.
		orr	r3, r3, r3, lsl #16
		add	r0, r0, r5, lsl #3		@ Move to bottom of character
		add	r1, r1, #7
		ldrb	r7, [r6, r1]
		tst	ip, #UNDERLINE << 24
		eoreq	r7, r7, #255
		teq	r4, #8
		beq	Lrow8bpplp
@
@ Smashable regs: {r0 - r3}, [r4], {r5 - r7}, (r8 - fp), [ip], (sp), {lr}, (pc)
@
		orr	r3, r3, r3, lsl #4
Lrow4bpplp:	ldr	r7, [lr, r7, lsl #2]
		mul	r7, r2, r7
		tst	r1, #7				@ avoid using r7 directly after
		eor	ip, r3, r7
		str	ip, [r0, -r5]!
		LOADREGS(eqfd, sp!, {r4 - r7, pc})
		sub	r1, r1, #1
		ldrb	r7, [r6, r1]
		ldr	r7, [lr, r7, lsl #2]
		mul	r7, r2, r7
		tst	r1, #7				@ avoid using r7 directly after
		eor	ip, r3, r7
		str	ip, [r0, -r5]!
		subne	r1, r1, #1
		ldrneb	r7, [r6, r1]
		bne	Lrow4bpplp
		LOADREGS(fd, sp!, {r4 - r7, pc})

@
@ Smashable regs: {r0 - r3}, [r4], {r5 - r7}, (r8 - fp), [ip], (sp), {lr}, (pc)
@
Lrow8bpplp:	mov	ip, r7, lsr #4
		ldr	ip, [lr, ip, lsl #2]
		mul	r4, r2, ip
		and	ip, r7, #15
		eor	r4, r3, r4
		ldr	ip, [lr, ip, lsl #2]
		mul	ip, r2, ip
		tst	r1, #7
		eor	ip, r3, ip
		sub	r0, r0, r5
		stmia	r0, {r4, ip}
		LOADREGS(eqfd, sp!, {r4 - r7, pc})
		sub	r1, r1, #1
		ldrb	r7, [r6, r1]
		mov	ip, r7, lsr #4
		ldr	ip, [lr, ip, lsl #2]
		mul	r4, r2, ip
		and	ip, r7, #15
		eor	r4, r3, r4
		ldr	ip, [lr, ip, lsl #2]
		mul	ip, r2, ip
		tst	r1, #7
		eor	ip, r3, ip
		sub	r0, r0, r5
		stmia	r0, {r4, ip}
		subne	r1, r1, #1
		ldrneb	r7, [r6, r1]
		bne	Lrow8bpplp
		LOADREGS(fd, sp!, {r4 - r7, pc})

@
@ Smashable regs: {r0 - r3}, [r4], {r5, r6}, [r7], (r8 - fp), [ip], (sp), [lr], (pc)
@
Lrow1bpp:	add	r6, r6, r1
		ldmia	r6, {r4, r7}
		tst	ip, #INVERSE << 24
		mvnne	r4, r4
		mvnne	r7, r7
		strb	r4, [r0], r5
		mov	r4, r4, lsr #8
		strb	r4, [r0], r5
		mov	r4, r4, lsr #8
		strb	r4, [r0], r5
		mov	r4, r4, lsr #8
		strb	r4, [r0], r5
		strb	r7, [r0], r5
		mov	r7, r7, lsr #8
		strb	r7, [r0], r5
		mov	r7, r7, lsr #8
		strb	r7, [r0], r5
		mov	r7, r7, lsr #8
		tst	ip, #UNDERLINE << 24
		mvneq	r7, r7
		strb	r7, [r0], r5
		LOADREGS(fd, sp!, {r4 - r7, pc})

		.globl	con_charconvtable
		.bss
con_charconvtable:
		.space	1024
