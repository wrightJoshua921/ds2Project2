/* 
 * File: queue.h
 * Author: Joshua Wright
 * Assignment Number: 2
 * Created on September 17, 2015, 12:10 PM
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include <stdlib.h>
#include "customer.h"

typedef struct queue
{
    int length;
    customerT *head;
        
} queueT;


queueT *createQueue(void);

int enqueue(customerT *customer, queueT *queue);

customerT *dequeue(queueT *queue);

int freeQueue(queueT *queue);

int qEmpty(queueT *queue);
#endif	/* QUEUE_H */

