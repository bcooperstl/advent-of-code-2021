# Day 5 #

## Part 1 Requirements ##

Take the input list of line segments, and only use those which are on horizonal or vertical lines.

From this list, determine the dangerous areas, which are points where at least two lines overlap.

Output the number of points that are in those dangerous areas.

## Part 2 Requirements ##

Take the input list of line segments. These can be horizontal lines, vertical lines, or lines at a 45 degree angles

From this list, determine the dangerous areas, which are points where at least two lines overlap.

Output the number of points that are in those dangerous areas.

### Input Format ###

List of vents given as line segments in the form:

`x1,y1 -> x2,y2`

### Output Format ###

#### Part 1 ####

Integer of the number of dangerous points - those will 2 or more lines overlapping.

#### Part 2 ####

Integer of the number of dangerous points - those will 2 or more lines overlapping.

## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day5_test1.txt) provided showing sample case.

### Custom Test Cases ###


## Approach ##

### Data Structures ###

- Line Segment Structure
    - This will contain x1,y1 and x2,y2 values.
    - Have an is_horizontal_or_vertical method.
- Map structure
    - Have an int 2-d array 1000x1000 in size, with all values starting at 0. This will correspond to the number of lines at each point
        - The first index will be the y-index, and the second index will be the x-index.
    - map_segment method, which will take a line segment and increment all of the points on it
    - count_dangerous method, which will return the count of points with 2 or more lines covering them

### Input Parsing ###

I will use my existing read_as_list_of_split_longs method, giving " ", ",", "-", and ">" all as delimeters.

This will result in 4 longs per line: x1, y1, x2, y2.

### Part 1 ###

- Obtain the list of line segments by parsing the input as described above.
- Initalize the map to be empty
- Loop over the list of line segments
    - If the line segment is a horizontal or vertical line
        - Call the map_segment method on the map
- **Output** the result of the count_dangerous method on the map

### Part 2 ###

This is the same as part 1, only it needs to also include line segments that are at 45-degree angles.
The main code change will be in the map_segment method to handle the 45-degree angles.

- Obtain the list of line segments by parsing the input as described above.
- Initalize the map to be empty
- Loop over the list of line segments
   - Call the map_segment method on the map
- **Output** the result of the count_dangerous method on the map

## Things I learned ##

I remembered that structs can have methods, just like classes.

