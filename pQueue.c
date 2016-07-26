/*
 * File: pQueue.c
 * Author: Joshua Wright
 * Assignment Number: 3
 */
#include "pQueue.h"
#include "queue.h"
#include <stdio.h>

pQueueT *createPQ(void)
{
    pQueueT *newQueue = (pQueueT *) malloc(sizeof(pQueueT));
    int i;
    
    newQueue->length = 0;
    for(i =1; i <= max_length; i++){
        newQueue->customers[i] = NULL;
    }
    
    return newQueue;
}
int enqueuePQ(customerT *customer, pQueueT *pQueue)
{
    if(pQueue == NULL || customer == NULL){
        return 0;
    }
    
    pQueue->length++;
    pQueue->customers[pQueue->length] = customer;
    if(pQueue->length > 1){
        if(!upPQ(pQueue, pQueue->length)){
            return 0;
        }
    }

    return 1;
}
customerT *dequeuePQ(pQueueT *pQueue)
{
    if(pQueue == NULL || pQueue->length == 0){
        return NULL;
    }

    customerT *customer = createCustomer();
    
    cust_Dup(pQueue->customers[1], customer);
    cust_Dup(pQueue->customers[pQueue->length], pQueue->customers[1]);
    freeCustomer(pQueue->customers[pQueue->length]);
    pQueue->customers[pQueue->length] = NULL;
    
    pQueue->length--;
    
    downPQ(pQueue, 1);
    
    if(customer == NULL)
        fprintf(stderr, "Queue is Null");
    
    return customer;
}
int upPQ(pQueueT *pQueue, int length)
{   
    if(pQueue->customers[length] == NULL || pQueue->customers[length / 2] == NULL){
        return 0;
    }
    
    if(length > 1 && pQueue->customers[length]->PQtime < pQueue->customers[length / 2]->PQtime){
        cust_Switch(&pQueue->customers[length], &pQueue->customers[length / 2]);
        upPQ(pQueue, length / 2);
    }
    
    return 1;
}
int downPQ(pQueueT *pQueue, int length)
{
    if (pQueue->customers[length] == NULL || pQueue->customers[length * 2] == NULL || length > (pQueue->length / 2) || length < 1) { 
        return 0;
    }
    
    // swap if parent is greater than the least child
    customerT **child;
    if (pQueue->customers[length * 2 + 1] != NULL){
        child = (pQueue->customers[length * 2]->PQtime < pQueue->customers[length * 2 + 1]->PQtime) ? &pQueue->customers[length * 2] : &pQueue->customers[length * 2 + 1];
    }else{
        child = &pQueue->customers[length * 2];    
    }
    
    if (pQueue->customers[length]->PQtime > (*child)->PQtime) {
        cust_Switch(&pQueue->customers[length], child);
        
        // keep percolating down unless already halfway point
        if (length < (pQueue->length / 2)) {
            downPQ(pQueue, length + 1);
        }
    } else {
        return 1;
    }

return 1;
}
int pQfull(pQueueT *pQueue)
{
    if(pQueue == NULL || pQueue->length < max_length - 1){
        return 0;
    } else if(pQueue->length == max_length - 1) {
        return 1;
    }else {
        exit(1);
    }
}
int pQempty(pQueueT *pQueue)
{
    if(pQueue->length)
        return 0;
    else
        return 1;
}
int freePQ(pQueueT *pQueue)
{
    int i;
    for(i = 1; i <= pQueue->length; i++){
        freeCustomer(pQueue->customers[i]);
    }
    
    free(pQueue);
    
    return 1;
}
