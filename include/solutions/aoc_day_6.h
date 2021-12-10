#ifndef __AOC_DAY_6__
#define __AOC_DAY_6__

#include "aoc_day.h"

#include <vector>

class AocDay6 : public AocDay
{
    private:
        vector<long> read_input(string filename);
    public:
        AocDay6();
        ~AocDay6();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
