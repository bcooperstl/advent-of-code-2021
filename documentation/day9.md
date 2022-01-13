# Day 9 #

## Part 1 Requirements ##

You are given a heightmap of the floor levels of the cave, with 9 being the highest and 0 being the lowest.

You need to identify low points - those that are lower than any of it's neighbors (north, south, east and west).

The risk level of a low point is 1 plus its height.

Output the sum of the risk levels of all of the low points.

## Part 2 Requirements ##

### Input Format ###

A heightmap, consiting of a *m* x *n* grid of single-digit integers 0-9. Each integer corresponds to the height at a point.

### Output Format ###

#### Part 1 ####

Integer of the sum of the risk levels of all of the low points on the heightmap.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day9_test1.txt) provided showing sample case.

### Custom Test Cases ###

I made up three test cases [1](../data/test_cases/day9_test2.txt), [2](../data/test_cases/day9_test3.txt), [3](../data/test_cases/day9_test4.txt).

These flip the original input horizontally, vertically, and both horizontally and vertically. This is to check that the low point on a corner is always found. 

## Approach ##

### Input Parsing ###

Start by parsing the input as a list of strings. From there, I will populate the values into a 2-D array. By inspection of the data file, I know the largest I need is 100x100.

I will need to return the populated array, as well as the number of rows and number of columns.

### Part 1 Processing ###

- Parse the input into *heightmap*, *num_rows*, and *num_cols*
- Initialize a *risk_sum* value as 0.
- Loop over each row
    - Loop over each column
        - Check up to 4 neighbors to determine if this is a lowpoint.
        - If it is a lowpoint
            - Add the value + 1 to *risk_sum*
- **Output** the *risk_sum* value

## Things I learned ##

Had to re-lookup how to pass a defined 2D array by reference to a function.