#include <kernel.h>
#include <proc.h> // for ulong and isbadpid
#include <queue.h>

/**
 * Created by: Casey O'Hare & Sam Ostlund
 * */

void insert(short pid, queue q, ulong key, int head);

short prioritize(short pid, queue q, ulong key){
	if (isbadqueue(q) || isbadpid(pid)) // ERROR checking
	{ return SYSERR; }

	int isfound = FALSE;
	int head = queuehead(q);
	if(isempty(q)) { insert(pid, q, key, queuetab[head].next); return pid; }
	
	do{
		head = queuetab[head].next;
		if(key > queuetab[head].key)
		{
			insert(pid, q, key, head);
			isfound = TRUE;
		}
	} while(queuetab[head].next != queuetail(q) && !isfound);
	
	if(!isfound) // Never found it, so we must insert before the tail
		insert(pid, q, key, queuetail(q));

	return pid;
}

// Insert a element into the queue and update surronding items
void insert(short pid, queue q, ulong key, int head)
{
	queuetab[queuetab[head].prev].next = pid;
	queuetab[pid].prev = queuetab[head].prev;
	queuetab[pid].next = head;
	queuetab[head].prev = pid;
	
	queuetab[pid].key = key;
}

/* To print out the queue. For debugging purposes. */
void printqueue(queue q)
{
	int head = queuehead(q);

	kprintf("| pid, key |\n");

	while(queuetab[head].next != queuetail(q))
	{
		head = queuetab[head].next;
		kprintf("| %d, %d ", head, queuetab[head].key);
	}
	kprintf("|\n");
}
