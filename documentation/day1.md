# Day 1 #

## Part 1 Requirements ##

Given the input list of depth measurements, count the number of times a measurement increases from the previous measurement.

Note that the first measurement does not count, since there is no zeroth measurement.

## Part 2 Requirements ##

### Input Format ###

List of integers, corresponding to depth measurements.

### Output Format ###

#### Part 1 ####

Integer corresponding to the number of times a measurement increases.

#### Part 2 ####

## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day1_test1.txt) provided showing sample case.

### Custom Test Cases ###

None are needed.

## Approach ##

- Read in the input list of measurements as a list of longs
- Initialize an *increase_counter* to 0
- Loop over the list of measurements with *i* starting at the second (*i* = 1) entry
    - If the *i*th measurement is greater than the *i-1*th measurement
        - Increment the *increase_counter*
- **Output** the *increase_counter*

## Things I learned ##

- How to reinstall my VirtualBox Guest Additions so my shared folder will work again.

