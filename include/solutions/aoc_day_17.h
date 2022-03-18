#ifndef __AOC_DAY_17__
#define __AOC_DAY_17__

#include "aoc_day.h"

#include <vector>
#include <map>

class AocDay17 : public AocDay
{
    private:
        void parse_input(string filename, int & target_minimum_x, int & target_maximum_x, int & target_minimum_y, int & target_maximum_y);
        map<int, int> calculate_x_steps_to_velocities(int target_minimum_x, int target_maximum_x, int & max_steps, int & zero_velocity_min_steps);
        bool check_y_velocity_is_hit(map<int, int> x_steps_to_velocities, int max_steps, int zero_x_velocity_min_steps, int start_y_velocity, int target_minimum_y, int target_maximum_y, bool & overshoot, int & peak_height);
        void calculate_x_range_for_brute_force(int target_minimum_x, int target_maximum_x, int & range_minimum_x, int & range_maximum_x);
        bool does_hit_target(int start_velocity_x, int start_velocity_y, int target_minimum_x, int target_maximum_x, int target_minimum_y, int target_maximum_y);
    public:
        AocDay17();
        ~AocDay17();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
