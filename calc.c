/*
 * File: calc.c
 * Author: Joshua Wright
 * Assignment Number: 2
 */
#include "calc.h"
#include <stdio.h>

float perc_Idle_Time(float avg_Arrivals, float avg_Num_Serv, int serv_Chan)
{
    int i;
    float summation, temp1, temp2, answer; 
    
    summation = 0;

    for(i = 0; i <= serv_Chan - 1; i++){
	summation += (pow((avg_Arrivals/ avg_Num_Serv), i) / factorial(i));
    }
    
    temp1 = ((1 / (float) factorial(serv_Chan)) * (pow((avg_Arrivals / avg_Num_Serv), serv_Chan)));
    temp2 = ((serv_Chan * avg_Num_Serv) / ((serv_Chan * avg_Num_Serv) - avg_Arrivals));
    answer = 1 / (summation + (temp1 * temp2));
    
    return answer;
}

float avg_Num_n_System(float avg_Arrivals, float avg_Num_Serv, int serv_Chan, float perc_Idle_Time)
{
    float temp1, temp2, temp3, temp4, answer;
    
    temp1 = ((avg_Arrivals * avg_Num_Serv) * (pow((avg_Arrivals / avg_Num_Serv), serv_Chan)));
    temp2 = factorial(serv_Chan - 1);
    temp3 = pow((serv_Chan * avg_Num_Serv - avg_Arrivals), 2);
    temp4 = (temp1 / (temp2 * temp3)) * perc_Idle_Time;
    answer = temp4 + (avg_Arrivals / avg_Num_Serv);

    return answer;
}

float avg_Time_n_System(float avg_Arrivals, float avg_Num_n_System)
{    
    return avg_Num_n_System / avg_Arrivals;
}

float avg_Cust_n_Queue(float avg_Num_n_System, float avg_Arrivals, float avg_Num_Serv)
{
    return avg_Num_n_System - (avg_Arrivals / avg_Num_Serv);
}

float avg_Time_Wait_n_Queue(float avg_Cust_n_Queue, float avg_Arrivals)
{
    return avg_Cust_n_Queue / avg_Arrivals;
}

float util_Fac(float avg_Arrivals, float avg_Num_Serv, int serv_Chan)
{
    return avg_Arrivals / (serv_Chan * avg_Num_Serv);
}
long factorial(int i)
{
    if(i == 0)
	return 1;
 	
    if(i == 1 || i == 1)
        return 1;
    
    int a = i;
    long result = i;
    while(a > 2)
        result *= --a;
    
    return result;
}
