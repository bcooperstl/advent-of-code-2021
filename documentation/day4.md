# Day 4 #

## Part 1 Requirements ##

Given the set of Bingo cards and the list of drawn numbers, find the board that will win first. 
In this case, Bingos are only horizontal and vertical lines - no diagonals or 4 corners.

When the winning card is found calculate its score based on the sum of all unmarked numbers by the number that gave the bingo.

Output the score of the winning card.

## Part 2 Requirements ##

### Input Format ###

The first line is a comma-sepaparated-list of drawn numbers for the bingo game.
It is followed by a blank line.

There are then one or more bingo cards consisting of 5 lines of 5 numbers, sepearted by spaces, corresponding to the bingo card
There is a blank line between each bingo card, but not one after the final bingo card.

### Output Format ###

#### Part 1 ####

Integer of the score of the winning board as described in the Part 1 Requirements.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day4_test1.txt) provided showing sample case.

### Custom Test Cases ###


## Approach ##

### Part 1 ###

#### Data Structures ####

There will be a "spot" structure used for each value on the bingo card. It will have value and marked properties.

There will be a "card" class used for a Bingo card. It will have:
- a 5x5 array of spots corresponding to the card data
- a mark_value method that will look for a given value and mark that spot, if it exists on the card
- a has_bingo method that will check for if the card has a bingo
- a calculate_score method, taking the last value as an input, which will sum the unmarked spots and then multiply that by the last value to give the score
- a constuctor taking vector<vector<long>> so I can pass in all 5 rows at once

#### Input Parsing ####

I will use my existing read_as_list_of_split_longs method, giving both space and comma as delimiters, so that I can parse everything in one go.

Note - I may have to fix this method - I'm not sure how it handles a delimiter as the first character.

One item being returend will be a vector containing the numbers being called.

I'll create Bingo cards based on lines 2-6, 8-12, etc as needed using the specfific constructor, and return a vector of those as well.

#### Approach ####

- Parse the input as described in the above section, resuling in a list of and a list of Bingo cards
- Loop over the numbers to be called
    - Loop over each card
        - Mark the current number in the card.
        - If this card has a bingo
            - Calculate the card's score and store in *winning_score*.
            - Mark that a bingo is found and break out of the loops
- **Output** the *winning_score* value

## Things I learned ##

Had fun developing the algorithm to work with this.


