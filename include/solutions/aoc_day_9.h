#ifndef __AOC_DAY_9__
#define __AOC_DAY_9__

#include "aoc_day.h"

#include <vector>

#define MAX_HEIGHTMAP_EDGE 100

class AocDay9 : public AocDay
{
    private:
        void parse_input(string filename, char (&heightmap)[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE], int &num_rows, int & num_cols);
    public:
        AocDay9();
        ~AocDay9();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
