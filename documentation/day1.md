# Day 1 #

## Part 1 Requirements ##

Given the input list of depth measurements, count the number of times a measurement increases from the previous measurement.

Note that the first measurement does not count, since there is no zeroth measurement.

## Part 2 Requirements ##

Given the input list of depth measurements, compute the depths as a three-measurement sliding window (1,2,3 then 2,3,4, then 3,4,5, etc).

With these windows, count the number of times a window increases from the prior window. Again the first window will not have a prior sum.

### Input Format ###

List of integers, corresponding to depth measurements.

### Output Format ###

#### Part 1 ####

Integer corresponding to the number of times a measurement increases.

#### Part 2 ####

Integer corresponding to the number of times a sliding window's value increases.

## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day1_test1.txt) provided showing sample case.

### Custom Test Cases ###

None are needed.

## Approach ##

### Part 1 ###

#### Main Program ####

- Read in the input list of measurements as a list of longs
- Initialize an *increase_counter* to 0
- Loop over the list of measurements with *i* starting at the second (*i* = 1) entry
    - If the *i*th measurement is greater than the *i-1*th measurement
        - Increment the *increase_counter*
- **Output** the *increase_counter*

### Part 2 ###

#### Create Sliding Windows fucntion ####
Input:  vector of measurements

Output: vector of sliding window values based on a sliding window of size 3

- create an empty *window_values* vector
- Iterate over the list of measurements with *i* from *0* to *size - 3*
    - Add the sum of *measurements[i]*, *measurements[i+1]*, and *measurements[i+2]* to *window_values*
- return the *window_values* vector

#### Main Program ####

- Read in the input list of measurements as a list of longs
- Use the CreateSlidingWindows function to get the sliding windows values
- Initialize an *increase_counter* to 0
- Loop over the list of sliding window values with *i* starting at the second (*i* = 1) entry
    - If the *i*th window value is greater than the *i-1*th window value
        - Increment the *increase_counter*
- **Output** the *increase_counter*


## Things I learned ##

- How to reinstall my VirtualBox Guest Additions so my shared folder will work again.

