# Day 14 #

## Part 1 Requirements ##

You are given a polymer as a string and a set of insertion rules.

For each round, loop over the polymer, and apply the appropriate insertion rule to insert a character between each pair.
All insertions happen at the same time, so you don't have to factor the newly inserted element into play.

Perform this insertion 10 times. After that is done, find the elements (character) with the highest and lowest quantities.

Subtract the lowest quantity from the highest quantity, and output that result.

## Part 2 Requirements ##

### Input Format ###

The first line of the linput is the polymer template. It is a string.

Then there is a blank line.

Then there are lines of pair insertion rules of the format `AB -> C`. There will be many of these lines.

### Output Format ###

#### Part 1 ####

A number that is the highest quantity minus the lowest quantity.

#### Part 2 ####


## Test Cases ##

There is [one sample test case](../data/test_cases/day14_test1.txt)

### Custom Test Cases ###

None anticipated.

## Approach ##

At first, I thought I would be using a linked this for this, but after doing some thinking, I realized that I don't care about the string at all. Just the elements in it.

### Input Parsing ###

Parse the input as space separated strings.

The first line will have one element, which will be the polymer.

The third line and any after that will be returned as a list of rules.

### Data Structures ###

Each polymer will be stored in a map<string, int>, with the string corresponding to a two-character sequence, and the integer as the quantity of those sequences.

A *rule* structure will be defined to contain:
- *input* of the two character sequence it defines
- *insertion* of the single character that is inserted
- *outputs[2]* containing the two outputted two-character sequences for when the *insertion* character is added

### Part 1 Main Loop ###

- Parse the input into *initial_polymer_str* and *rules*
- Construct the *current* map from the *initial_polymer_str* string
- Loop 10 times
    - Create a *next* polymer map
    - Loop over each pair in *current* with *pair*
        - For both *pair.output* items
            - If *next* contains the item
                - Increment its count by *pair.quantity*
            - Else
                - Insert it into *next* with *pair.quantity*
    - Copy *next* to *current*
- Declare a map<char, int> *quantities* to hold quantities of each element (character)
- Loop over *current* with *pair*
    - For both characers in *pair.input*
        - If *quantities* contains the character
            - Increment its count by *pair.quantity*
        - Else
            - Insert it into *quantities* with *pair.quantity*
- Find the largest and smallest values in *quantities*
- **Output** largest - smallest

## Things I learned ##

It was interesting thinking through this problem to not see it as a string, but as an alternate way of thinking.
