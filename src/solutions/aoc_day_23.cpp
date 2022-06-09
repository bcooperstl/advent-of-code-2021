#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include "aoc_day_23.h"
#include "file_utils.h"

using namespace std;
using namespace Day23;

namespace Day23
{
    Board::Board()
    {
    }
    
    Board::~Board()
    {
    }
    
    SmallBoard::SmallBoard()
    {
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            m_layout[i][0]='\0';
        }
    }
    
    SmallBoard::SmallBoard(vector<string> input)
    {
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            strncpy(m_layout[i], input[i].c_str(), ROW_WIDTH+1);
        }
    }
    
    SmallBoard::SmallBoard(const SmallBoard & other)
    {
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            strncpy(m_layout[i], other.m_layout[i], ROW_WIDTH+1);
        }
    }
    
    SmallBoard::~SmallBoard()
    {
    }
    
    SmallBoard & SmallBoard::operator =(const SmallBoard & other)
    {
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            strncpy(m_layout[i], other.m_layout[i], ROW_WIDTH+1);
        }
        return *this;
    }
    
    bool SmallBoard::operator == (const SmallBoard & other)
    {
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            if (strncmp(m_layout[i], other.m_layout[i], ROW_WIDTH) != 0)
            {
                return false;
            }
        }
        return true;
    }
    
    void SmallBoard::display()
    {
        display(0);
    }
    
    void SmallBoard::display(int padding)
    {
        string pad_str(padding, ' ');
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            cout << pad_str << m_layout[i] << endl;
        }
        cout << pad_str << "Representation string is " << get_representation() << endl;
    }
    
    string SmallBoard::get_representation()
    {
        string rep(SMALL_REP_STR_LENGTH, ' ');
        map<pair<int, int>, int> index = AocDay23::get_smallboard_index();
        for (map<pair<int, int>,int>::iterator it = index.begin(); it != index.end(); ++it)
        {
            rep[it->second]=m_layout[it->first.first][it->first.second];
        }
        return rep;
    }
    
    char SmallBoard::get(int row, int col)
    {
        return m_layout[row][col];
    }
    
    void SmallBoard::set(int row, int col, char value)
    {
        m_layout[row][col] = value;
    }
    
    bool SmallBoard::is_final(int row, int col)
    {
        // never final on landing row. doesn't really make sense anyways
        if (row == HALLWAY_ROW)
        {
            return false;
        }
        
        // final on bottom row if the anthro matches its column
        if (row == SMALL_BOTTOM_ROW)
        {
            return ((col == COL_A && m_layout[row][col] == ANTHRO_A) ||
                    (col == COL_B && m_layout[row][col] == ANTHRO_B) ||
                    (col == COL_C && m_layout[row][col] == ANTHRO_C) ||
                    (col == COL_D && m_layout[row][col] == ANTHRO_D));
        }
        else // row is top row in the column. need it and the bottom to match. need this to move a matching one out of the column to get a non-matching one below it out
        {
            return ((col == COL_A && m_layout[row][col] == ANTHRO_A && m_layout[SMALL_BOTTOM_ROW][col] == ANTHRO_A) ||
                    (col == COL_B && m_layout[row][col] == ANTHRO_B && m_layout[SMALL_BOTTOM_ROW][col] == ANTHRO_B) ||
                    (col == COL_C && m_layout[row][col] == ANTHRO_C && m_layout[SMALL_BOTTOM_ROW][col] == ANTHRO_C) ||
                    (col == COL_D && m_layout[row][col] == ANTHRO_D && m_layout[SMALL_BOTTOM_ROW][col] == ANTHRO_D));
        }
        
        // should not get here
        return false;
    }
    
    int SmallBoard::get_num_final()
    {
        int count = 0;
        count += is_final(SMALL_BOTTOM_ROW, COL_A) ? 1 : 0;
        count += is_final(SMALL_BOTTOM_ROW, COL_B) ? 1 : 0;
        count += is_final(SMALL_BOTTOM_ROW, COL_C) ? 1 : 0;
        count += is_final(SMALL_BOTTOM_ROW, COL_D) ? 1 : 0;
        count += is_final(SMALL_TOP_ROW, COL_A) ? 1 : 0;
        count += is_final(SMALL_TOP_ROW, COL_B) ? 1 : 0;
        count += is_final(SMALL_TOP_ROW, COL_C) ? 1 : 0;
        count += is_final(SMALL_TOP_ROW, COL_D) ? 1 : 0;
        return count;
    }
    
    Board * SmallBoard::clone()
    {
        return new SmallBoard(*this);
    }
    
    LargeBoard::LargeBoard()
    {
        for (int i=0; i<NUM_LARGE_ROWS; i++)
        {
            m_layout[i][0]='\0';
        }
    }
    
    /*
    ############# -- 0 from 0
    #...........# -- 1 from 1
    ###B#C#B#D### -- 2 from 2
      #D#C#B#A#   -- 3 manually entered
      #D#B#A#C#   -- 4 manually entered
      #A#D#C#A#   -- 5 from 3
      #########   -- 6 from 4
    */
    LargeBoard::LargeBoard(vector<string> input)
    {
        strncpy(m_layout[0], input[0].c_str(), ROW_WIDTH+1);
        strncpy(m_layout[1], input[1].c_str(), ROW_WIDTH+1);
        strncpy(m_layout[2], input[2].c_str(), ROW_WIDTH+1);
        strncpy(m_layout[3], "  #D#C#B#A#  ", ROW_WIDTH+1);
        strncpy(m_layout[4], "  #D#B#A#C#  ", ROW_WIDTH+1);
        strncpy(m_layout[5], input[3].c_str(), ROW_WIDTH+1);
        strncpy(m_layout[6], input[4].c_str(), ROW_WIDTH+1);
    }
    
    LargeBoard::LargeBoard(const LargeBoard & other)
    {
        for (int i=0; i<NUM_LARGE_ROWS; i++)
        {
            strncpy(m_layout[i], other.m_layout[i], ROW_WIDTH+1);
        }
    }
    
    LargeBoard::~LargeBoard()
    {
    }
    
    LargeBoard & LargeBoard::operator =(const LargeBoard & other)
    {
        for (int i=0; i<NUM_LARGE_ROWS; i++)
        {
            strncpy(m_layout[i], other.m_layout[i], ROW_WIDTH+1);
        }
        return *this;
    }
    
    bool LargeBoard::operator == (const LargeBoard & other)
    {
        for (int i=0; i<NUM_LARGE_ROWS; i++)
        {
            if (strncmp(m_layout[i], other.m_layout[i], ROW_WIDTH) != 0)
            {
                return false;
            }
        }
        return true;
    }
    
    void LargeBoard::display()
    {
        display(0);
    }
    
    void LargeBoard::display(int padding)
    {
        string pad_str(padding, ' ');
        for (int i=0; i<NUM_LARGE_ROWS; i++)
        {
            cout << pad_str << m_layout[i] << endl;
        }
        cout << pad_str << "Representation string is " << get_representation() << endl;
    }
    
    string LargeBoard::get_representation()
    {
        string rep(LARGE_REP_STR_LENGTH, ' ');
        map<pair<int, int>, int> index = AocDay23::get_largeboard_index();
        for (map<pair<int, int>,int>::iterator it = index.begin(); it != index.end(); ++it)
        {
            rep[it->second]=m_layout[it->first.first][it->first.second];
        }
        return rep;
    }
    
    char LargeBoard::get(int row, int col)
    {
        return m_layout[row][col];
    }
    
    void LargeBoard::set(int row, int col, char value)
    {
        m_layout[row][col] = value;
    }
    
    bool LargeBoard::is_final(int row, int col)
    {
        // never final on landing row. doesn't really make sense anyways
        if (row == HALLWAY_ROW)
        {
            return false;
        }
        
        // final on bottom row if the anthro matches its column
        if (row == LARGE_BOTTOM_ROW)
        {
            return ((col == COL_A && m_layout[row][col] == ANTHRO_A) ||
                    (col == COL_B && m_layout[row][col] == ANTHRO_B) ||
                    (col == COL_C && m_layout[row][col] == ANTHRO_C) ||
                    (col == COL_D && m_layout[row][col] == ANTHRO_D));
        }
        else if (row == LARGE_BOTTOM_ROW - 1) // need to check one below
        {
            return ((col == COL_A && m_layout[row][col] == ANTHRO_A && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_A) ||
                    (col == COL_B && m_layout[row][col] == ANTHRO_B && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_B) ||
                    (col == COL_C && m_layout[row][col] == ANTHRO_C && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_C) ||
                    (col == COL_D && m_layout[row][col] == ANTHRO_D && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_D));
        }
        else if (row == LARGE_BOTTOM_ROW - 2) // need to check two below
        {
            return ((col == COL_A && m_layout[row][col] == ANTHRO_A && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_A && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_A) ||
                    (col == COL_B && m_layout[row][col] == ANTHRO_B && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_B && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_B) ||
                    (col == COL_C && m_layout[row][col] == ANTHRO_C && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_C && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_C) ||
                    (col == COL_D && m_layout[row][col] == ANTHRO_D && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_D && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_D));
        }
        else if (row == LARGE_BOTTOM_ROW - 1) // need to check three below
        {
            return ((col == COL_A && m_layout[row][col] == ANTHRO_A && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_A && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_A && m_layout[LARGE_BOTTOM_ROW-2][col] == ANTHRO_A) ||
                    (col == COL_B && m_layout[row][col] == ANTHRO_B && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_B && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_B && m_layout[LARGE_BOTTOM_ROW-2][col] == ANTHRO_B) ||
                    (col == COL_C && m_layout[row][col] == ANTHRO_C && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_C && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_C && m_layout[LARGE_BOTTOM_ROW-2][col] == ANTHRO_C) ||
                    (col == COL_D && m_layout[row][col] == ANTHRO_D && m_layout[LARGE_BOTTOM_ROW][col] == ANTHRO_D && m_layout[LARGE_BOTTOM_ROW-1][col] == ANTHRO_D && m_layout[LARGE_BOTTOM_ROW-2][col] == ANTHRO_D));
        }
        
        // should not get here
        return false;
    }
    
    int LargeBoard::get_num_final()
    {
        int count = 0;
        for (int i=0; i<=3; i++)
        {
            count += is_final(LARGE_BOTTOM_ROW - i, COL_A) ? 1 : 0;
            count += is_final(LARGE_BOTTOM_ROW - i, COL_B) ? 1 : 0;
            count += is_final(LARGE_BOTTOM_ROW - i, COL_C) ? 1 : 0;
            count += is_final(LARGE_BOTTOM_ROW - i, COL_D) ? 1 : 0;
        }
        return count;
    }
    
    Board * LargeBoard::clone()
    {
        return new LargeBoard(*this);
    }
    
    MoveIndex::MoveIndex()
    {
    }
    
    MoveIndex::~MoveIndex()
    {
    }
    
    SmallMoveIndex::SmallMoveIndex()
    {
        for (int i=0; i<NUM_SMALL_ROWS; i++)
        {
            for (int j=0; j<ROW_WIDTH; j++)
            {
                m_lookup[i][j] = 0;
            }
        }
        map<int, pair<int, int>> index = AocDay23::get_smallboard_reverse_index();
        map<int, pair<int, int>>::iterator pos = index.begin();
        while (pos != index.end())
        {
            m_lookup[pos->second.first][pos->second.second] = pos->first;
            ++pos;
        }
    }
    
    SmallMoveIndex::~SmallMoveIndex()
    {
    }
    
    vector<Move> SmallMoveIndex::get_moves(int move_type, char anthro, int row, int col)
    {
        return m_moves[move_type][anthro-ANTHRO_A][m_lookup[row][col]];
    }
    
    void SmallMoveIndex::add_move(Move move)
    {
        int move_type = MOVE_TYPE_COL_TO_COL;
        if (move.from_row == HALLWAY_ROW)
        {
            move_type = MOVE_TYPE_HALL_TO_COL;
        }
        else if (move.to_row == HALLWAY_ROW)
        {
            move_type = MOVE_TYPE_COL_TO_HALL;
        }
        
        cout << "move type " << move_type << endl;
        cout << "Adding move of anthro " << move.anthro << " from " << move.from_row << "," << move.from_col
             << " to " << move.to_row << "," << move.to_col 
             << " with " << move.steps << " steps and " << move.cost << " cost" 
             << " and mask set to [" << move.move_mask << "]" << endl;
        m_moves[move_type][move.anthro-ANTHRO_A][m_lookup[move.from_row][move.from_col]].push_back(move);
        
    }
    
    LargeMoveIndex::LargeMoveIndex()
    {
        for (int i=0; i<NUM_LARGE_ROWS; i++)
        {
            for (int j=0; j<ROW_WIDTH; j++)
            {
                m_lookup[i][j] = 0;
            }
        }
        map<int, pair<int, int>> index = AocDay23::get_largeboard_reverse_index();
        map<int, pair<int, int>>::iterator pos = index.begin();
        while (pos != index.end())
        {
            m_lookup[pos->second.first][pos->second.second] = pos->first;
            ++pos;
        }
    }
    
    LargeMoveIndex::~LargeMoveIndex()
    {
    }
    
    vector<Move> LargeMoveIndex::get_moves(int move_type, char anthro, int row, int col)
    {
        return m_moves[move_type][anthro-ANTHRO_A][m_lookup[row][col]];
    }
    
    void LargeMoveIndex::add_move(Move move)
    {
        int move_type = MOVE_TYPE_COL_TO_COL;
        if (move.from_row == HALLWAY_ROW)
        {
            move_type = MOVE_TYPE_HALL_TO_COL;
        }
        else if (move.to_row == HALLWAY_ROW)
        {
            move_type = MOVE_TYPE_COL_TO_HALL;
        }
        
        cout << "move type " << move_type << endl;
        cout << "Adding move of anthro " << move.anthro << " from " << move.from_row << "," << move.from_col
             << " to " << move.to_row << "," << move.to_col 
             << " with " << move.steps << " steps and " << move.cost << " cost" 
             << " and mask set to [" << move.move_mask << "]" << endl;
        m_moves[move_type][move.anthro-ANTHRO_A][m_lookup[move.from_row][move.from_col]].push_back(move);
    }
    
    Position::Position(Board * board, int cost)
    {
        m_board = board;
        m_cost = cost;
        m_worked = false;
        m_num_final = board->get_num_final();
    }
    
    Position::~Position()
    {
        delete m_board;
    }
    
    bool Position::is_worked()
    {
        return m_worked;
    }
    
    void Position::set_worked(bool worked)
    {
        m_worked = worked;
    }
    
    int Position::get_cost()
    {
        return m_cost;
    }
    
    void Position::update_cost(int cost)
    {
        m_cost = cost;
        if (cost > m_cost)
        {
            cout << "WARNING...raising cost...you probably don't want to do this" << endl;
        }
        set_best_total_cost();
    }
    
    Board * Position::get_board()
    {
        return m_board;
    }
    
    int Position::get_num_final()
    {
        return m_num_final;
    }
    
    void Position::set_num_final(int num_final)
    {
        m_num_final = num_final;
    }
    
    int Position::get_best_total_cost()
    {
        return m_best_total_cost;
    }
    
    SmallPosition::SmallPosition(SmallBoard * board, int cost):Position(board, cost)
    {
        set_best_total_cost();
    }
    
    SmallPosition::~SmallPosition()
    {
    }
    
    bool SmallPosition::is_final()
    {
        return (m_num_final == NUM_SMALL_ANTHROS);
    }
    
    void SmallPosition::set_best_total_cost()
    {
        m_best_total_cost = m_cost;
        
        map<pair<int, int>, int> index = AocDay23::get_smallboard_index();
        for (map<pair<int, int>,int>::iterator it = index.begin(); it != index.end(); ++it)
        {
            char ch = m_board->get(it->first.first, it->first.second);
            int step_cost = 0;
            int target_col = 0;
            int up_steps = 0;
            int over_steps = 0;
            int down_steps = 0;
            switch (ch)
            {
                case 'A':
                    step_cost = COST_A;
                    target_col = COL_A;
                    break;
                case 'B':
                    step_cost = COST_B;
                    target_col = COL_B;
                    break;
                case 'C':
                    step_cost = COST_C;
                    target_col = COL_C;
                    break;
                case 'D':
                    step_cost = COST_D;
                    target_col = COL_D;
                    break;
            }
            if (step_cost > 0)
            {
                if (it->first.second == target_col)
                {
                    // already in the target column. just move it to the bottom
                    down_steps = SMALL_BOTTOM_ROW - it->first.first;
                }
                else
                {
                    up_steps = it->first.first - HALLWAY_ROW;
                    down_steps = SMALL_BOTTOM_ROW - HALLWAY_ROW;
                    over_steps = abs(target_col - it->first.second);
                }
                m_best_total_cost += (step_cost * (up_steps + over_steps + down_steps));
//                cout << "Moving " << ch << " from " << it->first.first << "," << it->first.second 
//                     << " to " << SMALL_BOTTOM_ROW << "," << target_col 
//                     << " takes " << (up_steps + down_steps + over_steps) << " steps and costs " << (step_cost * (up_steps + over_steps + down_steps)) << endl;
            }
        }
        m_best_total_cost -= ((COST_A + COST_B + COST_C + COST_D)*1); // subtract the one step for cols A,B,C, and D for the top row
//        cout << "Best total cost is " << m_best_total_cost << " after adding all of the steps to current position cost " << m_cost << endl;
    }
    
    Position * SmallPosition::create(Board * board, int cost)
    {
        return new SmallPosition((SmallBoard *)board, cost);
    }
    
    LargePosition::LargePosition(LargeBoard * board, int cost):Position(board, cost)
    {
        set_best_total_cost();
    }
    
    LargePosition::~LargePosition()
    {
    }
    
    bool LargePosition::is_final()
    {
        return (m_num_final == NUM_LARGE_ANTHROS);
    }
    
    void LargePosition::set_best_total_cost()
    {
        m_best_total_cost = m_cost;
        
        map<pair<int, int>, int> index = AocDay23::get_largeboard_index();
        for (map<pair<int, int>,int>::iterator it = index.begin(); it != index.end(); ++it)
        {
            char ch = m_board->get(it->first.first, it->first.second);
            int step_cost = 0;
            int target_col = 0;
            int up_steps = 0;
            int over_steps = 0;
            int down_steps = 0;
            switch (ch)
            {
                case 'A':
                    step_cost = COST_A;
                    target_col = COL_A;
                    break;
                case 'B':
                    step_cost = COST_B;
                    target_col = COL_B;
                    break;
                case 'C':
                    step_cost = COST_C;
                    target_col = COL_C;
                    break;
                case 'D':
                    step_cost = COST_D;
                    target_col = COL_D;
                    break;
            }
            if (step_cost > 0)
            {
                if (it->first.second == target_col)
                {
                    // already in the target column. just move it to the bottom
                    down_steps = SMALL_BOTTOM_ROW - it->first.first;
                }
                else
                {
                    up_steps = it->first.first - HALLWAY_ROW;
                    down_steps = LARGE_BOTTOM_ROW - HALLWAY_ROW;
                    over_steps = abs(target_col - it->first.second);
                }
                m_best_total_cost += (step_cost * (up_steps + over_steps + down_steps));
//                cout << "Moving " << ch << " from " << it->first.first << "," << it->first.second 
//                     << " to " << SMALL_BOTTOM_ROW << "," << target_col 
//                     << " takes " << (up_steps + down_steps + over_steps) << " steps and costs " << (step_cost * (up_steps + over_steps + down_steps)) << endl;
            }
        }
        m_best_total_cost -= ((COST_A + COST_B + COST_C + COST_D)*6); // subtract the one step for cols A,B,C, and D for the next-to-bottom row, 2 for the one above it, and 3 for the one above it
//        cout << "Best total cost is " << m_best_total_cost << " after adding all of the steps to current position cost " << m_cost << endl;
    }
    
    Position * LargePosition::create(Board * board, int cost)
    {
        return new LargePosition((LargeBoard *)board, cost);
    }
    
    Positions::Positions()
    {
    }
    
    Positions::~Positions()
    {
        map<string, Position*>::iterator pos = m_position_map.begin();
        while (pos != m_position_map.end())
        {
            delete pos->second;
            ++pos;
        }
    }
    
    Position * Positions::get_final_position()
    {
        map<string, Position *>::iterator pos = m_position_map.begin();
        while (pos != m_position_map.end())
        {
            if (pos->second->is_final())
            {
                cout << "Postion has " << pos->second->get_num_final() << " as final" << endl;
                return pos->second;
            }
            ++pos;
        }
        return NULL;
    }
    
    Position * Positions::find(string input)
    {
        map<string, Position *>::iterator found = m_position_map.find(input);
        if (found != m_position_map.end())
        {
            return found->second;
        }
        return NULL;
    }
    
    void Positions::add(Position * position)
    {
        m_position_map[position->get_board()->get_representation()] = position;
    }
    
    Position * Positions::get_next_position_to_work()
    {
        // return the unworked position with the most in final and lowest cost
        Position * best = NULL;
        map<string, Position *>::iterator pos = m_position_map.begin();
        while (pos != m_position_map.end())
        {
            if (!pos->second->is_worked())
            {
                if (best == NULL)
                {
                    best = pos->second;
                }
                else if ((pos->second->get_best_total_cost() < best->get_best_total_cost()))
                {
                    best = pos->second;
                }
            }
            ++pos;
        }
        return best;
    }
    
    
};

AocDay23::AocDay23():AocDay(23)
{
    set_up_indices();
}

AocDay23::~AocDay23()
{
}

/*
#############
#...........#
###B#C#B#D###
  #A#D#C#A#
  #########
*/

void AocDay23::parse_input_small(string filename, SmallBoard & small_board)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    small_board = SmallBoard(lines);
}

void AocDay23::parse_input_large(string filename, LargeBoard & large_board)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    large_board = LargeBoard(lines);
}

void AocDay23::parse_moveindex(string filename, MoveIndex & index, int move_mask_length)
{
/*
!Format is Anthro|Start Location|End Location|Requirements
!Requirements are of the format value:Cell[,Cell...]
!Anthro can be A,B,C,D,X where X is any
*/
    FileUtils fileutils;
    vector<vector<string>> moves;
    char delimiters[3];
    delimiters[0] = '|';
    delimiters[1] = ':';
    delimiters[2] = ',';

    if (!fileutils.read_as_list_of_split_strings(filename, moves, delimiters, 3, '\0', '!'))
    {
        cerr << "Error reading in move index from " << filename << endl;
        return;
    }
    
    for (int i=0; i<moves.size(); i++)
    {
        Move move;
        move.anthro = moves[i][0][0]; // get the first char
        pair<int, int> from = m_smallboard_reverse_index[strtol(moves[i][1].c_str(), NULL, 10)];
        move.from_row = from.first;
        move.from_col = from.second;
        pair<int, int> to = m_smallboard_reverse_index[strtol(moves[i][2].c_str(), NULL, 10)];
        move.to_row = to.first;
        move.to_col = to.second;
        move.steps = abs(move.from_row - HALLWAY_ROW) + abs(move.to_row - HALLWAY_ROW) + abs(move.from_col - move.to_col);
        move.move_mask = string(move_mask_length, ' ');
        char current = ' ';
        for (int j=3; j<moves[i].size(); j++)
        {
            if (moves[i][j][0] == ANTHRO_A || moves[i][j][0] == ANTHRO_B || moves[i][j][0] == ANTHRO_C || moves[i][j][0] == ANTHRO_D || moves[i][j][0] == OPEN)
            {
                current = moves[i][j][0];
            }
            else
            {
                move.move_mask[strtol(moves[i][j].c_str(), NULL, 10)] = current;
            }
        }
        
        bool all = (move.anthro == ANTHRO_ALL);
        
        if (move.anthro == ANTHRO_A || all)
        {
            move.anthro = ANTHRO_A;
            move.cost = move.steps * COST_A;
            index.add_move(move);
        }
        if (move.anthro == ANTHRO_B || all)
        {
            move.anthro = ANTHRO_B;
            move.cost = move.steps * COST_B;
            index.add_move(move);
        }
        if (move.anthro == ANTHRO_C || all)
        {
            move.anthro = ANTHRO_C;
            move.cost = move.steps * COST_C;
            index.add_move(move);
        }
        if (move.anthro == ANTHRO_D || all)
        {
            move.anthro = ANTHRO_D;
            move.cost = move.steps * COST_D;
            index.add_move(move);
        }
    }
}

void AocDay23::work_positions(MoveIndex & index, Positions & positions, map<int, pair<int, int>> location_index)
{
    Position * position = positions.get_next_position_to_work();
    int location_rows[LARGE_REP_STR_LENGTH];
    int location_cols[LARGE_REP_STR_LENGTH];
    int num_locations = 0;
    
    map<int, pair<int, int>>::iterator loca_pos = location_index.begin();
    while (loca_pos != location_index.end())
    {
        location_rows[num_locations] = loca_pos->second.first;
        location_cols[num_locations] = loca_pos->second.second;
        num_locations++;
        ++loca_pos;
    }
    
    while (position != NULL)
    {
        Board * board = position->get_board();
        string rep = board->get_representation();
        cout << "Working position with cost " << position->get_cost() << " and best cost " << position->get_best_total_cost() << " and board " << rep << endl;
        if (position->is_final()) // no need to go further
        {
            return;
        }
        //board->display();
        
        bool did_a_move = false;
        // work col to col moves
        for (int i=0; i<num_locations; i++)
        {
            int row = location_rows[i];
            int col = location_cols[i];
            char at = board->get(row, col);
            if (row == HALLWAY_ROW) // must not start in hallway row
            {
                continue;
            }
            if (at == OPEN) // nothing to do at open start spot
            {
                continue;
            }
            if (board->is_final(row, col)) // already in a final spot. nothing to do
            {
                continue;
            }
            vector<Move> moves = index.get_moves(MOVE_TYPE_COL_TO_COL, at, row, col);
            vector<Move>::iterator move_iter = moves.begin();
            for (vector<Move>::iterator move_iter = moves.begin(); move_iter != moves.end(); ++move_iter)
            {
                Move move = *move_iter;
                //cout << " Checking move from " << move.from_row << "," << move.from_col << " to " << move.to_row << "," << move.to_col << endl;
                if (board->get(move.to_row, move.to_col) != OPEN)
                {
                    //cout << "  Ending location is not open. skipping move" << endl;
                    continue;
                }
                bool match = true;
                // check if mask matches
                for (int j=0; j<move.move_mask.length(); j++)
                {
                    if (move.move_mask[j] != ' ' && move.move_mask[j] != rep[j])
                    {
                        //cout << "  Mask mismatch at position " << j << " makes move invalid. skipping move." << endl;
                        match = false;
                    }
                }
                if (match == true)
                {
                    did_a_move = true;
                    Board * next_board = board->clone();
                    next_board->set(move.from_row, move.from_col, OPEN);
                    next_board->set(move.to_row, move.to_col, move.anthro);
                    int next_cost = position->get_cost() + move.cost;
                    string next_rep = next_board->get_representation();
                    
                    //cout << "  This is a potential move to add with cost " << next_cost << " and board " <<endl;
                    //next_board->display(2);
                    
                    Position * found_position = positions.find(next_rep);
                    if (found_position == NULL)
                    {
                        //cout << "   New position to be added" << endl;
                        Position * next_position = position->create(next_board, next_cost);
                        next_position->set_num_final(position->get_num_final() + (move.to_row != HALLWAY_ROW ? 1 : 0));
                        positions.add(next_position);
                    }
                    else
                    {
                        if (next_cost < found_position->get_cost())
                        {
                            //cout << "   Lower cost of " << next_cost << " is better than existing cost " << found_position->get_cost() << endl;
                            found_position->update_cost(next_cost);
                        }
                        else
                        {
                            //cout << "   Existing position has better cost of " << found_position->get_cost() << endl;
                        }
                        delete next_board;
                    }
                }
            }
        }
        if (did_a_move == true)
        {
            position->set_worked(true);
            position = positions.get_next_position_to_work();
            continue;
        }

                
        did_a_move = false;
        // now try hall to col moves
        for (int i=0; i<num_locations; i++)
        {
            int row = location_rows[i];
            int col = location_cols[i];
            char at = board->get(row, col);
            if (row != HALLWAY_ROW) // must start in hallway row
            {
                continue;
            }
            if (at == OPEN) // nothing to do at open start spot
            {
                continue;
            }
            if (board->is_final(row, col)) // already in a final spot. nothing to do
            {
                continue;
            }
            vector<Move> moves = index.get_moves(MOVE_TYPE_HALL_TO_COL, at, row, col);
            vector<Move>::iterator move_iter = moves.begin();
            for (vector<Move>::iterator move_iter = moves.begin(); move_iter != moves.end(); ++move_iter)
            {
                Move move = *move_iter;
                //cout << " Checking move from " << move.from_row << "," << move.from_col << " to " << move.to_row << "," << move.to_col << endl;
                if (board->get(move.to_row, move.to_col) != OPEN)
                {
                    //cout << "  Ending location is not open. skipping move" << endl;
                    continue;
                }
                bool match = true;
                // check if mask matches
                for (int j=0; j<move.move_mask.length(); j++)
                {
                    if (move.move_mask[j] != ' ' && move.move_mask[j] != rep[j])
                    {
                        //cout << "  Mask mismatch at position " << j << " makes move invalid. skipping move." << endl;
                        match = false;
                    }
                }
                if (match == true)
                {
                    did_a_move = true;
                    Board * next_board = board->clone();
                    next_board->set(move.from_row, move.from_col, OPEN);
                    next_board->set(move.to_row, move.to_col, move.anthro);
                    int next_cost = position->get_cost() + move.cost;
                    string next_rep = next_board->get_representation();
                    
                    //cout << "  This is a potential move to add with cost " << next_cost << " and board " <<endl;
                    //next_board->display(2);
                    
                    Position * found_position = positions.find(next_rep);
                    if (found_position == NULL)
                    {
                        //cout << "   New position to be added" << endl;
                        Position * next_position = position->create(next_board, next_cost);
                        next_position->set_num_final(position->get_num_final() + (move.to_row != HALLWAY_ROW ? 1 : 0));
                        positions.add(next_position);
                    }
                    else
                    {
                        if (next_cost < found_position->get_cost())
                        {
                            //cout << "   Lower cost of " << next_cost << " is better than existing cost " << found_position->get_cost() << endl;
                            found_position->update_cost(next_cost);
                        }
                        else
                        {
                            //cout << "   Existing position has better cost of " << found_position->get_cost() << endl;
                        }
                        delete next_board;
                    }
                }
            }
        }
        if (did_a_move == true)
        {
            position->set_worked(true);
            position = positions.get_next_position_to_work();
            continue;
        }
            
        // now do col to hall
        for (int i=0; i<num_locations; i++)
        {
            int row = location_rows[i];
            int col = location_cols[i];
            char at = board->get(row, col);
            if (row == HALLWAY_ROW) // must start not in hallway row
            {
                continue;
            }
            if (at == OPEN) // nothing to do at open start spot
            {
                continue;
            }
            if (board->is_final(row, col)) // already in a final spot. nothing to do
            {
                continue;
            }
            vector<Move> moves = index.get_moves(MOVE_TYPE_COL_TO_HALL, at, row, col);
            vector<Move>::iterator move_iter = moves.begin();
            for (vector<Move>::iterator move_iter = moves.begin(); move_iter != moves.end(); ++move_iter)
            {
                Move move = *move_iter;
                //cout << " Checking move from " << move.from_row << "," << move.from_col << " to " << move.to_row << "," << move.to_col << endl;
                if (board->get(move.to_row, move.to_col) != OPEN)
                {
                    //cout << "  Ending location is not open. skipping move" << endl;
                    continue;
                }
                bool match = true;
                // check if mask matches
                for (int j=0; j<move.move_mask.length(); j++)
                {
                    if (move.move_mask[j] != ' ' && move.move_mask[j] != rep[j])
                    {
                        //cout << "  Mask mismatch at position " << j << " makes move invalid. skipping move." << endl;
                        match = false;
                    }
                }
                if (match == true)
                {
                    Board * next_board = board->clone();
                    next_board->set(move.from_row, move.from_col, OPEN);
                    next_board->set(move.to_row, move.to_col, move.anthro);
                    int next_cost = position->get_cost() + move.cost;
                    string next_rep = next_board->get_representation();
                    
                    //cout << "  This is a potential move to add with cost " << next_cost << " and board " <<endl;
                    //next_board->display(2);
                    
                    Position * found_position = positions.find(next_rep);
                    if (found_position == NULL)
                    {
                        //cout << "   New position to be added" << endl;
                        Position * next_position = position->create(next_board, next_cost);
                        next_position->set_num_final(position->get_num_final() + (move.to_row != HALLWAY_ROW ? 1 : 0));
                        positions.add(next_position);
                    }
                    else
                    {
                        if (next_cost < found_position->get_cost())
                        {
                            //cout << "   Lower cost of " << next_cost << " is better than existing cost " << found_position->get_cost() << endl;
                            found_position->update_cost(next_cost);
                        }
                        else
                        {
                            //cout << "   Existing position has better cost of " << found_position->get_cost() << endl;
                        }
                        delete next_board;
                    }
                }
            }
        }
        
        position->set_worked(true);
        position = positions.get_next_position_to_work();
    }
    
};

string AocDay23::part1(string filename, vector<string> extra_args)
{
    SmallBoard small_board;
    parse_input_small(filename, small_board);
    small_board.display();
    
    SmallMoveIndex index;
    parse_moveindex("data/day23_smallmoves.txt", index, SMALL_REP_STR_LENGTH);
    
    Positions positions;
    
    Position * start_position = new SmallPosition(new SmallBoard(small_board), 0);
    positions.add(start_position);
    
    work_positions(index, positions, m_smallboard_reverse_index);
    
    Position * final = positions.get_final_position();
    
    ostringstream out;
    if (final != NULL)
    {
        out << final->get_cost();
    }
    else
    {
        out << "";
    }
    return out.str();
}

string AocDay23::part2(string filename, vector<string> extra_args)
{
    LargeBoard large_board;
    parse_input_large(filename, large_board);
    large_board.display();
    
    LargeMoveIndex index;
    parse_moveindex("data/day23_largemoves.txt", index, LARGE_REP_STR_LENGTH);
    
    Positions positions;
    
    Position * start_position = new LargePosition(new LargeBoard(large_board), 0);
    positions.add(start_position);
    
    work_positions(index, positions, m_largeboard_reverse_index);
    
    Position * final = positions.get_final_position();
    
    ostringstream out;
    if (final != NULL)
    {
        out << final->get_cost();
    }
    else
    {
        out << "";
    }
    return out.str();
}

map<pair<int, int>, int> AocDay23::m_smallboard_index;

map<int, pair<int, int>> AocDay23::m_smallboard_reverse_index;

map<pair<int, int>, int> AocDay23::get_smallboard_index()
{
    return m_smallboard_index;
}

map<int, pair<int, int>> AocDay23::get_smallboard_reverse_index()
{
    return m_smallboard_reverse_index;
}

map<pair<int, int>, int> AocDay23::m_largeboard_index;

map<int, pair<int, int>> AocDay23::m_largeboard_reverse_index;

map<pair<int, int>, int> AocDay23::get_largeboard_index()
{
    return m_largeboard_index;
}

map<int, pair<int, int>> AocDay23::get_largeboard_reverse_index()
{
    return m_largeboard_reverse_index;
}

void AocDay23::set_up_indices()
{
    m_smallboard_index[make_pair(HALLWAY_ROW,1)] = 0;
    m_smallboard_index[make_pair(HALLWAY_ROW,2)] = 1;
    m_smallboard_index[make_pair(HALLWAY_ROW,3)] = 2;
    m_smallboard_index[make_pair(HALLWAY_ROW,4)] = 3;
    m_smallboard_index[make_pair(HALLWAY_ROW,5)] = 4;
    m_smallboard_index[make_pair(HALLWAY_ROW,6)] = 5;
    m_smallboard_index[make_pair(HALLWAY_ROW,7)] = 6;
    m_smallboard_index[make_pair(HALLWAY_ROW,8)] = 7;
    m_smallboard_index[make_pair(HALLWAY_ROW,9)] = 8;
    m_smallboard_index[make_pair(HALLWAY_ROW,10)] = 9;
    m_smallboard_index[make_pair(HALLWAY_ROW,11)] = 10;
    m_smallboard_index[make_pair(2,COL_A)] = 11;
    m_smallboard_index[make_pair(2,COL_B)] = 12;
    m_smallboard_index[make_pair(2,COL_C)] = 13;
    m_smallboard_index[make_pair(2,COL_D)] = 14;
    m_smallboard_index[make_pair(3,COL_A)] = 15;
    m_smallboard_index[make_pair(3,COL_B)] = 16;
    m_smallboard_index[make_pair(3,COL_C)] = 17;
    m_smallboard_index[make_pair(3,COL_D)] = 18;
    
    m_smallboard_reverse_index[0] = make_pair(HALLWAY_ROW,1);
    m_smallboard_reverse_index[1] = make_pair(HALLWAY_ROW,2);
    m_smallboard_reverse_index[2] = make_pair(HALLWAY_ROW,3);
    m_smallboard_reverse_index[3] = make_pair(HALLWAY_ROW,4);
    m_smallboard_reverse_index[4] = make_pair(HALLWAY_ROW,5);
    m_smallboard_reverse_index[5] = make_pair(HALLWAY_ROW,6);
    m_smallboard_reverse_index[6] = make_pair(HALLWAY_ROW,7);
    m_smallboard_reverse_index[7] = make_pair(HALLWAY_ROW,8);
    m_smallboard_reverse_index[8] = make_pair(HALLWAY_ROW,9);
    m_smallboard_reverse_index[9] = make_pair(HALLWAY_ROW,10);
    m_smallboard_reverse_index[10] = make_pair(HALLWAY_ROW,11);
    m_smallboard_reverse_index[11] = make_pair(2,COL_A);
    m_smallboard_reverse_index[12] = make_pair(2,COL_B);
    m_smallboard_reverse_index[13] = make_pair(2,COL_C);
    m_smallboard_reverse_index[14] = make_pair(2,COL_D);
    m_smallboard_reverse_index[15] = make_pair(3,COL_A);
    m_smallboard_reverse_index[16] = make_pair(3,COL_B);
    m_smallboard_reverse_index[17] = make_pair(3,COL_C);
    m_smallboard_reverse_index[18] = make_pair(3,COL_D);
    
    m_largeboard_index[make_pair(HALLWAY_ROW,1)] = 0;
    m_largeboard_index[make_pair(HALLWAY_ROW,2)] = 1;
    m_largeboard_index[make_pair(HALLWAY_ROW,3)] = 2;
    m_largeboard_index[make_pair(HALLWAY_ROW,4)] = 3;
    m_largeboard_index[make_pair(HALLWAY_ROW,5)] = 4;
    m_largeboard_index[make_pair(HALLWAY_ROW,6)] = 5;
    m_largeboard_index[make_pair(HALLWAY_ROW,7)] = 6;
    m_largeboard_index[make_pair(HALLWAY_ROW,8)] = 7;
    m_largeboard_index[make_pair(HALLWAY_ROW,9)] = 8;
    m_largeboard_index[make_pair(HALLWAY_ROW,10)] = 9;
    m_largeboard_index[make_pair(HALLWAY_ROW,11)] = 10;
    m_largeboard_index[make_pair(2,COL_A)] = 11;
    m_largeboard_index[make_pair(2,COL_B)] = 12;
    m_largeboard_index[make_pair(2,COL_C)] = 13;
    m_largeboard_index[make_pair(2,COL_D)] = 14;
    m_largeboard_index[make_pair(3,COL_A)] = 15;
    m_largeboard_index[make_pair(3,COL_B)] = 16;
    m_largeboard_index[make_pair(3,COL_C)] = 17;
    m_largeboard_index[make_pair(3,COL_D)] = 18;
    m_largeboard_index[make_pair(4,COL_A)] = 19;
    m_largeboard_index[make_pair(4,COL_B)] = 20;
    m_largeboard_index[make_pair(4,COL_C)] = 21;
    m_largeboard_index[make_pair(4,COL_D)] = 22;
    m_largeboard_index[make_pair(5,COL_A)] = 23;
    m_largeboard_index[make_pair(5,COL_B)] = 24;
    m_largeboard_index[make_pair(5,COL_C)] = 25;
    m_largeboard_index[make_pair(5,COL_D)] = 26;
    
    m_largeboard_reverse_index[0] = make_pair(HALLWAY_ROW,1);
    m_largeboard_reverse_index[1] = make_pair(HALLWAY_ROW,2);
    m_largeboard_reverse_index[2] = make_pair(HALLWAY_ROW,3);
    m_largeboard_reverse_index[3] = make_pair(HALLWAY_ROW,4);
    m_largeboard_reverse_index[4] = make_pair(HALLWAY_ROW,5);
    m_largeboard_reverse_index[5] = make_pair(HALLWAY_ROW,6);
    m_largeboard_reverse_index[6] = make_pair(HALLWAY_ROW,7);
    m_largeboard_reverse_index[7] = make_pair(HALLWAY_ROW,8);
    m_largeboard_reverse_index[8] = make_pair(HALLWAY_ROW,9);
    m_largeboard_reverse_index[9] = make_pair(HALLWAY_ROW,10);
    m_largeboard_reverse_index[10] = make_pair(HALLWAY_ROW,11);
    m_largeboard_reverse_index[11] = make_pair(2,COL_A);
    m_largeboard_reverse_index[12] = make_pair(2,COL_B);
    m_largeboard_reverse_index[13] = make_pair(2,COL_C);
    m_largeboard_reverse_index[14] = make_pair(2,COL_D);
    m_largeboard_reverse_index[15] = make_pair(3,COL_A);
    m_largeboard_reverse_index[16] = make_pair(3,COL_B);
    m_largeboard_reverse_index[17] = make_pair(3,COL_C);
    m_largeboard_reverse_index[18] = make_pair(3,COL_D);
    m_largeboard_reverse_index[19] = make_pair(4,COL_A);
    m_largeboard_reverse_index[20] = make_pair(4,COL_B);
    m_largeboard_reverse_index[21] = make_pair(4,COL_C);
    m_largeboard_reverse_index[22] = make_pair(4,COL_D);
    m_largeboard_reverse_index[23] = make_pair(5,COL_A);
    m_largeboard_reverse_index[24] = make_pair(5,COL_B);
    m_largeboard_reverse_index[25] = make_pair(5,COL_C);
    m_largeboard_reverse_index[26] = make_pair(5,COL_D);
}
