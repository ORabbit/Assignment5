#include <queue.h>

short prioritize(short pid, queue q, ulong key){

	//pcb *proc;
	//proc = &proctab[ pid ];
	qentry newq;
	newq->key = key;
	int isfound = FALSE;
	int head = queuehead(q);	

	while(queuetab[head].next != NULL && !isfound){
	if(key <= queuetab[head].key)
		if(key <= queuetab[head].next.key)
			continue;
		else{
			queuetab[head].prev.next = pid;
			queuetab[pid].prev = queuetab[head].prev;
			queuetab[pid].next = head;
			queuetab[head].prev = pid;
			isfound = TRUE;
		}
		
		head = queuetab[head].next;	
	}
	return pid;
}
