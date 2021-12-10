# Day 6 #

## Part 1 Requirements ##

You are given a group of lanternfish of different ages, as defined in the input format.

Every 7 days, a lanternfish produces a new lanternfish. However, the new lanternfish will need an extra two days before it produces its first lanternfish.

Output the number of lanternfish that will be present after the given number of days, which will be provided as an additional parameter.

## Part 2 Requirements ##

### Input Format ###

Comma-separated list of ages as integers of the lanternfish.

### Output Format ###

#### Part 1 ####

Integer of the number of lanternfish at the given day.

#### Part 2 ####

## Test Cases ##

### Given Test Cases ###

There are two examples from [one file](../data/test_cases/day6_test1.txt) provided showing sample case. One will use 18 days, and the other will use 80 days

### Custom Test Cases ###


## Approach ##

### Part 1 ###

- Parse the input file as a list of longs *ages*
- Parse in the extra parameter as an integer *days*
- Create a *counters* array 9 long integers to represent the number of lanternfish of each age, starting at 0
- Loop over *ages* array
    - Increment the counters element that corresponds to the age
- Loop for *days* times
    - Store of the 0-index as *cloning*
    - Loop from with *i* 1 to 8
        - Copy the *counters[i]* to *counters[i-1]* to simulate the aging.
    - Increment *counters[6]* by *cloning* for those lanternfish that will be cloning and restarting at 6 days
    - Set *counters[8]* to *cloning* for the lanternfish that were just cloned
- **Output** the sum of the values in *counters*

## Things I learned ##

Interesting algorithm to come up with and not handle the fish indivdually.
