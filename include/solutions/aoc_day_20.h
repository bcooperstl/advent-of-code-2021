#ifndef __AOC_DAY_20__
#define __AOC_DAY_20__

#include "aoc_day.h"

#include <vector>
#include <set>

#include "screen.h"

#define PIXEL_DARK '.'
#define PIXEL_LIGHT  '#'

class AocDay20 : public AocDay
{
    private:
        void parse_input(string filename, char * enhancement_algorithm, Screen & image);
        void process_enhancement(char * enhancement_algorithm, Screen & image);
    public:
        AocDay20();
        ~AocDay20();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
