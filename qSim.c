/* 
 * *************************************************************************************************************************
 * File:   qSim.c
 * Author: Joshua Wright
 * Assignment Number: 2
 * Created on September 17, 2015, 9:22 AM
 *
 *
 * This Project is supposed to similate an even driven actions. Depending on if the customer is either arriving or
 * departing. A different action will happen. We will simulate work being done by giving start of service times 
 * and departing times. We will be generating random times for the departing times. we will also "check" to see
 * if there is any available servers to let people get there work done. If there isnt then we will simulate
 * the workers go into a waiting queue to wait until they can get there work done. And so on and so on untile every
 * single person has gotten there work completed. We will then calculate times according to what happend. We will
 * calculate queue total time, system total time, idle total time, the probability of waiting for a server, and
 * various other times. 
 * *********************************************************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"
#include "calc.h"
#include "pQueue.h"
/*
 * 
 * 
 * 
 */
typedef struct Equations{
    double perc_Idle_Time_Equation, perc_Idle_Time_Results;
    double avg_Num_n_System_Equation;
    double avg_Time_n_System_Equation, avg_Time_n_System_Results;
    double avg_Cust_n_Queue_Equation;
    double avg_Time_Wait_n_Queue_Equation, avg_Time_Wait_n_Queue_Results;
    double util_Fac_Equation, util_Fac_Results;
    double queue_Total_Time;
    double system_Total_Time;
    double idle_Total_Time;
    double prob_Of_Waiting_For_Server;
    int waited_Count;
}equationsT;
/*
 * 
 * 
 * 
 */
double genRandInterval(int avg);
void getData(calcT *calc);
void setData(calcT *calc);
void calcData(calcT *calc, equationsT *equations);
void primePump(pQueueT * pQueue, calcT *calc);
void simResults(calcT *calc, equationsT *equations);
void doWork(pQueueT *pQueue, queueT *queue, calcT *calc, equationsT *equations);
void equationsInit(equationsT *equations);
void printStats(equationsT *equations);
/*
 * 
 * 
 * 
 */
int main(int argc, char** argv)
{
//create priority queue, fifo queue, and the structs calc, and equations 
    pQueueT *PQ = createPQ();
    queueT *fifoQueue = createQueue();
    calcT calc;
    equationsT equations;

//initialize values in the struct equations to 0
    equationsInit(&equations);

//get the data from the file runSimulation.txt and set the data and calculate the data to the equations
    getData(&calc);
    setData(&calc);
    calcData(&calc, &equations);

//fill the priority queue up with customers
    primePump(PQ, &calc);
    
//run this loop untile basically all customers have been completed
    while(!pQempty(PQ)){
        doWork(PQ, fifoQueue, &calc, &equations);
        if(calc.remaining_Cust && PQ->length <= calc.serv_Chan + 1){
            primePump(PQ, &calc);
        }
    }
   
//use the times and data from the events to get the results and compare them to the calculated equation data
    simResults(&calc, &equations);
    printStats(&equations);

//free allocated memory
    freePQ(PQ);
    freeQueue(fifoQueue);
    
    return(0);
}
void printStats(equationsT *equations)
{
    printf("Percent Idle Time (calculated, actual): %lf, %lf\n", equations->perc_Idle_Time_Equation, equations->perc_Idle_Time_Results);
    printf("Average number of people in the system (calculated): %lf\n", equations->avg_Num_n_System_Equation);
    printf("Average time each customer spends in the system (calculated, actual): %lf, %lf\n", equations->avg_Time_n_System_Equation, equations->avg_Time_n_System_Results);
    printf("Average number of customers in the queue (calculated): %lf\n", equations->avg_Cust_n_Queue_Equation);
    printf("Average time each customer spends in the queue (calculated, actual): %lf, %lf\n", equations->avg_Time_Wait_n_Queue_Equation, equations->avg_Time_Wait_n_Queue_Results);
    printf("System Utilization Factor (calculated, actual): %lf, %lf\n", equations->util_Fac_Equation, equations->util_Fac_Results);
    printf("Probability of having to wait for service: %lf\n", equations->prob_Of_Waiting_For_Server);
}
void equationsInit(equationsT *equations)
{
    equations->waited_Count = 0;
    equations->idle_Total_Time = 0;
    equations->queue_Total_Time = 0;
    equations->system_Total_Time = 0;    
}

void doWork(pQueueT *pQueue, queueT *queue, calcT *calc, equationsT *equations)
{
//take the first customer from the priority queue
    customerT *customer = dequeuePQ(pQueue);

//if customer is empty then there was a problem retrieving the customer
    if(customer == NULL)
	{
        	exit(1);
    	}

//if departure time is less then 0 then the customer is an arrival
    if(customer->departureTime < 0)
	{
		//check to see if there is a server available
		//if there is then times will be set       
        	if(calc->avail_Serv)
		{
        		customer->startOfServiceTime = customer->arrivalTimes;
        		customer->departureTime = customer->PQtime = customer->startOfServiceTime + genRandInterval(calc->avg_Num_Serv);
        		
			//if the available servers is equal to the initial server size then the event is idel
        		if(calc->avail_Serv == calc->serv_Chan)
			{
            			equations->idle_Total_Time += customer->startOfServiceTime - calc->startIdle;
        		}
		
			//after the work has been done the customer goes back into the PQ with a departing time
        		enqueuePQ(customer, pQueue);

			//decrement the available server by 1
        		calc->avail_Serv--;
        
        	}
		else
		{
			//no servers available, so customer has to wait in the fifo queue. customers waited incremenets by 1
                	enqueue(customer, queue);
                	equations->waited_Count++;
        	}
    	}
	else
	{
		//if departure time is greater than 0, then the customer is departing, total system and queue times are incremented
        	equations->system_Total_Time += customer->arrivalTimes - customer->departureTime;
        	equations->queue_Total_Time += customer->arrivalTimes - customer->startOfServiceTime;
        
		//last departure time is set from the departure time and the customer is now free'd
        	calc->last_Dep = customer->departureTime;
        	freeCustomer(customer);
        	
		//available server is free and ready to be used by another customer
        	calc->avail_Serv++;
        
	//if the queue is not empty we take the customer from the queue and set its times accordingly and put the customer in the priortiy queue
        if(!qEmpty(queue))
	{
        	customer = dequeue(queue);
            
                if(customer == NULL)
		{
                	exit(1);
            	}
            	customer->startOfServiceTime = calc->last_Dep;
            	customer->departureTime = customer->PQtime = customer->startOfServiceTime + genRandInterval(calc->avg_Num_Serv);
            	enqueuePQ(customer, pQueue);
            	calc->avail_Serv--;
        }
	else if(calc->avail_Serv == calc->serv_Chan)
	{
        	calc->startIdle = calc->last_Dep;
        }
    }
        
    
}
void simResults(calcT *calc, equationsT *equations)
{
    equations->perc_Idle_Time_Results = equations->idle_Total_Time / calc->last_Dep;
    
    equations->avg_Time_n_System_Results = calc->num_Of_Arrivals / equations->system_Total_Time;
    
    equations->avg_Time_Wait_n_Queue_Results = calc->num_Of_Arrivals / equations->queue_Total_Time;
    
    equations->util_Fac_Results = (calc->serv_Chan * calc->last_Dep) / equations->system_Total_Time;
    
    equations->prob_Of_Waiting_For_Server = (double) equations->waited_Count / calc->num_Of_Arrivals;
}
void primePump(pQueueT * pQueue, calcT *calc)
{
    while(!pQfull(pQueue) && calc->remaining_Cust)
	{
        	customerT *nCustomer = createCustomer();
        	calc->last_Arrival += genRandInterval(calc->avg_Arrivals);
        	nCustomer->arrivalTimes = nCustomer->PQtime = calc->last_Arrival;
        	enqueuePQ(nCustomer, pQueue);
        	calc->remaining_Cust--;
        
        }
}
void setData(calcT *calc)
{
    calc->avail_Serv = calc->serv_Chan;
    calc->remaining_Cust = calc->num_Of_Arrivals;
    calc->last_Arrival = calc->last_Dep = calc->startIdle = 0;
}
void getData(calcT *calc)
{   
    FILE *fp = fopen("runSimulation.txt", "r");
    fscanf(fp, "%lf", &calc->avg_Arrivals);
    fscanf(fp, "%lf", &calc->avg_Num_Serv);
    fscanf(fp, "%d", &calc->serv_Chan);
    fscanf(fp, "%d", &calc->num_Of_Arrivals); 
    
    calc->avail_Serv = 0;
    calc->startIdle = 0;
    calc->remaining_Cust = 0;
    calc->last_Dep = 0;
}
double genRandInterval(int avg)
{
	double a;
    
	do
	{
		srandom((int)clock());    
    		a = ((double)random() / (RAND_MAX));

    	}while(a <= 0 || a > 1);
    
    	double answer = -1.0 * (1.0 / avg) * log(a);
    
    	return answer;
}
void calcData(calcT *calc, equationsT *equations)
{
    equations->perc_Idle_Time_Equation = perc_Idle_Time(calc->avg_Arrivals, calc->avg_Num_Serv, calc->serv_Chan);
    
    equations->avg_Num_n_System_Equation = avg_Num_n_System(calc->avg_Arrivals, calc->avg_Num_Serv, calc->serv_Chan, equations->perc_Idle_Time_Equation);
    
    equations->avg_Time_n_System_Equation = avg_Time_n_System(calc->avg_Arrivals, equations->avg_Num_n_System_Equation);
    
    equations->avg_Cust_n_Queue_Equation = avg_Cust_n_Queue(equations->avg_Num_n_System_Equation, calc->avg_Arrivals, calc->avg_Num_Serv);
    
    equations->avg_Time_Wait_n_Queue_Equation = avg_Time_Wait_n_Queue(equations->avg_Cust_n_Queue_Equation, calc->avg_Arrivals);
    
    equations->util_Fac_Equation = util_Fac(calc->avg_Arrivals, calc->avg_Num_Serv, calc->serv_Chan);
}
