#ifndef __AOC_DAY_2__
#define __AOC_DAY_2__

#include "aoc_day.h"

#include <vector>

namespace Day2
{
    enum Operation
    {
        Forward,
        Down,
        Up
    };
    
    struct Command
    {
        Operation operation;
        int amount;
    };
}

using namespace Day2;

class AocDay2 : public AocDay
{
    private:
        vector<Command> parse_input(string filename);
    public:
        AocDay2();
        ~AocDay2();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
