# Day 1 #

## Part 1 Requirements ##

You are given a group of scanners. For each scanner, there is a collection of beacons that it detects.

A scanner can detect a beacon within 1000 units in the x, y, and z directions. 
However, a scanner does not know which way it is oriented, so the x, y, and z coordinates aren't consistant between scanners. 
Scanners will be facing a value along an axis, rotated anyway around it to line up for 90-degree angles.

The 3D region can be reconstructed by combining beacons detected by multiple scanners. 
There need to be at least 12 common beacons between scanners for the overlap to match.

Given the list of beacons for each scanner, determine the layout of all the scanners and beacons in the 3D region.

After this is determined, output the total number of beacons in the region.



## Part 2 Requirements ##

### Input Format ###

A series of sections of lines, separated by blank lines.
The first line in each setion gives the scanner number.
Any subsequent lines in the section give the location of a beacon in x,y,z coordinates.

### Output Format ###

#### Part 1 ####

Number - the count of distinct beacons in the 3D space.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one given test case](../data/test_cases/day19_test1.txt)

### Custom Test Cases ###

I created [one simplified test case](../data/test_cases/day19_test2.txt) containing only the first two scanners from the given test case.


## Approach ##

I'm going to doing this iteratively.

### Data Structures ###

There will be a coordincates struct that will contain x, y, and z values

There will be a ObservedBeacon class which will contain:
- The x,y,z cooredinates reported by the scanner.
- Pointer to the Scanner class which contains it.

There will be an ActualBeacon class which will contains:
- The x,y,z coordincates in actual space
- A list of pointers to ObservedBeacons that correspond to the ActualBeacon.

There will be a Scanner class which will contain:
- A number corresponding to the number in the input file.
- Observed location, which will be 0,0,0 for all scanners.
- Actual location, which will be determined.
- A list of ObservedBeacons, containing the locations relative to the Observed location of all the beacons

There will be a Region class which will contain:
- A list of Scanners
- A list of ActualBeacons
- A method to set Scanner 0 to center of region

### Input Parsing ###

- Split into a list of list of strings with spaces and commas a delimiters.
- Declare a *currentScanner* as null
- Loop over the outer list of lines
    - If the line has 4 elements, it is the scanner line
        - Create a new Scanner. Store in *currentScanner* and add it to the *scanners* list to be returned
    - If the line has 3 elements, it is a beacon line
        - Create a new ObservedBeacon and add it to the *currentScanner*'s list
    - If the line is blank, skip it
- Return the list of Scanners
    
### Setting scanner 0 to center of region ###

We will delcare that scanner 0 is at the origin and its beacons are relative to the origin.

- on Scanner 0, set the actual location to be the same as observed, that is 0,0,0
- for each of the scanner 0 observed beacons:
    - create an absolute beacon with its same location
    - add the observed beacon as one that corresponds to the actual beacon
    - add this absolute beacon to the region

### Identifying overlapping regions between scanners ###

The first step to identify overlapping regions is to find pairs of points with the same distances apart across 3 dimensions.

The largest scanner has 27 beacons, which means 351 pairs of points.

We will store the distances between points in two different ways. The first way is what I am calling a simple distance - bascially a formula.
If we have pairs of beacons from different scanners with the same simple distance apart, we can then compare the actual distances in the x, y, and z, components.




### Main Loop ###
- Parse the input file into a list of Scanners, and assign those to the Region

- **Output** the size of the region's actual beacons list

## Things I learned ##


