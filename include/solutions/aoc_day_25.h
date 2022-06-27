#ifndef __AOC_DAY_25__
#define __AOC_DAY_25__

#include "aoc_day.h"
#include "screen.h"

#include <vector>
#include <set>

#define EMPTY '.'
#define EAST '>'
#define SOUTH 'v'

class AocDay25 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        int move_sea_cucumbers(Screen & from, Screen & to); // returns number moved
    public:
        AocDay25();
        ~AocDay25();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
