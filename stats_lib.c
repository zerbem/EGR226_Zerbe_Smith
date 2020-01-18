/***********************************************
* Names:        Marshall Zerbe, Michael Smith
* Course:       EGR226-905
* Project:      Lab 01, Stats Library
* File:         stats_lib.c
* Description:  This program does the calculations for the custom functions created
************************************************/

#include "stats_lib.h"
#include <math.h>


/***| maximum() |************************************//*
*Brief:    using the values of the data set this function finds the maximum.

*Params:
*            (int) i:        Used to increment where in the array the value should be taken.
            (float) nums[]:  This is the array of the data set.
            (int)   n:       The number of values in the data set.
*Returns:
*            (int) max: value of the data set
********************************************************/
int maximum(float nums[], int n){           //calculating the maximum
   int max = 0, i;
    for (i=0; i<n; i++){
        if (nums[i]> max){
            max = nums[i];
        }
    }

return max;
}
/***| minimum() |************************************//*
*Brief:    Using the values of the data set this function finds the minimum value.
*Params:
*            (int) n:        Number of values in the data set.
            (float) nums[]:  This is the array of the data set.
            (int)   i:       Used to increment where in the array the value should be taken.
*Returns:
*            (int) min:      The minimum value in the data set.
********************************************************/
int minimum(float nums[], int n){              //calculating the minimum
    int min=1000000, i;
    for (i=0; i<n; i++){
        if (nums[i]<min){
            min = nums[i];
        }
    }

return min;
}

/***| mean() |************************************//*
*Brief:    Using the values of the data set this function finds the average value.
*Params:
*            (int) n:        Number of values in the data set.
            (float) nums[]:  This is the array of the data set.
            (int)   i:       Used to increment where in the array the value should be taken.
            (int) sum:      The sum of the all the values in the data set.
*Returns:
*            (double) ave:      The average of the values in the data set.
********************************************************/

double mean(float nums[], int n){               //calculating the mean
        int sum=0, i;
        double ave=0;

        for (i=0; i<n; i++){
            sum = sum + nums[i];
        }
        ave = (double)sum/n;

return ave;
}

/***| median() |************************************//*
*Brief:    Using the values in the data set this function finds the median of the data set.
*Params:
*            (int) n:        Number of values in the data set.
            (float) nums[]:  This is the array of the data set.
            (int) mid:       The middle value of the data set.
*Returns:
*            (float) med:      The average of the values in the data set.
********************************************************/
float median(float nums[], int n){             //calculating the median
    int mid;
    float med;
    mid = (n+1)/2;

    med = nums[mid];


return med;
}

/***| variance() |************************************//*
*Brief:    Using the values of the data set this function finds the variance of the data set.
*Params:
*            (int) n:        Number of values in the data set.
            (float) nums[]:  This is the array of the data set.
            (int)   i:       Used to increment where in the array the value should be taken.
            (int) ds:        The sum of the all the values squared.
            (float) u:       The mean of the data set.
*Returns:
*            (float) s:      The variance of the values in the data set.
********************************************************/
float variance(float nums[], int n){           //calculating the variance
    int ds=0, i;
    float u,s;

    u = mean(nums,n);

    for (i=0;i<n;i++){
        ds = ds + ( (nums[i] - u) * (nums[i] - u) );
    }
     s = (float)(ds / (n-1));

     return s;
}

/***| standard_deviation() |************************************//*
*Brief:    Using the values of the data set this function finds the standard deviation.
*Params:
*            (int) n:        Number of values in the data set.
            (float) nums[]:  This is the array of the data set.
            (float) s:      The variance of the values in the data set.

*Returns:
*            (float) sd:      The standard deviation of the values in the data set.
********************************************************/
double standard_deviation(float nums[], int n){     //calculating the standard deviation
    double s, sd;

    s = variance( nums, n);

    sd = sqrt(s);
return sd;
}
