/***********************************************
* Names:        Marshall Zerbe, Michael Smith
* Course:       EGR226-905
* Project:      Lab 01, Stats Library
* File:         main.c
* Description:  This program implements a stats library to calculate values within a file given.
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "stats_lib.h"

int main()
{
    int n,w=1000, max, min;
    double me, stdev;
    float nums[w], med, var;
    FILE  *fpin;



    fpin = fopen("data_lab1.txt" , "r");                      //opening and reading the file

     if ((fpin = fopen("data_lab1.txt","r")) == NULL)         //making sure the file exists
    {
    printf("File does not exist\n");
    exit(1);
    }

    for (n=0; fscanf(fpin, "%f",&nums[n]) !=EOF; n++);

    max = maximum(nums, n);                                  //finding and printing the maximum value
    printf("The maximum is %d\n",max);

    min = minimum (nums, n);                                 // finding and printing the minimum value
    printf("The minimum is %d\n", min);

    me = mean(nums, n);                                     // finding and printing the mean
    printf("The mean is %lf\n", me);

    med = median(nums, n);                                  //finding and printing the median
    printf("The median is %lf\n", med);

    var = variance(nums, n);                                //finding and printing the variance
    printf("The variance is %lf\n", var);

    stdev = standard_deviation(nums, n);                    // finding and printing the standard deviation
    printf("The standard deviation is %lf", stdev);


    return 0;
}

