#include <queue.h>
#include <kernel.h> // For ulong
#include <proc.h> // For isbadpid(pid)

/**
 *
 * Created by: Casey O'Hare & Sam Ostlund
 *
 * */

void insert(short pid, queue q, ulong key, int head);

short prioritize(short pid, queue q, ulong key){

	//pcb *proc;
	//proc = &proctab[ pid ];
	//qentry newq;
	//newq->key = key;
	//kprintf("Before: "); //temp
	//printqueue(q); // temp
	
	if (isbadqueue(q) || isbadpid(pid)) // ERROR checking
	{ return SYSERR; }

	int isfound = FALSE;
	int head = queuehead(q);
	if(isempty(q)) { insert(pid, q, key, queuetab[head].next); /*kprintf("After: "); printqueue(q);*/ return pid; }
	
	do{
		head = queuetab[head].next; // testing
		if(key > queuetab[head].key)
		{
			//if(key > queuetab[queuetab[head].next].key)
			//{
				insert(pid, q, key, head);
				//queuetab[queuetab[head].prev].next = pid;
				//queuetab[pid].prev = queuetab[head].prev;
				//queuetab[pid].next = head;
				//queuetab[head].prev = pid;
				isfound = TRUE;
			//}
		}
		//head = queuetab[head].next;
	} while(queuetab[head].next != queuetail(q) && !isfound);
	
	if(!isfound) // Never found it, so we must insert before the tail
		insert(pid, q, key, queuetail(q));

	//kprintf("After: "); //temp
	//printqueue(q); // TEMP
	return pid;
}

void insert(short pid, queue q, ulong key, int head)
{
	queuetab[queuetab[head].prev].next = pid;
	queuetab[pid].prev = queuetab[head].prev;
	queuetab[pid].next = head;
	queuetab[head].prev = pid;
	
	queuetab[pid].key = key;
}

/* TEMPORARY: to print out the queue. For debugging purposes. */
void printqueue(queue q)
{
	int head = queuehead(q);

	//kprintf("head pid: %d, head key: %d\ttail: %d, tail key: %d\n", head, queuetab[head].key, queuetail(q), queuetab[queuetail(q)].key);
	kprintf("| pid, key |\n");

	while(queuetab[head].next != queuetail(q))
	{
		head = queuetab[head].next;
		kprintf("| %d, %d ", head, queuetab[head].key);
	}
	kprintf("|\n");
}
