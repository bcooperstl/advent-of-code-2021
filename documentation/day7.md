# Day 7 #

## Part 1 Requirements ##

You are given a group of crabs that can only move horizontally. You are provided with their start positions. They need to be moved to be in alignment.

You need to calculate the optimal position of the crabs, so that they have to make the least amount of movement so they all line up. It costs one unit of fuel for each crab to move one position.

After determining the optimal position, calculate the amount of fuel used for the crabs to get to that position and return that value.

## Part 2 Requirements ##

This is similar to part 1, but does not use 1 unit of fuel for each step. Instead, the first step uses 1 unit of fuel, and each subsequent uses 1 more unit that the prior.

### Input Format ###

Comma-separated list of horizonal positions of the crabs.

### Output Format ###

#### Part 1 ####

Integer of the total fuel used to move the crabs to the optimal position.

#### Part 2 ####

Integer of the total fuel used to move the crabs to the optimal position.

## Test Cases ##

### Given Test Cases ###

There is one given [test case](../data/test_cases/day7_test1.txt).

### Custom Test Cases ###


## Approach ##

For part 1, since each step uses 1 unit of fuel, the median value will provide the optimal postion.

For part 2, since each step goes up by 1 unit of fuel from the prior, the mean (average) value will provide the optimal position. Because of integer limits, this could be within 1 value either way of the mean, so we will calculate for all 3 values.

### Part 1 ###

- Parse the input file as a list of longs *positions*
- Sort the *positions* array
- Find the median value (can use length/2) and store as *optimal*
- Initialize a *total_fuel* as 0
- Loop over *positions* array
    - Increment *total_fuel* by the absolute value of (*positions[i]* - *optimal*)
- **Output** the *total_fuel* value

### Part 2 ###

#### Calculate_part2_fuel function ####
There will be a calculate_part2_fuel funciton that takes the optimal and starting positions as arguments

- calculate absolute value of ((*optimal* - *starting*) - 1) and store in *steps*
- return (*steps* ** (*steps* + 1))/2

#### Main Loop ####

- Parse the input file as a list of longs *positions*
- Sort the *positions* array
- Find the mean value and store as *optimal*
- Initialize a *total_fuel[3]* array s {0,0,0}
- Loop over *positions* array
    - Increment *total_fuel* by the calculate_part2_fuel(*optimal*, *positions[i]*)
    - Increment *total_fuel* by the calculate_part2_fuel(*optimal*, *positions[i]*)
    - Increment *total_fuel* by the calculate_part2_fuel(*optimal*, *positions[i]*)
- **Output** the minimum of the *total_fuel* values


## Things I learned ##

Thinking through the optimal algorithm to figure out that the median will determine the optimal position.

Looking up how to use the standard template library's sort algorithm.

Learned of the c++ lround function for calculating the average value