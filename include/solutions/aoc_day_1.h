#ifndef __AOC_DAY_1__
#define __AOC_DAY_1__

#include "aoc_day.h"

#include <vector>

class AocDay1 : public AocDay
{
    private:
        vector<long> read_input(string filename);
        vector<long> create_sliding_windows(vector<long> & measurements);
    public:
        AocDay1();
        ~AocDay1();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
