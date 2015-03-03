/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 5
 * Modified by: Casey O'Hare & Sam Ostlund
 */
/* Embedded XINU, Copyright (C) 2010.  All rights reserved. */
  
#include <kernel.h>
#include <proc.h>
#include <stdarg.h>
#include <mips.h>
#include <string.h>

local newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a process to start running a procedure.
 * @param procaddr procedure address
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall	create(void *procaddr, ulong ssize, ulong priority,
			   char *name, ulong nargs, ...)
{
	ulong   *saddr;     /* stack address                */
	ulong   *savargs;   /* pointer to arg saving region */
	ulong   pid;        /* stores new process id        */
	pcb     *ppcb;      /* pointer to proc control blk  */
	ulong   i;
	va_list	ap;         /* points to list of var args   */
	ulong   pads;       /* padding entries in record.   */
	void	INITRET(void);

	if (ssize < MINSTK) ssize = MINSTK;
	ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;  
                                    /* round up to even boundary    */
	saddr = (ulong *)getstk(ssize); /* allocate new stack and pid   */
	pid   = newpid();
	                                /* a little error checking      */ 
	if ( (((ulong *)SYSERR) == saddr) || (SYSERR == pid) )
	{ return SYSERR; }

	numproc++;
	ppcb = &proctab[ pid ];
                                    /* setup PCB entry for new proc */
	ppcb->state   = PRSUSP;

	// TODO: Setup PCB entry for new process.
	ppcb->stkbase = (void *)((ulong)saddr - ssize);
	ppcb->stklen = ssize;
	ppcb->priority = priority;
	for(i = 0; (ppcb->name[i] = name[i]) != '\0'; i++) ;

	/* Initialize stack with accounting block. */
	*saddr   = STACKMAGIC;
	*--saddr = pid;
	*--saddr = ppcb->stklen;
	*--saddr = (ulong)ppcb->stkbase;
	
	if (0 == nargs)
	{ pads = 4; } 
	else if (0 == (nargs % 4))           /* Must pad record size to      */
	{ pads = 0; }                        /*  multiple of native memory   */
	else                                 /*  transfer size.              */
	{ pads = 4 - (nargs % 4); }

	for (i = 0; i < pads; i++) { *--saddr = 0; }
                                    /* Reserve space for all args.  */
	for (i = nargs ; i > 0 ; i--) { *--saddr = 0; }
	savargs = saddr;

	// TODO: Initialize process context.
	*--saddr = (ulong)procaddr;
	*--saddr = (ulong)INITRET;
	for(i = CONTEXT_WORDS - 2; i > 0; i--)
		*--saddr = 0;
	ppcb->stkptr = saddr;

	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.
	va_start(ap, nargs);
	for(i = 0; i < nargs; i++)
	{
		*savargs++ = va_arg(ap, ulong);
	}
	va_end(ap);

	return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
local newpid(void)
{
	int	pid;			/* process id to return     */
	static int nextpid = 0;

	for (pid = 0 ; pid < NPROC ; pid++) 
	{	                /* check all NPROC slots    */
		nextpid = (nextpid + 1) % NPROC;
	    if (PRFREE == proctab[nextpid].state)
		{ return nextpid; }
	}
	return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
	kill( currpid );
}
