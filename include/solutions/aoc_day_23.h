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

#define NUM_SMALL_ANTHROS 8
#define NUM_LARGE_ANTHROS 16

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
#define SMALL_TOP_ROW 2
#define SMALL_BOTTOM_ROW 3
#define NUM_SMALL_ROWS 5

#define LARGE_BOTTOM_ROW 5
#define NUM_LARGE_ROWS 7


#define FINAL_SMALL_REP "...........ABCDABCD"
#define SMALL_REP_STR_LENGTH 19

#define FINAL_LARGE_REP "...........ABCDABCDABCDABCD"
#define LARGE_REP_STR_LENGTH 27

#define ROW_WIDTH 13

#define MOVE_TYPE_COL_TO_COL 0
#define MOVE_TYPE_HALL_TO_COL 1
#define MOVE_TYPE_COL_TO_HALL 2

#define NUM_MOVE_TYPES 3

namespace Day23
{
    class Board
    {
        public:
            Board();
            virtual ~Board();
            virtual void display() = 0;
            virtual void display(int padding) = 0;
            virtual string get_representation() = 0;
            virtual char get(int row, int col) = 0;
            virtual void set(int row, int col, char ch) = 0;
            virtual bool is_final(int row, int col) = 0;
            virtual Board * clone() = 0;
            virtual int get_num_final() = 0;
    };
    
    class SmallBoard : public Board
    {
        private:
            char m_layout[NUM_SMALL_ROWS][ROW_WIDTH+1];
        public:
            SmallBoard();
            SmallBoard(vector<string> input); // takes the five lines of input
            SmallBoard(const SmallBoard & other);
            SmallBoard& operator=(const SmallBoard & other);
            bool operator==(const SmallBoard & other);
            virtual ~SmallBoard();
            virtual string get_representation();
            virtual void display();
            virtual void display(int padding);
            virtual char get(int row, int col);
            virtual void set(int row, int col, char value);
            virtual bool is_final(int row, int col);
            virtual Board * clone();
            virtual int get_num_final();
    };
    
    class LargeBoard : public Board
    {
        private:
            char m_layout[NUM_LARGE_ROWS][ROW_WIDTH+1];
        public:
            LargeBoard();
            LargeBoard(vector<string> input); // takes the five lines of input
            LargeBoard(const LargeBoard & other);
            LargeBoard& operator=(const LargeBoard & other);
            bool operator==(const LargeBoard & other);
            virtual ~LargeBoard();
            virtual string get_representation();
            virtual void display();
            virtual void display(int padding);
            virtual char get(int row, int col);
            virtual void set(int row, int col, char value);
            virtual bool is_final(int row, int col);
            virtual Board * clone();
            virtual int get_num_final();
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
            virtual vector<Move> get_moves(int move_type, char anthro, int row, int col) = 0;
            virtual void add_move(Move move) = 0;
    };
    
    class SmallMoveIndex : public MoveIndex
    {
        private:
            int m_lookup[NUM_SMALL_ROWS][ROW_WIDTH];
            vector<Move> m_moves[NUM_MOVE_TYPES][NUM_ANTHRO_TYPES][SMALL_REP_STR_LENGTH];
        public:
            SmallMoveIndex();
            virtual ~SmallMoveIndex();
            virtual vector<Move> get_moves(int move_type, char anthro, int row, int col);
            void add_move(Move move);
    };
    
    class LargeMoveIndex : public MoveIndex
    {
        private:
            int m_lookup[NUM_LARGE_ROWS][ROW_WIDTH];
            vector<Move> m_moves[NUM_MOVE_TYPES][NUM_ANTHRO_TYPES][LARGE_REP_STR_LENGTH];
        public:
            LargeMoveIndex();
            virtual ~LargeMoveIndex();
            virtual vector<Move> get_moves(int move_type, char anthro, int row, int col);
            void add_move(Move move);
    };
    
    class Position
    {
        protected:
            Board * m_board;
            int m_cost;
            int m_best_total_cost;
            bool m_worked;
            int m_num_final;
        public:
            Position(Board * board, int cost);
            virtual ~Position();
            virtual bool is_final() = 0;
            bool is_worked();
            void set_worked(bool worked);
            int get_cost();
            void update_cost(int cost);
            int get_best_total_cost();
            Board * get_board();
            virtual Position * create(Board * board, int cost) = 0; // this is so ugly
            void set_num_final(int num_final);
            int get_num_final();
            virtual void set_best_total_cost() = 0;
    };
    
    class SmallPosition : public Position
    {
        private:
        public:
            SmallPosition(SmallBoard * board, int cost);
            virtual ~SmallPosition();
            virtual bool is_final();
            virtual Position * create(Board * board, int cost);
            virtual void set_best_total_cost();
    };
    
    class LargePosition : public Position
    {
        private:
        public:
            LargePosition(LargeBoard * board, int cost);
            virtual ~LargePosition();
            virtual bool is_final();
            virtual Position * create(Board * board, int cost);
            virtual void set_best_total_cost();
    };
    
    class Positions
    {
        private:
            map<string, Position *> m_position_map;
        public:
            Positions();
            ~Positions();
            Position * get_final_position();
            Position * find(string input);
            void add(Position * position);
            Position * get_next_position_to_work();
    };
    
}

using namespace Day23;

class AocDay23 : public AocDay
{
    private:
        void parse_input_small(string filename, SmallBoard & small_board);
        void parse_input_large(string filename, LargeBoard & large_board);
        void parse_moveindex(string filename, MoveIndex & index, int move_mask_length);
        static map<pair<int, int>, int> m_smallboard_index;
        static map<int, pair<int, int>> m_smallboard_reverse_index;
        static map<pair<int, int>, int> m_largeboard_index;
        static map<int, pair<int, int>> m_largeboard_reverse_index;
        void set_up_indices();
        void work_positions(MoveIndex & index, Positions & positions, map<int, pair<int, int>> locaiton_index);
    public:
        AocDay23();
        ~AocDay23();
        static map<pair<int, int>, int> get_smallboard_index();
        static map<int, pair<int, int>> get_smallboard_reverse_index();
        static map<pair<int, int>, int> get_largeboard_index();
        static map<int, pair<int, int>> get_largeboard_reverse_index();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif

