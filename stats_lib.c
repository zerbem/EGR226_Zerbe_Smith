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
            (float) nums[]:  This is the array of the dataset.
            (int)   n:       The number of values in the dataset.
*Returns:
*            (int) max: value of the dataset
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
*Brief:    Using the values of the dataset this function finds the minimum value.
*Params:
*            (int) n:        Number of values in the dataset.
            (float) nums[]:  This is the array of the dataset.
            (int)   i:       Used to increment where in the array the value should be taken.
*Returns:
*            (int) min:      The minimum value in the dataset.
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
*            (int) n:        Number of values in the dataset.
            (float) nums[]:  This is the array of the dataset.
            (int)   i:       Used to increment where in the array the value should be taken.
            (int) sum:      The sum of the all the values in the dataset.
*Returns:
*            (double) ave:      The average of the values in the dataset.
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
*Brief:    Using the values in the data set this function finds the median of the dataset.
*Params:
*            (int) n:        Number of values in the dataset.
            (float) nums[]:  This is the array of the dataset.
            (int) mid:       The middle value of the dataset.
             (int) i:        Incrementing for sorting the dataset
             (int) j:        Incrementing for sorting the dataset
            (float) tmp:     Dummy variable to help sort values.
*Returns:
*            (float) med:      The average of the values in the dataset.
********************************************************/
float median(float nums[], int n){             //calculating the median
    int mid;
    float med,tmp;
    mid = (n)/2;
    for (int i = 0; i < n; i++)                     //Loop for ordering
	{
		for (int j = 0; j < n; j++)             //Loop for comparing values
		{
			if (nums[j] > nums[i])                //Comparing other values
			{
				int tmp = nums[i];         //Using dummy variable for storing last value
				nums[i] = nums[j];            //replacing value
				nums[j] = tmp;             //storing last value
			}
		}
	}


    med = nums[mid];


return med;
}

/***| variance() |************************************//*
*Brief:    Using the values of the data set this function finds the variance of the dataset.
*Params:
*            (int) n:        Number of values in the dataset.
            (float) nums[]:  This is the array of the dataset.
            (int)   i:       Used to increment where in the array the value should be taken.
            (int) ds:        The sum of the all the values squared.
            (float) u:       The mean of the dataset.
*Returns:
*            (float) s:      The variance of the values in the dataset.
********************************************************/
float variance(float nums[], int n){           //calculating the variance
    int  i;
    float u,s,ds=0;

    u = mean(nums,n);

    for (i=0;i<n;i++){
        ds = ds + (pow(nums[i]- u,2));
    }
     s = (ds / (n-1));

     return s;
}

/***| standard_deviation() |************************************//*
*Brief:    Using the values of the data set this function finds the standard deviation.
*Params:
*            (int) n:        Number of values in the dataset.
            (float) nums[]:  This is the array of the dataset.
            (float) s:      The variance of the values in the dataset.

*Returns:
*            (float) sd:      The standard deviation of the values in the dataset.
********************************************************/
double standard_deviation(float nums[], int n){     //calculating the standard deviation
    double s, sd;

    s = variance( nums, n);

    sd = sqrt(s);
return sd;
}
