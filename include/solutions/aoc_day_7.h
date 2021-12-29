#ifndef __AOC_DAY_7__
#define __AOC_DAY_7__

#include "aoc_day.h"

#include <vector>

class AocDay7 : public AocDay
{
    private:
        vector<long> read_input(string filename);
        long calculate_part2_fuel(long optimal, long starting);
    public:
        AocDay7();
        ~AocDay7();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
