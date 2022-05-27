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

#define HALLWAY_ROW 1
#define TOP_ROW 2
#define BOTTOM_ROW 3
#define NUM_SMALL_ROWS 5

#define FINAL_SMALL_REP "...........ABCDABCD"

#define ROW_WIDTH 13

namespace Day23
{
    class SmallBoard
    {
        private:
            char m_layout[NUM_SMALL_ROWS][ROW_WIDTH+1];
        public:
            SmallBoard();
            SmallBoard(vector<string> input); // takes the five lines of input
            SmallBoard(const SmallBoard & other);
            SmallBoard& operator=(const SmallBoard & other);
            bool operator==(const SmallBoard & other);
            ~SmallBoard();
            string get_representation();
            void display();
            void display(int padding);
            
    };
    
    class SmallMove
    {
        private:
            SmallBoard * m_board;
            int m_depth;
        public:
            SmallMove(SmallBoard * board, int depth);
            ~SmallMove();
            bool is_final();
    };
            
    
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
        void create_next_move(Move & next, int anthro, int to_row, int to_col, int steps);
        bool has_matching_parent();
        Move * parent;
        
    };
}

using namespace Day23;

class AocDay23 : public AocDay
{
    private:
        Move parse_input(string filename);
        void find_best_move_depth_first_search(Move parent, int & lowest);
        static map<pair<int, int>, int> m_smallboard_index;
        map<int, pair<int, int>> m_smallboard_reverse_index;
        void set_up_indices();
    public:
        AocDay23();
        ~AocDay23();
        static map<pair<int, int>, int> get_smallboard_index();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
