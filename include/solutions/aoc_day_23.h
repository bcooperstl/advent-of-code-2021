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

#define ANTHRO_ALL 'X'

#define NUM_ANTHRO_TYPES 4

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
#define SMALL_REP_STR_LENGTH 19

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
    
    struct Move
    {
        char anthro;
        int from_row;
        int from_col;
        int to_row;
        int to_col;
        int steps;
        int cost;
        string move_mask;
    };
    
    
    class MoveIndex
    {
        private:
        public:
            MoveIndex();
            virtual ~MoveIndex();
            virtual vector<Move> get_moves(char anthro, int row, int col) = 0;
            virtual void add_move(Move move) = 0;
    };
    
    class SmallMoveIndex : public MoveIndex
    {
        private:
            vector<Move> m_moves[NUM_ANTHRO_TYPES][SMALL_REP_STR_LENGTH];
        public:
            SmallMoveIndex();
            virtual ~SmallMoveIndex();
            virtual vector<Move> get_moves(char anthro, int row, int col);
            void add_move(Move move);
    };
    
    class SmallMove
    {
        private:
            SmallBoard * m_board;
            int m_cost;
            bool m_worked;
        public:
            SmallMove(SmallBoard * board, int cost);
            ~SmallMove();
            bool is_final();
            bool is_worked();
            int get_cost();
            void update_cost(int cost);
    };
    
}

using namespace Day23;

class AocDay23 : public AocDay
{
    private:
        void parse_input(string filename, SmallBoard & small_board);
        void parse_moveindex(string filename, MoveIndex & index, int move_mask_length);
        static map<pair<int, int>, int> m_smallboard_index;
        static map<int, pair<int, int>> m_smallboard_reverse_index;
        void set_up_indices();
    public:
        AocDay23();
        ~AocDay23();
        static map<pair<int, int>, int> get_smallboard_index();
        static map<int, pair<int, int>> get_smallboard_reverse_index();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif

