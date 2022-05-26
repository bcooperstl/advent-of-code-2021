#ifndef __AOC_DAY_23__
#define __AOC_DAY_23__

#include "aoc_day.h"

#include <vector>
#include <map>

#define ANTHRO_A 'A'
#define ANTHRO_B 'B'
#define ANTHRO_C 'C'
#define ANTHRO_D 'D'
#define OPEN '.'

#define NUM_ANTHROS 8

#define COST_A 1
#define COST_B 10
#define COST_C 100
#define COST_D 1000

#define COL_A 3
#define COL_B 5
#define COL_C 7
#define COL_D 9

#define COL_HALLWAY_LEFT 1
#define COL_HALLWAY_RIGHT 11

#define BOTTOM_ROW 3
#define TOP_ROW 2
#define HALLWAY_ROW 1

namespace Day23
{
    struct Board
    {
        char layout[5][14];
        void display();
        void display(int padding);
    };
    
    struct Position
    {
        int row;
        int col;
        char anthro;
    };
    
    struct Move
    {
        int depth;
        int cost;
        Position positions[NUM_ANTHROS];
        Board board;
        bool is_final();
        bool is_final(int position);
        vector<Move> get_all_possible_moves();
        bool can_move(int position, int to_row, int to_col, int & steps);
        Move * parent;
        
    };
}

using namespace Day23;

class AocDay23 : public AocDay
{
    private:
        Move parse_input(string filename);
    public:
        AocDay23();
        ~AocDay23();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
