/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2010, 2013.  All rights reserved. */
/**
	Edited by: Casey O'Hare & Sam Ostlund
*/

#include <mips.h>
	
.text
	.align 4
	.globl	ctxsw
	.ent	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:

	addiu sp, sp, -64
	sw ra, CTX_RA(sp)
	// TODO: Save callee-save ("non-volatile") registers.
	sw s0, CTX_S0(sp)
	sw s1, CTX_S1(sp)
	sw s2, CTX_S2(sp)
	sw s3, CTX_S3(sp)
	sw s4, CTX_S4(sp)
	sw s5, CTX_S5(sp)
	sw s6, CTX_S6(sp)
	sw s7, CTX_S7(sp)
	sw s8, CTX_S8(sp)	/* global pointer */
	sw s9, CTX_S9(sp)	/* frame pointer */
	sw ra, CTX_JA(sp)

	// TODO: Save outgoing stack pointer.
	sw sp, 0(a0)

	// TODO: Load incoming stack pointer.
	lw sp, 0(a1)

	// TODO: Restore callee-save ("non-volatile") registers.
	lw s0, CTX_S0(sp)
	lw s1, CTX_S1(sp)
	lw s2, CTX_S2(sp)
	lw s3, CTX_S3(sp)
	lw s4, CTX_S4(sp)
	lw s5, CTX_S5(sp)
	lw s6, CTX_S6(sp)
	lw s7, CTX_S7(sp)
	lw s8, CTX_S8(sp)	/* global pointer */
	lw s9, CTX_S9(sp)	/* frame pointer */

	// TODO: Jump to next function.
	lw a0, 64(sp)
	lw a1, 68(sp)
	lw a2, 72(sp)
	lw a3, 76(sp)
	lw ra, CTX_RA(sp)
	lw v0, CTX_JA(sp)
	addiu sp, sp, 64
	jr v0

	.end ctxsw
