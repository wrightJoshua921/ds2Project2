/*
 *File: customer.c
 *Author: Joshua Wright
 *Assignment Number: 2
 *
 */
#include <stdio.h>
#include "customer.h"

customerT *createCustomer(void)
{
    customerT *nCustomer = (customerT *) malloc(sizeof(customerT));
    nCustomer->arrivalTimes = -1.0;
    nCustomer->startOfServiceTime = -1.0;
    nCustomer->departureTime = -1.0;
    nCustomer->PQtime = -1.0;
    nCustomer->nextCust = NULL;
    
    return nCustomer;
}

int freeCustomer(customerT *customer)
{
    if(customer == NULL){
        return 0;
    }
    
    free(customer);
    return 1;
}
int cust_Switch(customerT **tempA, customerT **tempB)
{
    if(*tempA == NULL || *tempB == NULL){
        return 0;
    }
    
    customerT *tempC = *tempA;
    *tempA = *tempB;
    *tempB = tempC;
    
    return 1;
}
int cust_Dup(customerT *from, customerT *going)
{
    if(from == NULL || going == NULL){
        return 0;
    }
    
    going->PQtime = from->PQtime;
    going->arrivalTimes = from->arrivalTimes;
    going->startOfServiceTime = from->startOfServiceTime;
    going->departureTime = from->departureTime;
    
    return 1;
}
