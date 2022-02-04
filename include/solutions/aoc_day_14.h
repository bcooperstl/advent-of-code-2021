#ifndef __AOC_DAY_14__
#define __AOC_DAY_14__

#include "aoc_day.h"

#include <vector>

namespace Day14
{
    struct Rule
    {
        string input;
        char insertion;
        string output[2];
    };
}

using namespace Day14;

class AocDay14 : public AocDay
{
    private:
        void parse_input(string filename, string & polymer, map<string, Rule> & rules);
    public:
        AocDay14();
        ~AocDay14();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
