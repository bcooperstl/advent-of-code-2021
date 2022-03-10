# Day 17 #

## Part 1 Requirements ##

You are given a square region that you are trying to hit with a projectile. The region will be below you (negative x direction) and away in the positive x direction.

You will launch the probe from 0,0 with initial x and y velocities. For each step:
- the X position increases by the X velocity
- The Y position increases by the Y velocity
- The X velocity moves by 1 towards 0
- The Y velocity decreases by 1

The probe must land in the target area on a step; not pass through it.

We are to find the trajectory that will send the probe the highest and still have it land in the target area.

Output the highest point the probe will reach.

## Part 2 Requirements ##

### Input Format ###

One line of the format:

    target area: x=20..30, y=-10..-5

### Output Format ###

#### Part 1 ####

Number corresponding to the highest point the probe will reach.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There is [one example](../data/test_cases/day17_test1.txt) for part 1.

### Custom Test Cases ###

None anticipated.

## Approach ##

### Overview ###

For part 1, I will plan on creating a map for the number of steps to the starting X velocity that will land me in the region in those number of steps.
I will start with an X velocity set to the maximum X value of the target area, and work my way down to an X value of 1.

Then, I will start with Y velocity equal to the minimum Y value of the target area, and see if it is possible to reach the target area with that value.
If it is possible, I will look for an X velocity that matches that number of steps. If this combination does exist, I'll compare the maximum height to see if it is the new highest height.
I'll increase this by 1 each time, until I get to a point where after the initial Y Velocity does not allow the Y position to be at or below the minimum Y value by the time maximum number of steps in my X-lookup table.

### Map steps to X velocities ###

- Initialize a *steps_to_velocities* map
- Loop from *target_maximum_x* down to 1 with *start_x_velocity*
    - set *current_x_velocity* to *start_x_velocity*
    - set *current_x_position* to 0
    - set *steps* to 0
    - while *current_x_velocity* > 0 and *current_x_position* < *target_maximum_x*
        - increment *steps* by 1
        - increment *current_x_position* by *current_x_velocity*
        - decrement *current_x_velocity* by 1
        - if *currrent_x_position* between *target_minimum_x* and *target_maximum_x* we have a starting velocity that will lead to the target area in *steps* steps
            - if *steps_to_velocities* map does not have an entry for *steps*
                - map *steps* to *start_x_velocity* in the *steps_to_velocities* map.
- return the *steps_to_velocities* map

### Check if Y velocity results in a hit ###

- takes *x_steps_to_velocity*, *max_steps*, *start_y_velocity*, *target_minimum_y* and *target_maximum_y* as inputs
- returns true/false for it is hits the target, a true/false overshoot boolean, and peak_height for the highest point
- set *peak_height* to 0
- set *steps* to 0
- set *current_y_velocity* to *start_y_velocity*
- set *current_y_position* to 0
- while *steps* <= *max_steps*
    - increment *steps* by 1
    - increment *current_y_position* by *current_y_velocity*
    - decrement *current_y_velocity* by 1
    - if *current_y_position* > *peak_height*
        - set *peak_height* to *current_y_position*
    - if *current_y_position* between *minimum_y_position* and *maximum_y_position*
        - if *x_steps_to_velocity* has a value for *steps* steps, we have a hit
            - set false for overshoot, and return true for hitting the target
    - else if *current_y_position* < *minimum_y_position* and *current_y_velocity* < 0, we are below the target area and going down
        - set false for overshoot and return false for hitting the target
- at this point, we are above the target and have overshot it
- set true for overshoot and return false for hitting the target

### Main Loop ###
- Read in the input file and parse into *target_minimum_x*, *target_maximum_x*, *target_minimum_y*, and *target_maximum_y*
- map the steps to x velociites and store in *x_steps_to_velocities*
- find the maximum key in *x_steps_to_velocities* and store in *max_steps*
- set *max_peak_height* to 0
- set *overshoot* to false
- set *start_y_velocity* to *target_minimum_y(
- while *overshoot* is false
    - call *check_y_velocity_hits* storing results in *is_hit*, *overshoot*, and *peak_height*
    - if *is_hit* is true
        - if *peak_height* > *max_peak_height*
            - set *max_peak_height* to *peak_height*
- **OUTPUT** the *max_peak_height* value


## Things I learned ##

Very interesting developing this algorithm. I'm sure I could not brute-force it as much by doing math to figure better limits, but I at least have the upper bounds here.
