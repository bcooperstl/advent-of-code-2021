# Day 2 #

## Part 1 Requirements ##

A submarine will start at 0,0 and move based on the following commands:
- forward *n*: moves sub forward *n* units
- down *n*: increases the sub's depth by *n* units
- up *n*: decreases the sub's depth by *n* units

Process the list of commands to move the sub to the final position.

Output a value of the final horizontal position multiplied by the depth.

## Part 2 Requirements ##

A submarine will start at 0,0. It will also have an initial aim value of 0.
It will  move based on the following commands:
- down *n*: increases the sub's aim by *n* units
- up *n*: decreases the sub's aim by *n* units
- forward *n*: moves sub forward *n* units and increase the depth by *aim* \* *n* units

Process the list of commands to move the sub to the final position.

Output a value of the final horizontal position multiplied by the depth.

### Input Format ###

A list of commands, one per line, each with two parts.

-part 1 is the action: `forward`, `down`, or `up`.
-part 2 is the amount. 

### Output Format ###

#### Part 1 ####

Integer of the calculated product as described in the Part 1 Requirements.

#### Part 2 ####

Integer of the calculated product as described in the Part 2 Requirements.

## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day1_test1.txt) provided showing sample case.

### Custom Test Cases ###


## Approach ##

### Input parsing ###

There will be a structure created to hold the commands.

The input will be parsed one line at a time, splitting the line into the command name and the value. These will be loaded into the command structure, in order.

A vector of the commands will be returned.

### Part 1 ###

#### Main program ####

- Read in the input and parse it as described above, reseturning a vector of commands
- Initialize the submarines *horizonal position* and *depth* to 0
- Loop over the vector of commands
    - If the command's operation is `forward`
        - Increase the *horizional position* by the command's value
    - If the command's operation is `down`
        - Increase the *depth* by the command's value
    - If the command's operation is `up`
        - Decrease the *depth* by the command's value
- **Output** the *horizonal position* \* *depth*

### Part 2 ###

#### Main program ####

- Read in the input and parse it as described above, reseturning a vector of commands
- Initialize the submarines *horizonal position*, *depth*, and *aim* to 0
- Loop over the vector of commands
    - If the command's operation is `forward`
        - Increase the *horizional position* by the command's value
        - Increase the *depth* by the command's value multiplied by the *aim* value.
    - If the command's operation is `down`
        - Increase the *aim* by the command's value
    - If the command's operation is `up`
        - Decrease the *aim* by the command's value
- **Output** the *horizonal position* \* *depth*

## Things I learned ##

Re-introducing myself to working with c++ namespaces in my Advent Of Code projects so I don't have to worry about name conflicts.

