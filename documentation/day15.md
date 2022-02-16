# Day 15 #

## Part 1 Requirements ##

You are given a 2D map of risk levels (1-9) as described in the input format. This risk level is the amount added when you enter that location

You need to determine the path that your ship will take from the top-left corner to the bottom-right corner which minimizes the total risk level.

Find this path, and then output the total risk level for travelling on the path. This will not include the risk for the top-left square, since you start there.

## Part 2 Requirements ##

This is similar to part 2, but the map is five times larger in each dimension.

The given input is repeated 25 times - filling in the horizontal and vertical.

However, for each time it is repeated, the risk value of each cell increases by 1. If a value becomes 10, it wraps around back to 1.

The same problem and output are required - find the shortest path from the top-left to the bottom-right and output the solution

### Input Format ###

The input is a grid of single-digit integers (values 1-9). There is no separation between integers within a row.

### Output Format ###

#### Part 1 ####

A number corresponding to the total risk level of the least risky path.

#### Part 2 ####

A number corresponding to the total risk level of the least risky path.

## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day15_test1.txt) provided showing sample case.

### Custom Test Cases ###

None needed.

## Approach ##

I will be implementing [Dijkstra's Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) to determine the least costly path.

### Data Structures ###

There will be a point structure which will store the data for each position in the cavern. It will also have the necessary fields for the Dijkstra algorithm:
- risk_level - which gives the cost of entering this cell
- visited - boolean indicating if the cell has been checked yet for Dijkstra's algorithm
- min_risk_to_here - interver value of the minimal risk to get to this cell from the Dijkstra's algorithm origin cell
- from_direction - either NORTH, SOUTH, EAST, or WEST, indiating the path to this cell for Dijkstra's algorithm

There will be a cavern class with the following:
- 2-D array of points, corresponding to the world. The max size is 100x100
- rows - number of rows in the cavern
- cols - number of columns in the cavern
- run_dijkstra function, which is given a starting location and will calculate the shortest distance to every point
- init_dijkstra function - used to clear the visited, min_risk_to_here, and neighbor_direction values for all points
- find_next_to_eval - used for dijkstra's algorithm to find the next point to evaluate
- get_min_distance - is passed a row and column, and will return the previously calculated minimum distance from the Dijkstra algorithm
- expand_input - this function will convert the part 1 parsed input into the part 2 expanded input

### Main processing ###

- Parse the input into the Cavern
- Call the run_dijkstra function, giving 0,0 (the top-left corner) as the starting point
- **OUTPUT** the result of the get_min_distance function for the bottom-right corner

### Part 2 changes ###

The expand_input function will do the following
- Iterate with row_copy from 0 to 4
    - Iterate col_copy from 0 to 4
        - Interate with row from 0 to num_rows
            - Iterate with col from 0 to num_cols
                - Set points[row_copy*rows+row][col-copy*cols+col].risk_level to (points[row][col].risk_level + row + col)
                - If points[row_copy*rows+row][col-copy*cols+col].risk_level >= 10, subract 9 from it to reset the value to the 1-9 range

The main loop will call the expand_input function right after parsing the input.

## Things I learned ##

So much fun implmenting Dijkstra's algorithm.
