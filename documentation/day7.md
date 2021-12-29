# Day 7 #

## Part 1 Requirements ##

You are given a group of crabs that can only move horizontally. You are provided with their start positions. They need to be moved to be in alignment.

You need to calculate the optimal position of the crabs, so that they have to make the least amount of movement so they all line up. It costs one unit of fuel for each crab to move one position.

After determining the optimal position, calculate the amount of fuel used for the crabs to get to that position and return that value.

## Part 2 Requirements ##

### Input Format ###

Comma-separated list of horizonal positions of the crabs.

### Output Format ###

#### Part 1 ####

Integer of the total fuel used to move the crabs to the optimal position.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is one given [test case](../data/test_cases/day7_test1.txt).

### Custom Test Cases ###


## Approach ##

### Part 1 ###

- Parse the input file as a list of longs *positions*
- Sort the *positions* array
- Find the median value (can use length/2) and store as *optimal*
- Initialize a *total_fuel* as 0
- Loop over *positions* array
    - Increment *total_fuel* by the absolute value of (*positions[i]* - *optimal*)
- **Output** the *total_fuel* value


## Things I learned ##

Thinking through the optimal algorithm to figure out that the median will determine the optimal position.

Looking up how to use the standard template library's sort algorithm.
