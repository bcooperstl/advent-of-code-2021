# Day 11 #

## Part 1 Requirements ##

The input consists of a 2-D array of energy levels of the octopuses, ranging from 0 to 9.

Process over the array with the following series of steps:
- Increment the energy level of each octopus by 1
- For each octopus that has an energy level greater than 9
    - That octopus flashes.
    - All adjacent (up to 8) octopuses' energy levels increase by 1
        - Note that this may then cause those octopuses to flash and increment their neighbors, etc.
        - An octopus may only flash once per step
- Any octopus that flashed during this step has its energy reset to 0

Perform this procedure on the input for 100 steps.

Output the total number of flashes across the 100 steps.

## Part 2 Requirements ##

### Input Format ###

The input is 10 rows by 10 columns of single-digit integers (values 0-9). There is no separation between integers within a row.

### Output Format ###

#### Part 1 ####

A number corresponding to the total number of flashes throughout the 100 steps.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day11_test1.txt) provided showing sample case.

### Custom Test Cases ###

None needed.

## Approach ##

### Data Structures ###

I will create a grid class to store the octopuses. It will have:
- a 10x10 array of the *energy* values
- a run_step method that runs a single step. It will return the number of flashes that occurs during this step
- a run_steps method that takes a number of steps to run. It will return the number of flashes that occurs for all the steps.
- a constructor that takes a vector<string> as described in the Input Format and creates the energy values from it.

### run_step method ###

- Declare a *flash_queue[100][2]* to contain the items that will flash.
- Set *insert_pos* to 0
- Loop over each cell in *energy* with *row* and *col*
    - Increment *energy[row][col]* by 1.
    - If the new energy is > 9
        - Set *flash_queue[insert_pos]* to be *row* and *col*
        - Increment *insert_pos*
- Declare *work_pos* to 0
- While *work_pos* < *insert_pos* 
    - Increment by 1 the energy values for all (up to 8) neighbors of *flash_queue[work_pos]*
    - If the value of those neighbors > 9 and they are not already in *flash_queue*
        - Set *flash_queue[insert_pos]* to be the neighbor's *row* and *col*
        - Increment *insert_pos*
        - By incrementing insert_pos, the while loop will process this new item at the end.
- Return *insert_pos*, which equals the number of items that flashed.

### run_steps method ###

This simple method will call run_step repeatedly for the number of steps. It will sum the return values and return that sum.

### Part 1 main loop ###

- Parse the input into an array of strings
- Construct a grid based on the input
- **Output** the result of the grid's run_steps method called with 100 steps.

## Things I learned ##

Had fun designing the algorithm for this problem.
