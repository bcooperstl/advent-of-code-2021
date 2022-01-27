#ifndef __AOC_DAY_11__
#define __AOC_DAY_11__

#include "aoc_day.h"

#include <vector>
#include <map>

#define SIDE_LENGTH 10
#define FLASH_THRESHOLD 9
#define ROW 0
#define COL 1

namespace Day11
{
    class Grid
    {
        private:
            char m_energy[SIDE_LENGTH][SIDE_LENGTH];
        public:
            Grid(vector<string> energy);
            ~Grid();
            int run_step();
            long run_steps(int num_steps);
            void dump();
    };
}

using namespace Day11;

class AocDay11 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay11();
        ~AocDay11();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
