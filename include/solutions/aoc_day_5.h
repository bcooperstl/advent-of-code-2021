#ifndef __AOC_DAY_5__
#define __AOC_DAY_5__

#include "aoc_day.h"

#include <vector>

#define MAP_SIZE 10

namespace Day5
{
    struct LineSegment
    {
        int x1;
        int y1;
        int x2;
        int y2;
        bool is_horizontal_or_vertical();
    };
    
    class Map
    {
        private:
            int m_map[MAP_SIZE][MAP_SIZE];
        public:
            Map();
            ~Map();
            void display();
            void map_segment(LineSegment segment);
            int count_dangerous();
    };
}

using namespace Day5;

class AocDay5 : public AocDay
{
    private:
        void parse_input(string filename, vector<LineSegment> & line_segments);
    public:
        AocDay5();
        ~AocDay5();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
