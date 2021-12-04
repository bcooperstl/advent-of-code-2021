# Day 3 #

## Part 1 Requirements ##

Calculate the gamma rate and episolon rate from the diagnostic report, as described in the input format.

The gamma rate is composed bit-wise by using the most-common bit in each position.

The epsilon rate is composed similary, but using the lease-common bit in each position.

Output the power consumption (gamma rate \* epsilon rate) as a decimal value.

## Part 2 Requirements ##

Calculate the oxygen generator rating and CO2 scrubber rating from the diagnostic report, as described in the input format.

Each of these is calculated by a similar, but different, iterative process, starting with the first bit and then progressing.

For the oxygen generator rating, look at the most common value in the current bit position, and only keep numbers matching that value.
If there are equal 1s and 0s, keep the values with a 1.

For the CO2 scrubber rating, look at the least common value in the current bit position, and only keep numbers matching that value.
If there are equal 1s and 0s, keep the values with a 0.

Output the power consumption (oxygen generator rating \* CO2 scrubber rating) as a decimal value.

### Input Format ###

A diagnostic report, consiting of a list of binary numbers, one per line.

### Output Format ###

#### Part 1 ####

Integer of the power consumption as described in the Part 1 Requirements.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day3_test1.txt) provided showing sample case.

### Custom Test Cases ###


## Approach ##

### Part 1 ###

#### Main Program ####

- Parse the input to a vector of *data*
    - Converting each input from binary to a decimal numbers.
    - Also note the number of binary digits in each input and store in *length*
- Initialize an array of *length* items called *ones* to start at 0. This will correspond to the number of one values at each position.
- Loop over each item in *data*
    - Loop over each bit with *i*
        - If the *i*th bit is a 1, increment *ones*[i]
        - If the *i*th bit is a 0, decrement *ones*[i]
- Initialize *gamma* and *epison* to both be 0
- Loop over each bit with *i*
    - If *ones[i]* is positive, one is the most common value and zero is the least common value
        - Set the *i*th bit in *gamma* to 1
        - The *i*th bit in *epsilon* is already 0, so no need to reset it.
    - Else if *ones[i]* is negative, so zero is the most common value and one is the least common value
        - Set the *i*th bit in *epsilon* to 1
        - The *i*th bit in *gamma* is already 0, so no need to reset it.
    - Else
        - Throw an error. Something's wrong or ambiguous
- **Output** the power consumption of *gamma* \* *epsilon*

### Part 2 ###

#### Main Program ####

- Parse the input to a vector of *data*
    - Converting each input from binary to a decimal numbers.
    - Also note the number of binary digits in each input and store in *length*
- Call the Calculate Oxygen Generator Rating function, storing the result in *oxygen*
- Call the Calculate CO2 Scrubber Rating function, storing the result in *carbon*
- **Output** the life support rating of *oxygen* \* *carbon*

#### Calculate Oxygen Generator Rating Function ####
Input:  vector of data, length 

Output: single item that is the oxygen generator rating

- Loop over each bit with *i* until *data* only has one item
    - Create empty vectors *zeros* and *ones*
    - Loop over each item in *data*
        - if the *i*th bit in *item* is a one
            - add it to the *ones* vector
        - else the *ith* bit is a zero
            - add it to the *zeros* vector
    - if there number of elements in *ones* is greater than or equal to the number of elements in *zeros*
        - set *data* to *ones*
    - else
        - set *data* to *zeros*
- Return the only element in *data*

#### Calculate CO2 Scrubber Rating Function ####
Input:  vector of data, length 

Output: single item that is the CO2 scrubber rating

- Loop over each bit with *i* until *data* only has one item
    - Create empty vectors *zeros* and *ones*
    - Loop over each item in *data*
        - if the *i*th bit in *item* is a one
            - add it to the *ones* vector
        - else the *ith* bit is a zero
            - add it to the *zeros* vector
    - if there number of elements in *zeroes* is less than or equal to the number of elements in *ones*
        - set *data* to *zeros*
    - else
        - set *data* to *ones*
- Return the only element in *data*

## Things I learned ##

Excited to remember that strtoul can take 2 as a base to parse the binary values


