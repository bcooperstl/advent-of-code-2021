# Day 10 #

## Part 1 Requirements ##

You are given a list of strings as described in the input format. Each string has one or more chunks, with other chunks able to be nested in the middle.

Some strings are incomplete, while others are corrupted. We need to identify the corrupted lines - those where the closing symbols do not match the corresponding opening symbols.

Discard the incomplete lines.

For each corrupted line, identify the first illegal (i.e. mismatched) character. Assign it a score as follows:
- ')' is worth 3
- ']' is worth 57
- '}' is worth 1197
- '>' is worth 25137

Output the sum of the scores for all of the corrupted lines in the file.

## Part 2 Requirements ##

### Input Format ###

A list of strings, one per line, consisting of combinations of the following open/close pairs of symbols:
- '(' and ')'
- '\[' and '\]'
- '{' and '}'
- '<' and '>'


### Output Format ###

#### Part 1 ####

A number corresponding to the sum of the scores as described in the requirements.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day10_test1.txt) provided showing sample case.

### Custom Test Cases ###

None needed.

## Approach ##

### Part 1 ###

#### Helper functions ####

isOpeningChunkChar(char ch) - returns true if ch is '(', '\[', '{', or '<' or false otherwise.

isClosingChunkChar(char ch) - return true if ch is ')', '\]', '}', or '<' or false otherwise.

isMatchingChunkChars(char open, char close) - returns true if the opening/closing pair matches or false otherwise.

#### Check Corrupted function ####

This function will take a string, and check if it is corrupted. It will return true if corrupted, or false if not-corrupted.

Additionally, it will return back the character that makes the string corrupted.

This will use an array as a stack to store the opening characters for the chunk

- Initialze a `stack_pos` to point to -1 to show there aren't any elements in the `stack` array.
- Loop over the input string with i
    - If `input[i]` is an opening character
        - Increment `stack_pos` and set `stack[stack_pos]` to `input[i]`
    - If `input[i]` is an closing character
        - If `stack_pos` is set to -1, there aren't any elements on the stack.
            - Set `invalid_char` to `input[i]` and return true
        - Else, there are elements on the stack
            - If `input[i]` is the closing char that matches `stack[stack_pos]` this is a valid match
                - Decrement `stack_pos`
            - Else, there makes a corrupted input
                - Set `invalid_char` to `input[i]` and return true
- Return false - there was no corrupted on the line; it's just incomplete

#### Main Processing ####

- Parse the input file as a list of strings as `lines`.
- Initialize a `sum_score` to 0.
- For each `line` in `lines`
    - Call the check_corrupted function, storing the result in `is_corrupted` and `invalid_char`
    - If `is_corrupted` is true
        - Use a lookup map to increment `sum_score` based on the `invalid_char` value
- **Output** the *sum_score* value

## Things I learned ##

Fun to write the algorithm. Annoyed that > is not simple 1 more than < in ASCII, so I have to do actual comparisons instead of just looking for the closing value being one higher than the opening value.

