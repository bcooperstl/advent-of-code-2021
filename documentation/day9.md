# Day 9 #

## Part 1 Requirements ##

You are given a heightmap of the floor levels of the cave, with 9 being the highest and 0 being the lowest.

You need to identify low points - those that are lower than any of it's neighbors (north, south, east and west).

The risk level of a low point is 1 plus its height.

Output the sum of the risk levels of all of the low points.

## Part 2 Requirements ##

You are given a heightmap of the floor levels of the cave, with 9 being the highest and 0 being the lowest.

You need to identify basins. These start at the low points as described in part 1, and contain every point that will flow down to that lowpoint.

Locations with height 9 do not count to basins.

Find the three largest basins, and output their product.

### Input Format ###

A heightmap, consiting of a *m* x *n* grid of single-digit integers 0-9. Each integer corresponds to the height at a point.

### Output Format ###

#### Part 1 ####

Integer of the sum of the risk levels of all of the low points on the heightmap.

#### Part 2 ####

Integer of the product of the three largest basins.

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

### Part 2 Processing ###

#### ClearBasinMembers Function ####

Takes the *basinmap* and resets all the values to false.

#### CountBasinMembers Function ####

Returns the number of values of *basinmap* that are true.

#### CheckNeighbors Recursive Function ####

Takes the *heightmap*, *basinmap*, *num_rows*, *num_cols*, *row*, and *col* values as parameters.
*row* and *col* specify the position of which to check the neighbors.

- If *row* > *0*, we will check the north neighbor at *row-1*,*col*
- If *row* < *num_rows - 1*, we will check the south neighbor at *row+1+,*col*
- If *col* > *0*, we will check the west neighbor at *row*,*col-1*
- If *col* < *num_cols - 1*, we will check the east neighbor at *row*,*col+1*

To check a neighbor:
- If *basinmap[new_row][new_col]* is false and *heightmap[new_row][new_col]* > *heightmap[row][col]* and *heightmap[new_row][new_col]* < *9*
    - Mark *basinmap[new_row][new_col]* to true
    - Call CheckNeighbors passing *heightmap*, *basinmap*, *num_rows*, *num_cols*, *new_row*, and *new_col* to recursively check its neighbors

#### Main Loop ####

- Parse the input into *heightmap*, *num_rows*, and *num_cols*
- Initialize a *largest_basins* array of 3 values to each be 0. This will contain the three largest basins, in descending order.
- Declare a *basinmap* that is the same size as *heightmap*
- Loop over each row
    - Loop over each column
        - Check up to 4 neighbors to determine if this is a lowpoint.
        - If it is a lowpoint
            - call *ClearBasinMembers* to clear the *basinmap*
            - map *basinmap[row][column]* to true
            - call CheckNeighbors giving the current row and column as the starting point
            - call *CountBasinMembers* and store in *size*
            - Check over *largest_basins* and insert *size* if it belongs in the array
- **Output** the product of the three *largest_basins* values

## Things I learned ##

Had to re-lookup how to pass a defined 2D array by reference to a function.

Fun developing the algorithm for part 2, including designing the recursion.