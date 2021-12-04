#ifndef __AOC_DAY_3__
#define __AOC_DAY_3__

#include "aoc_day.h"

#include <vector>

class AocDay3 : public AocDay
{
    private:
        vector<long> read_input(string filename, int & length);
        long calculate_oxygen_generator_rating(vector<long> data, int length);
        long calculate_co2_scrubber_rating(vector<long> data, int length);
    public:
        AocDay3();
        ~AocDay3();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
