/* 
 * File:   calc.h
 * Author: Joshua Wright
 * Assignment Number: 2
 * Created on September 19, 2015, 10:03 AM
 */

#ifndef CALC_H
#define	CALC_H

#include <stdlib.h>
#include <math.h>

typedef struct calc{
    double avg_Arrivals;
    double avg_Num_Serv;
    int avail_Serv;
    double last_Arrival;
    double last_Dep;
    double startIdle;
    int serv_Chan;
    int num_Of_Arrivals;
    int remaining_Cust;
    
}calcT;

float perc_Idle_Time(float avg_Arrivals, float avg_Num_Serv, int serv_Chan);

float avg_Num_n_System(float avg_Arrivals, float avg_Num_Serv, int serv_Chan, float perc_Idle_Time);

float avg_Time_n_System(float avg_Arrivals, float avg_Num_Serv);

float avg_Cust_n_Queue(float avg_Num_n_System, float avg_Arrivals, float avg_Num_Serv);

float avg_Time_Wait_n_Queue(float avg_Cust_n_Queue, float avg_Arrivals);

float util_Fac(float avg_Arrivals, float avg_Num_Serv, int serv_Chan);

long factorial(int i);


#endif	/* CALC_H */

