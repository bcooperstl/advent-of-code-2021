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

These cases test the splitting function, doing one split action. Note that since all inputa are assumed to be 0-9, these will be given in hexidecimal instead of two-digit numbers.

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

### Data structures ###

I will create a *node* base class, which can have two inherited classes: *pair* and *number*.
A *node* will have:
- a virtual *to_string* method, for dumping a node to a string.
- a virtual *clone* method, for making a deep copy of a node
- a virtual *get_type* method, for returning the node type
- a virtual *get_magnitude* method, for returning the magnitude

A *number* will have:
- a *value* that corresponds to its numberical value.
- a *to_string* method, which outputs the value
- a *clone* method, which creates a new number
- a *get_type* method, which returns NUMBER
- a *get_magnitude* method, which returns the value as the magnitude

A *pair* will have:
- an array of two *members* to correspond to its left and right elements. These will be type *node \** because a pair can have pairs.
- a *depth* value to correspond to the depth of each pair. also include getter and setter
- a *get_string* method that will return a string that corresponds to the 
- a *clone* method, which creates a new pair, and then clones the left and right nodes of this pair to assign tot he new pair assigns the 
- a *get_type* method, which returns PAIR
- a *build_number_list* function that recursively builds a list of the Number values to be used for exploding
- a *find_parent* function that recursively looks for the parent of a given node
- a *find_first_to_explode* function that finds the first cell that should be exploded.
- an *increment_depth* function to recusrively increment the depth of all pairs
- a *get_magnitude* method, which returns the calculated magnitude

### Input parsing ###

I will read each line as a string, and then create a *pair* from the input string.

#### Processing the line ####

Note that by inspection, all input values are in the range 0-9. 

- Create an array of *pair \** *current_pairs* length 5 (will skip 0)
- Create a new *pair* called *base* with depth 1
- Create an array of booleans *set_left* length 5, and set all to false.
- Store *base* in *current_pairs[1]*
- Set *current_depth* to 1
- Set *position* to 1 (skip the first \[ character)
- while *position* < length of input
    - if *input[position]* is a digit
        - Create a *number* from *input[position]*
        - if *set_left[current_depth]* is false
            - Store this *number* at *current_pairs[current_depth].left*
            - set *set_left[current_depth]* to true.
        - else
            - Store this *number* at *current_pairs[current_depth].right*
    - else if *input[position]* is `[`, we are starting a new pair
        - Create a *pair* with depth set to *current_depth + 1*
        - if *set_left[current_depth]* is false
            - Store this *pair* at *current_pairs[current_depth].left*
            - set *set_left[current_depth]* to true.
        - else
            - Store this *pair* at *current_pairs[current_depth].right*
        - increment *current_depth*
        - store this *pair* at *current_pairs[current_depth]*
        - set *set_left[current_depth]* to false
    - else if *input[position]* is `]`, we are starting a closing a pair
        - decrement *current_depth*
    - increment position
- return *base*

### Exploding a pair ###

To identify the pair to explode, do a depth-first search from *base*, looking for a pair where the depth is equal to 5.
If none is found, there are no pairs to explode.

When this pair is found, store it in *target*. From there, we need to find the literal values to the left and right of it, as well as the parent of it.
- Call the *build_number_list* method on the *base* pair, storing the result in *numbers*
- loop over *numbers* with *num*
    - if *numbers[num]* == *target->left* and *num* > 0
        - increment *numbers[num-1]* by *target->left*
    - if *numbers[num*] == *target->right* and *num* < *length(numbers)-1*
        - increment *numbers[num+1]* by *target->right*
- Call the *find_parent_node* for target, storing result in *parent*
- Create a new Number *zero* with value 0
- if *parent->left* is *target*
    - set *parent->left* to *zero*
- else
    - set *parent->right* to *zero*
- delete *target*

### Splitting a pair ###

To identify a pair to split, do a depth-first search from *base* looking for a number where the value is larger than 9.
If none is found, there are no pairs to split.

When this pair, is found, store it in *target*. From there, we need to find the parent.
- Create a new Pair, stored in *split_pair*
- Create a new Number with value *target->value/2* and store it in *split_pair->left*
- Create a new Number with value *(target->value+1)/2* and store it in *split_pair->right*. This takes care of the rounding it up if necessary. Thanks C++
- Call the *find_parent_node* for target, storing result in *parent*
- Set the *split_pair->depth* to *parent->depth + 1*
- if *parent->left* is *target*
    - set *parent->left* to *split_pair*
- else
    - set *parent->right* to *split_pair*
- delete *target*

### Summing two pairs ###

This will sum two pairs *first* and *second*, storing the result in *sum*.
It will also handle the reducing by exploding and splitting.

- Create a new pair *sum*
- Set the *sum->depth* to 1
- Clone *first* and store in *sum->left*
- Clone *second* and store in *sum->right*
- Call the increment_depth methods on both *sum->left* and *sum->right*
- Set *to_explode* equal to *sum->find_first_to_explode* and *to_split* equal to *sum->find_first_to_split*
- While *to_explode* or *to_split* is not equal to null
    - if *to_explode* is not null
        - call the *explode* fucntion on *sum* and *to_explode*
    - else *to_split* must not be null
        - call the *split* function on *sum* and *to_split*
    - Set *to_explode* equal to *sum->find_first_to_explode* and *to_split* equal to *sum->find_first_to_split*
- return *sum*

### Calculating the magnitude ###

This is a simple recusrive function to calculate the magnitude with the formula *3 \* left + 2 \* right*.

### Main program for part 1 ###

- Read in the input file in *inputs*
- Clone the first input pair into *running_sum*
- Loop over the remaining inputs with *i*
    - store *running_sum + i* into *current
    - delete *running_sum*
    - set *running_sum* to *current*
- **OUTPUT** the magnitude of *running_sum* and clean up memory

## Things I learned ##

Doing this with test-driven development to make up all of the unique test cases for each portion.

Doing an interative approach, to make sure each step functions on its own before moving on.
