# Day 12 #

## Part 1 Requirements ##

You are given a map of a series of caves, with one connection provided per line, as described in the input format.

From these individual connections, you first need to construct the cave system.

There are two types of caves - BIG caves (written as capital letters) and small caves (written as lower-cased letters). There is additionally a start and end cave.
You need to find all of the possible paths through the cave system going from start to end. Small caves may only be visited once, and big caves may be visited multiple times.

After you find all of the distinct paths, output that value.

## Part 2 Requirements ##

### Input Format ###

A series of lines showing the connections between caves.
Each line is of the format: *foo-bar* with *foo* and *bar* being the names of different caves. Name will consist of capital and lower-cased letters only.

### Output Format ###

#### Part 1 ####

A number which is the count of distinct paths from start to end.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There are three sample test cases:
- [test 1](../data/test_cases/day12_test1.txt)
- [test 2](../data/test_cases/day12_test2.txt)
- [test 3](../data/test_cases/day12_test3.txt)


### Custom Test Cases ###

None anticipated to be needed

## Approach ##

### Input Parsing ###

I will use my library function to parse each line as a vector of strings, with the `-` character as the delimiter. This will give me two entries per line for the two caves being connected.

### Data Structures ###

I plan on using polymorphism to represent the four different types of Caves.
- `Cave` is the base type.
- `StartCave` corresponds to the starting cave.
- `EndCave` corresponds to the ending cave.
- `BigCave` corresponds to a big cave, which can be visited multiple times.
- `SmallCave` corresponds to a small cave, which can only be visited once.

A cave will have:
- a name
- a list of connected caves
- is_big, is_small, is_start, and is_end methods to return the type of the cave
- get_neighbors function, which will return the list of neighbors
- an add_neighbor function to add a neighbor to the internal connected caves list

A path will have:
- an array of caves which have been traversed on the given path
- a can_visit_cave method which takes a cave and returns true if the path allows that cave to be visited next or false if not.
- visit method, which takes the next cave to visit.
- is_complete method, which will return true on a complete path (going from start-to-end) or false if not
- a get_current_cave method, which returns the current (last) cave in the array of caves

The main processing will contain a map<string, Cave \*> to map from the cave's name to the object

### Recursive find_paths function ###

The two parameters are *current_path* and *completed_paths*

- If current_path is complete, add it to completed_paths and return
- Call the get_current_cave function on *current_path* and store the result in *current_cave*
- Call the get_neighbors function on *current_cave* and store the result in *neighbors*
- For each *neighbor* in *neighbors*
    - if current_path.can_visit_cave(neighbor) is true
        - Copy current_path to next_path
        - Add neighbor to next_path
        - call find_paths(neighbor)
- return

### Part 1 Main Processing ###

- Parse the input into connections as described in the input parsing section.
- Initialize a blank map<string, Cave \*> *lookups*
- Initialize an empty vector<Cave \*> *caves* to store all of the caves.
- For each connection
    - Check *lookups* for both ends of the connection
    - If the item is not in *lookups*, create it as the appropriate Cave subclass, and add it to *lookups* and *caves*
    - If both caves are big or small (can be one of each)
        - Call the add_neighbor method on each cave to add the other as a neighbor
    - If one cave is start
        - Call the add_neighbor method only on the start cave, since we won't go back to start from the other neighbor
    - If one cave is end
        - Call the add_neighbor method only on the non-end cave, since we will never leave the end cave
- At this point, the cave map is build
- Initialize an empty vector<Path \*> *paths* to store all of the completed paths.
- Create a Path object as *start_path*, and call the *visit* method, passing in the start cave
- Call the find_paths function, passing in *start_path* and *paths*
- Delete all of the Cave objects in *caves*
- **Output** the size of the *paths* array

## Things I learned ##

Had a lot of fun thinking through the data structes and algorithms for this.

Enjoyed using polymorphism again.

Used recursion again.