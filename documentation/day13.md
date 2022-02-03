# Day 13 #

## Part 1 Requirements ##

You are given a list of dots on a clear sheet of paper and a set of folding instructions as described in the Input Format section.

Plot the given points as dots onto the plane. For each fold, simluate folding the paper in half either up (for y= instruction) or left (for x= instruction).

After folding the paper up or left, the dots may overlap, only counting as one position with a dot.

Only do the first fold in the instructions, and output the number of dots that are visible.

## Part 2 Requirements ##

Continue folding all of the folds.

The solution is determined by reading the final display, which is 8 capital letters.

### Input Format ###

The input is broken up into two sections.

The first section is a list of points given as x,y. Note that the y-coordinate increases as you go downward.

The section section is a list of instructions for how to fold: `fold along y=7`

### Output Format ###

#### Part 1 ####

A number corresponding to the number of dots shown after the first fold.

#### Part 2 ####

Text of 8 capital letters from reading the output. Must be read by a human.

## Test Cases ##

### Given Test Cases ###

There is [one sample test case](../data/test_cases/day13_test1.txt)

### Custom Test Cases ###

None anticipated.

## Approach ##

### Data structures ###

I will use two simple structures:

-A *dot* with *x* and *y* components
-A *fold_instruction* with *direction* and *position* components

### Input Parsing ###

I will parse the input as a list of strings separated by commas, spaces, and the equals sign.

To extract the dot, I can pull the first to components out to store in the x and y values, by converting to an integer.

To extract the fold instructions, I can pull the 3rd and 4th components for the direction and position values. The position will be converted to an integer.

### Displaying a sheet of paper ###

I will use my screen class to display the sheet of paper, since it already can support these operations.

I will determine the maximum x and y values from the list of dots, and use that to set the size of the screen.

### Performing a fold ###

This function will take a set<dot> and a fold, and return the resulting dots in a set<dot>

For a vertical (y=*n*) fold:
    - Any points with a y-component less than *n* will stay the same.
    - Any points with a y-component greater than *n* will keep their x component, and set the new y to n-(y-n) to refelct them.
For a horizontal (x=*n*) fold:
    - Any points with a x-component less than *n* will stay the same.
    - Any points with a x-component greater than *n* will keep their y component, and set the new x to n-(x-n) to refelct them.

### Part 1 Main Loop ###

- Parse the input into a set<dot> *dots* and vector<fold_instructions> *folds*
- Display the sheet of paper with *dots*
- Perform the first fold (*folds[0]*) on *dots*, storing the result in *dots*
- Display the sheet of paper with *dots*
- **Output** the size of *dots*. This is the number of visible dots, since *dots* is a set and therefore duplicate free

### Part 2 Main Loop ###

- Parse the input into a set<dot> *dots* and vector<fold_instructions> *folds*
- Loop over all of the *folds* and perform them, storing the result in *dots* each time
- Display the sheet of paper with *dots*
- **Output** a zero; you have to read the displayed output to understand the real result

## Things I learned ##

Using the c++ set collection to remove duplicates when I perform folds.

