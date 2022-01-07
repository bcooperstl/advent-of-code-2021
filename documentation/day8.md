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

Continuing from part 1, we need to identify all of the output digits for each display.

Then, compute the 4-digit output value from each display.

Add up all of the computed values and return the sum of all those values.

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

Integer - the sum of the 4-digit output values

## Test Cases ##

### Given Test Cases ###

There is one given [test case](../data/test_cases/day8_test1.txt).

### Custom Test Cases ###

I made a [custom test case](../data/test_cases/day8_test2.txt) containing just the first item to make testing easier for the logic.

## Approach ##

### Some Logic ###

I am going to refer to the correctly-located segments of the display with capital letters as ABCDEFG.
I am going to refer to the given scrambled segments with the lower-cased letters abcdefg as provided in the test files.

### Data Structure Design ###

The `display` structure will have:

    - a list of the ten patterns values called `patterns`
    - a list of the four output values called `outputs`
    - a map <char, char> `segment_matches` that will map from the random (a-g) segment to the correct (A-G) segment and vice-versa, abusing that I use both capital and lower-cased letters
    - a list strings `values` that will contain the pattern value for the corresponding digit. This will start as blank strings and get populated as they get identified.
    - a map <string, int> `digits` that maps a pattern to its numberical digit value.
    - a `process_patterns` function that will handle the matching. Logic will be described below.
    - a `get_part1_count` function that will return the count of 1, 4, 7, or 8 values in the `outputs`.
    - a `get_output_value` function that will return the 4-digit output value.
    
### Input Parsing ###

- I will use my existing read_as_list_of_split_longs method, giving " " and "|" as delimeters.
- This will result in 14 items per line - the first 10 are the patterns, and the last 4 are the outputs.
- When populating a `display` structure, sort each of the patterns and the outputs alphabetically, so that there aren't issues with other comparisons.
- Return the list of display structures.

### process_patterns function ###

#### Part 1 ####

For part 1, all we need to do are identify the 1, 4, 7, and 8 values.

- loop over the 10 `patterns` with `i`
    - if `patterns[i]` is length 2
        - This is a 1. Set `values[1]` to `patterns[i]`
    - if `patterns[i]` is length 3
        - This is a 7. Set `values[7]` to `patterns[i]`
    - if `patterns[i]` is length 4
        - This is a 4. Set `values[4]` to `patterns[i]`
    - if `patterns[i]` is length 7
        - This is a 8. Set `values[8]` to `patterns[i]`

#### Part 2 ####

For part 2, we need to build up all 10 string-to-digit mappings for part 2

- initialize two empty lists of `five_patterns` and `six_patterns` to hold 
- loop over the 10 `patterns` with `i`
    - if `patterns[i]` is length 2
        - This is a 1. Set `values[1]` to `patterns[i]` and `digits[patterns[i]]` to `1`
    - if `patterns[i]` is length 3
        - This is a 7. Set `values[7]` to `patterns[i]` and `digits[patterns[i]]` to `7`
    - if `patterns[i]` is length 4
        - This is a 4. Set `values[4]` to `patterns[i]` and `digits[patterns[i]]` to `4`
    - if `patterns[i]` is length 5
        - Add `patterns[i]` to `five_patterns`
    - if `patterns[i]` is length 6
        - Add `patterns[i]` to `six_patterns`
    - if `patterns[i]` is length 7
        - This is a 8. Set `values[8]` to `patterns[i]` and `digits[patterns[i]]` to `8`
- Identify segment A by finding the segment in `values[7]` that is not in `values[1]`. 
Store this bidirectionally in `segment_matches`
- Identify the pattern for 6 by looking at the three options in `patterns_sixes`. 
The one that does not have both of the segments of `values[1]`.
Set `values[6]` to this item and the coresponding entry in digits
- Find the segment in `values[1]` that is also in `values[6]`. This is the segment F.
Store this bidirectionally in `segment_matches`.
- Find the segment in `values[1]` that is not in `values[6]`. This is the segment C.
Store this bidirectionally in `segment_matches`.
- Using the knowledge of segments C and F, loop over the three items in `five_patterns`.
    - If the item has both `pattern_matches[C]` and `pattern_maches[F]`, then it is 3. Set up `values` and `digits` accordingly.
    - If the item has only `pattern_matches[C]`, then it is 2. Set up `values` and `digits` accordingly.
    - If the item has only `pattern_matches[F]`, then it is 5. Set up `values` and `digits` accordingly.
- create a list `dg_possibilities` containing the elements in `values[3]` that are not matched to A,C, or F.
- Looking at `values[2]`, the segment that is not A, or C, and is not in dg_possibilities, maps to E.
Store this bidirectionally in `segment_matches`.
- Looking at `values[5]`, the segment that is not A, or F, and is not in dg_possibilities, maps to B.
Store this bidirectionally in `segment_matches`.
- Loop through six_patterns to find the element that only contains one of dg_possibilities.
    - This is 0. Set up `values` and `digits` accordingly.
    - The one of the two matched in dg_possibilities maps to G.
Store this bidirectionally in `segment_matches`.
    - The other of the two in dg_possibilities maps to D.
Store this bidirectionally in `segment_matches`.
- Loop through the six_patterns to find the element that isn't matched to anything in `digits`.
    - This is 9. Set up `values` and `digits` accordingly.

### Main Processing ###

#### Part 1 ####

- Process the input into a list of `displays`
- Initialize a `counter` value to 0
- For each item in `displays`:
    - Call the `process_patterns` function on that item
    - Increment `counter` by the result of the `get_part1_count` function
- **Output** the final `counter` value

#### Part 2 ####

- Process the input into a list of `displays`
- Initialize a `output_sum` value to 0
- For each item in `displays`:
    - Call the `process_patterns` function on that item
    - Increment `output_sum` by the result of the `get_output_value` function
- **Output** the final `output_sum` value


## Things I learned ##

Using std::sort on a string as an easy way to sort the input.

Developing a unique algorithm for this.

