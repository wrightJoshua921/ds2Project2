/*
 * File: queue.c 
 * Author: Joshua Wright
 * Assignment Number: 2
 */
#include "queue.h"

queueT *createQueue(void)
{
    queueT *queue = (queueT *) malloc(sizeof(queueT));
    
    queue->length = 0;
    queue->head = NULL;
    
    return queue;
}

int enqueue(customerT *customer, queueT *queue)
{
    if(queue == NULL){
        return 0;
    }
    
    if(queue->head == NULL){
        queue->head = customer; // first in queue
        queue->head->nextCust = NULL;
    }
    else{
        customerT *temp = queue->head;
        while(temp->nextCust != NULL){
            temp = temp->nextCust;
        }
        temp->nextCust = customer;
        temp->nextCust->nextCust = NULL;
    }
    queue->length++;
    
    return 1;
}

customerT *dequeue(queueT *queue)
{
    if(queue == NULL || queue->head == NULL){
        return NULL;
    }
    else{
        customerT *customer = queue->head;
        queue->head = queue->head->nextCust;
        queue->length--;
        
        return customer;
    }
}
int qEmpty(queueT *queue)
{
    if(queue->length){
        return 0;
    }
    else{
        return 1;
    }
         
}
int freeQueue(queueT *queue)
{
    if(queue->head == NULL || queue == NULL){
        return 0;
    }
    else{
        customerT *temp1 = queue->head;
        customerT *temp2 = temp1->nextCust;
        while(temp2 != NULL){
            freeCustomer(temp1);
            temp1 = temp2;
            temp2 = temp2->nextCust;
        }
        freeCustomer(temp1);
    }
    free(queue);
    
    return 1;
}
