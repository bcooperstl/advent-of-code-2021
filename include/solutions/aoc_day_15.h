#ifndef __AOC_DAY_15__
#define __AOC_DAY_15__

#include "aoc_day.h"

#include <vector>
#include <map>

#define CAVERN_SIDE_LENGTH 100

#define DIR_NONE 0
#define DIR_NORTH 1
#define DIR_SOUTH 2
#define DIR_EAST 3
#define DIR_WEST 4

namespace Day15
{
    struct Point
    {
        int risk_level;
        bool visited;
        int min_risk_to_here;
        int from_direction;
    };
    
    class Cavern
    {
        private:
            Point m_points[CAVERN_SIDE_LENGTH][CAVERN_SIDE_LENGTH];
            int m_rows;
            int m_cols;
            void init_dijkstra();
            bool find_next_to_eval(int & next_row, int & next_col);
        public:
            Cavern(vector<string> risks);
            ~Cavern();
            int get_rows();
            int get_cols();
            void run_dijkstra(int start_row, int start_col);
            int get_min_distance(int row, int col);
    };
}

using namespace Day15;

class AocDay15 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay15();
        ~AocDay15();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
