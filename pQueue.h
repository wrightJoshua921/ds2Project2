/* 
 * File: pQueue.h
 * Author: Joshua Wright
 * Assignment Number: 2
 * Created on September 17, 2015, 12:14 PM
 */

#ifndef PQUEUE_H
#define	PQUEUE_H

#include <stdlib.h>
#include "customer.h"
#define max_length 101

typedef struct pQueue
{
    int length;
    customerT *customers[max_length];    
}pQueueT;


pQueueT *createPQ(void);

int enqueuePQ(customerT *, pQueueT *);

customerT *dequeuePQ(pQueueT *);

int freePQ(pQueueT *);

int upPQ(pQueueT *, int index);

int downPQ(pQueueT *, int index);

int pQempty(pQueueT *);

int pQfull(pQueueT *);



#endif	/* PQUEUE_H */

