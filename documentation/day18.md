# Day 18 #

## Part 1 Requirements ##

A pair is written as `[x,y]` where *x* and *y* can be either a number or another pair.

To add two pairs together, you form a new pair, so that `left + right = [left,right]`. 

After two pairs are added together, they may need to be reduced. Reduction has two steps:
- If any pair is nested inside four pairs, the leftmost such pair *explodes*.
- If any regular number is 10 or more, the leftmost one of them *splits*.

Explosion always happens first, and is checked everytime before a split may occur.

To explode a pair:
- The left value in the pair is added to the first regular number to the left of it, or none if it is the leftmost regular number.
- The right value in the pair is added to the first regular number to the right of it, or none if it is the rightmost regular number.
- The pair is replaced with the regular number 0.

The split a regular number:
- Replace it with a pair
    - The left value is the number divided by 2 and rounded down.
    - The right value is the number divided by 2 and rounded up.

We are to add up a list of pairs. Each pair is listed on a line, and we add from top to bottom.

From the final pair, we are to calculate its magnitude by the following:
- The magnitude of a regular number is itself.
- The magnitude of a pair is 3 times the magnitude of the left element + 2 times the magnitude of the right element, calculated recursively, if needed.


## Part 2 Requirements ##

### Input Format ###

The input will be a list of pairs, one per line.

### Output Format ###

#### Part 1 ####

Number - the magnitude of the sum of the pairs.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

I will be using a bunch of test-driven development, and will have so so so many test cases

#### Parsing ####

To ensure the parsing to the data strutures works successfully, I will work with the 7 provided examples.

[1](../data/test_cases/day18_parse1.txt), 
[2](../data/test_cases/day18_parse2.txt), 
[3](../data/test_cases/day18_parse3.txt), 
[4](../data/test_cases/day18_parse4.txt), 
[5](../data/test_cases/day18_parse5.txt), 
[6](../data/test_cases/day18_parse6.txt), 
[7](../data/test_cases/day18_parse7.txt)

#### Exploding ####

These cases test the exploding function, doing one explode action.

[1](../data/test_cases/day18_explode1.txt), 
[2](../data/test_cases/day18_explode2.txt), 
[3](../data/test_cases/day18_explode3.txt), 
[4](../data/test_cases/day18_explode4.txt), 
[5](../data/test_cases/day18_explode5.txt)

#### Splitting ####

These cases test the splitting function, doing one split action.

[1](../data/test_cases/day18_split1.txt), 
[2](../data/test_cases/day18_split2.txt), 
[3](../data/test_cases/day18_split3.txt)

#### Summing ####

These test cases sum the list of numbers and outputs the final pair.

[1](../data/test_cases/day18_sum1.txt), 
[2](../data/test_cases/day18_sum2.txt), 
[3](../data/test_cases/day18_sum3.txt), 
[4](../data/test_cases/day18_sum4.txt), 
[5](../data/test_cases/day18_sum5.txt),


#### Magnitude ####

These test cases perform the magnitude of the one pair.

[1](../data/test_cases/day18_mag1.txt), 
[2](../data/test_cases/day18_mag2.txt), 
[3](../data/test_cases/day18_mag3.txt), 
[4](../data/test_cases/day18_mag4.txt), 
[5](../data/test_cases/day18_mag5.txt), 
[6](../data/test_cases/day18_mag6.txt), 
[7](../data/test_cases/day18_mag7.txt), 
[8](../data/test_cases/day18_mag8.txt), 
[9](../data/test_cases/day18_mag9.txt)

#### Complete Test ####

There is one test case that does a [complete test](../data/test_cases/day18_test1.txt)

### Custom Test Cases ###


## Approach ##


## Things I learned ##


