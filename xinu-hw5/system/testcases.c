/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by: Casey O'Hare & Sam Ostlund
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <device.h>
#include <stdio.h>
#include <uart.h>
#include <proc.h>
#include <queue.h>


void bigargs(int a, int b, int c, int d, int e, int f)
{
	kprintf("bigargs(%d, %d, %d, %d, %d, %d) == %d\r\n",
			a, b, c, d, e, f, a+b+c+d+e+f);
}
 
void printpid(int times)
{
	int i = 0;
	   
	enable();
	for (i = 0; i < times; i++)
	{
		kprintf("This is process %d\r\n", currpid);
		resched();
	}
}

void printiran(void)
{
	kprintf("I RAN\r\n");
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	int c;

	kprintf("0) Test priority scheduling\r\n");
	kprintf("1) Test priority scheduling order by printing priority queue\r\n");
	kprintf("A) Test AGING by printing priority queue and putting a small priority at the bottom with other high priorities\r\n");
	kprintf("P) Test preemption...\r\n");

	kprintf("===TEST BEGIN===\r\n");

	// TODO: Test your operating system!

	c = kgetc();
	switch(c)
	{
	case	'0':
		ready(create((void *)printpid, INITSTK, 2, "PRINTER-A", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 5, "PRINTER-B", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 10, "PRINTER-C", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 5, "PRINTER-D", 1, 5), 0);

		ready(create((void *)bigargs, INITSTK, 20, "BIGARGS", 6, 10, 20, 30, 40, 50, 60), 0);
		break;

	case	'1':
		ready(create((void *)printpid, INITSTK, 2, "PRINTER-A", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 5, "PRINTER-B", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 10, "PRINTER-C", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 5, "PRINTER-D", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 1, "PRINTER-E", 1, 5), 0);

		irqmask i = disable(); // So that the order of the priority doesn't change during our run and change the results
		kprintf("Expecting: | pid, key |\r\n");
		kprintf("| 4, 10 | 3, 5 | 5, 5 | 2, 2 | 6, 1 | 0, 0 |\r\n");
		kprintf("Actual: ");
		printqueue(readylist);
		restore(i); // Back to normal interupts

		break;

	case	'A':
		kprintf("When AGING=FALSE then the lowest process key will only run once all others (excluding null) have ran; however, when AGING=TRUE then the lowest process key will run BEFORE the others finish. Lowest proccess key will print, \"I RAN\" when it runs.\r\n");
		ready(create((void *)printpid, INITSTK, 16, "PRINTER-A", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 12, "PRINTER-B", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 13, "PRINTER-C", 1, 5), 0);
		ready(create((void *)printpid, INITSTK, 15, "PRINTER-D", 1, 5), 0);
		ready(create((void *)printiran, INITSTK, 1, "PRINTER-I RAN", 0), 0);

		break;

	case	'P':
		// STUFF
		break;

	default:
		break;
	}

	while (numproc > 2) { resched(); }
	kprintf("\r\n===TEST END===\r\n");
	return;
}
