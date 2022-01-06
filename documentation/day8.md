# Day 8 #

## Part 1 Requirements ##

We are given a four-digit, seven-segment display, with the segments labeled as:
````
 aaaa
b    c
b    c
 dddd
e    f
e    f
 gggg
````

However, the signals controlling each of the segments have been mixed up, but for each input, they are all mixed up the same way.

Process the input file as described in the input format - with the ten unique patterns and the four output values.

For part 1, we are going to identify the easy-to-find output values:
    - 1 has two segments
    - 7 has three segments
    - 4 has four segments
    - 8 has all seven segments.

Given the input, identify the easy values and count the total number of times they appear in the outputs.

## Part 2 Requirements ##

### Input Format ###

The input consists of two parts, separated by a `|` character. 
The first part is ten strings separated by spaces; one for each of the ten output patterns observerd.
The second part is four strings separated by spaes corresponding to a four-digit output value.

Example:

`acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf`


### Output Format ###

#### Part 1 ####

Integer - the number of times the digits 1, 4, 7, or 8 appear in the output.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is one given [test case](../data/test_cases/day7_test8.txt).

### Custom Test Cases ###


## Approach ##

### Some Logic ###

I am going to refer to the correctly-located segments of the display with capital letters as ABCDEFG.
I am going to refer to the given scrambled segments with the lower-cased letters abcdefg as provided in the test files.

Within a test case, there will be several maps.


### Data Structure Design ###

The `display` structure will have:

    - a list of the ten patterns values called `patterns`
    - a list of the four output values called `outputs`
    - a map <char, char> `segment_matches` that will map from the random (a-g) segment to the correct (A-G) segment and vice-versa, abusing that I use both capital and lower-cased letters
    - a list strings `values` that will contain the pattern value for the corresponding digit. This will start as blank strings and get populated as they get identified.
    - a `process_patterns` function that will handle the matching. Logic will be described below.
    - a `get_part1_count` function that will return the count of 1, 4, 7, or 8 values in the `outputs`.
    
### Input Parsing ###

- I will use my existing read_as_list_of_split_longs method, giving " " and "|" as delimeters.
- This will result in 14 items per line - the first 10 are the patterns, and the last 4 are the outputs.
- When populating a `display` structure, sort each of the patterns and the outputs alphabetically, so that there aren't issues with other comparisons.
- Return the list of display structures.

### process_patterns function ###

#### Part 1 ####

For part 1, all we need to do are identify the 1, 4, 7, and 8 values.

- loop over the 10 `parterns` with `i`
    - if `patterns[i]` is length 2
        - This is a 1. Set `values[1]` to `patterns[i]`
    - if `patterns[i]` is length 3
        - This is a 7. Set `values[7]` to `patterns[i]`
    - if `patterns[i]` is length 4
        - This is a 4. Set `values[4]` to `patterns[i]`
    - if `patterns[i]` is length 7
        - This is a 8. Set `values[8]` to `patterns[i]`


### Main Processing ###

#### Part 1 ####

- Process the input into a list of `displays`
- Initialize a `counter` value to 0
- For each item in `displays`:
    - Call the `process_patterns` function on that item
    - Increment `counter` by the result of the `get_part1_count` function
- **Output** the final `counter` value


## Things I learned ##

Using std::sort on a string as an easy way to sort the input.

Developing a unique algorithm for this.

