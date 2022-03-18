#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <climits>

#include "aoc_day_17.h"
#include "file_utils.h"

using namespace std;

AocDay17::AocDay17():AocDay(17)
{
}

AocDay17::~AocDay17()
{
}

void AocDay17::parse_input(string filename, int & target_minimum_x, int & target_maximum_x, int & target_minimum_y, int & target_maximum_y)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    
    // target area: x=20..30, y=-10..-5
    // target area: x 20 30 y -10 -5
    // 0      1     2 3  4  5 6   7
    
    char delimiters[4];
    delimiters[0]=' ';
    delimiters[1]='.';
    delimiters[2]='=';
    delimiters[3]=',';
    
    if (!fileutils.read_as_list_of_split_strings(filename, data, delimiters, 4, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    target_minimum_x = strtol(data[0][3].c_str(), NULL, 10);
    target_maximum_x = strtol(data[0][4].c_str(), NULL, 10);
    target_minimum_y = strtol(data[0][6].c_str(), NULL, 10);
    target_maximum_y = strtol(data[0][7].c_str(), NULL, 10);
    
    return;
}

map<int, int> AocDay17::calculate_x_steps_to_velocities(int target_minimum_x, int target_maximum_x, int & max_steps, int & zero_velocity_min_steps)
{
    map<int, int> steps_to_velocities;
    max_steps = 0;
    zero_velocity_min_steps = INT_MAX;
    for (int start_x_velocity = target_maximum_x; start_x_velocity >= 1; start_x_velocity--)
    {
        cout << "Checking start_x_velocity=" << start_x_velocity << endl;
        int current_x_velocity = start_x_velocity;
        int current_x_position = 0;
        int steps = 0;
        while ((current_x_velocity > 0) && (current_x_position < target_maximum_x))
        {
            steps++;
            current_x_position += current_x_velocity;
            if (current_x_velocity > 0)
            {
                current_x_velocity--;
            }
            else if (current_x_velocity < 0) // this is in the requirements, but I don't see it being used here
            {
                current_x_velocity++;
            }
            cout << " After step " << steps << " x is at " << current_x_position << " with velocity " << current_x_velocity << endl;
            if ((current_x_position >= target_minimum_x) && (current_x_position <= target_maximum_x))
            {
                if (steps_to_velocities.find(steps) == steps_to_velocities.end())
                {
                    steps_to_velocities[steps] = start_x_velocity;
                    cout << "  Adding entry to x map: " << steps << " steps at starting velocity " << start_x_velocity << " hits at position " << current_x_position << endl;
                    if (steps > max_steps)
                    {
                        max_steps = steps;
                    }
                    if (current_x_velocity == 0)
                    {
                        if (steps < zero_velocity_min_steps)
                        {
                            cout << "  Setting zero_velocity_min_steps to " << steps << endl;
                            zero_velocity_min_steps = steps;
                        };
                    }
                    
                }
            }
        }
    }
    return steps_to_velocities;
}

bool AocDay17::check_y_velocity_is_hit(map<int, int> x_steps_to_velocities, int max_steps, int zero_x_velocity_min_steps, int start_y_velocity, int target_minimum_y, int target_maximum_y, bool & overshoot, int & peak_height)
{
    peak_height = 0;
    overshoot = false;
    int steps = 0;
    int current_y_velocity = start_y_velocity;
    int current_y_position = 0;
    while ((current_y_velocity >= 0) || (current_y_position > target_minimum_y))
    {
        steps++;
        current_y_position += current_y_velocity;
        current_y_velocity--;
        if (current_y_position > peak_height)
        {
            peak_height = current_y_position;
        }
        if ((current_y_position >= target_minimum_y) && (current_y_position <= target_maximum_y))
        {
            if ((steps > zero_x_velocity_min_steps) || (x_steps_to_velocities.find(steps) != x_steps_to_velocities.end()))
            {
                cout << "Can hit target with starting velocities x=" << x_steps_to_velocities[steps] << " y=" << start_y_velocity << " and peak height " << peak_height << endl;
                return true;
            }
        }
        else if ((current_y_position <= target_minimum_y) && (current_y_velocity < 0))
        {
            return false;
        }
    }
    overshoot = true;
    return false;
}

void AocDay17::calculate_x_range_for_brute_force(int target_minimum_x, int target_maximum_x, int & range_minimum_x, int & range_maximum_x)
{
    range_maximum_x = target_maximum_x;
    range_minimum_x = range_maximum_x;
    cout << "Finding range for brute force with x between " << target_minimum_x << " and " << target_maximum_x << endl;
    cout << "Setting maximum starting velocity to " << range_maximum_x << endl;
    for (int start_x=target_maximum_x; start_x >= 1; start_x--)
    {
        int steps = 0;
        int position = 0;
        int velocity = start_x;
        while (velocity > 0)
        {
            steps++;
            position += velocity;
            velocity --;
            if (position >= target_minimum_x && position <= target_maximum_x)
            {
                cout << "Starting velocity " << start_x << " lands in range at " << position << " after " << steps << " steps" << endl;
                if (start_x < range_minimum_x)
                {
                    range_minimum_x = start_x;
                    cout << " Setting minimum starting velocity to " << range_minimum_x << endl;
                }
            }
        }
    }
}

bool AocDay17::does_hit_target(int start_velocity_x, int start_velocity_y, int target_minimum_x, int target_maximum_x, int target_minimum_y, int target_maximum_y)
{
    int velocity_x = start_velocity_x;
    int velocity_y = start_velocity_y;
    int position_x = 0;
    int position_y = 0;
    
    while (position_x <= target_maximum_x)
    {
        position_x += velocity_x;
        position_y += velocity_y;
        velocity_y--;
        if (velocity_x > 0)
        {
            velocity_x--;
        }
        if (position_x >= target_minimum_x && position_x <= target_maximum_x && position_y >= target_minimum_y && position_y <= target_maximum_y)
        {
            cout << "Hit with starting velocity " << start_velocity_x << "," << start_velocity_y << " by landing at " << position_x << "," << position_y << endl;
            return true;
        }
        if ((position_y < target_minimum_y) && (velocity_y < 0)) // below the target and going down
        {
            return false;
        }
    }
    return false;
}

string AocDay17::part1(string filename, vector<string> extra_args)
{
    int target_minimum_x, target_maximum_x, target_minimum_y, target_maximum_y;
    parse_input(filename, target_minimum_x, target_maximum_x, target_minimum_y, target_maximum_y);
    
    cout << "Target area is from " << target_minimum_x << " <= x <= " << target_maximum_x
         << " and " << target_minimum_y << " <= y <= " << target_maximum_y << endl;

    int max_steps, zero_x_velocity_min_steps;
    map<int, int> x_steps_to_velocities = calculate_x_steps_to_velocities(target_minimum_x, target_maximum_x, max_steps, zero_x_velocity_min_steps);

    int max_peak_height = 0;
    bool overshoot = false;
    int start_y_velocity = target_minimum_y;
    while (start_y_velocity <= -1*target_minimum_y)
    {
        int peak_height;
        cout << "Checking start_y_velocity=" << start_y_velocity << endl;
        bool is_hit = check_y_velocity_is_hit(x_steps_to_velocities, max_steps, zero_x_velocity_min_steps, start_y_velocity, target_minimum_y, target_maximum_y, overshoot, peak_height);
        if (is_hit)
        {
            cout << " hit found" << endl;
            if (peak_height > max_peak_height)
            {
                cout << "  new max peak height of " << peak_height << endl;
                max_peak_height = peak_height;
            }
        }
        start_y_velocity++;
    }
    
    ostringstream out;
    out << max_peak_height;
    return out.str();
}

string AocDay17::part2(string filename, vector<string> extra_args)
{
    int target_minimum_x, target_maximum_x, target_minimum_y, target_maximum_y;
    int range_minimum_x, range_maximum_x, range_minimum_y, range_maximum_y;
    int range_minimum_steps = 1; // can get there in 1 step many ways
    int range_maximum_steps;
    parse_input(filename, target_minimum_x, target_maximum_x, target_minimum_y, target_maximum_y);
    
    cout << "Target area is from " << target_minimum_x << " <= x <= " << target_maximum_x
         << " and " << target_minimum_y << " <= y <= " << target_maximum_y << endl;
    
    calculate_x_range_for_brute_force(target_minimum_x, target_maximum_x, range_minimum_x, range_maximum_x);
    
    cout << "For brute force, use X values between " << range_minimum_x << " and " << range_maximum_x << endl;
    
    range_minimum_y = target_minimum_y;
    range_maximum_y = -1*target_minimum_y;
    
    cout << "For brute force, use Y values between " << range_minimum_y << " and " << range_maximum_y << endl;
    
    int num_found = 0;
    for (int x=range_minimum_x; x<=range_maximum_x; x++)
    {
        for (int y=range_minimum_y; y<=range_maximum_y; y++)
        {
            if (does_hit_target(x, y, target_minimum_x, target_maximum_x, target_minimum_y, target_maximum_y))
            {
                num_found++;
            }
        }
    }
    
    ostringstream out;
    out << num_found;
    return out.str();

}