/* 
 * File:   customer.h
 * Author: Joshua Wright
 * Assignment Number: 2
 * Created on September 17, 2015, 9:23 AM
 */

#ifndef PQ_H
#define	PQ_H
#include <stdlib.h>

typedef struct Customer
{
 double PQtime;   
 double arrivalTimes;
 double startOfServiceTime;
 double departureTime;
 struct Customer *nextCust; //for FIFO
}customerT;

customerT *createCustomer(void);

int freeCustomer(customerT *);

int cust_Switch(customerT **cust1, customerT **cust2);

int cust_Dup(customerT *from, customerT *going);

#endif	/* PQ_H */

