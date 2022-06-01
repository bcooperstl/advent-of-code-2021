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
        string rep(19, ' ');
        map<pair<int, int>, int> index = AocDay23::get_smallboard_index();
        for (map<pair<int, int>,int>::iterator it = index.begin(); it != index.end(); ++it)
        {
            rep[it->second]=m_layout[it->first.first][it->first.second];
        }
        return rep;
    }
    
    MoveIndex::MoveIndex()
    {
    }
    
    MoveIndex::~MoveIndex()
    {
    }
    
    SmallMoveIndex::SmallMoveIndex()
    {
    }
    
    SmallMoveIndex::~SmallMoveIndex()
    {
    }
    
    vector<Move> SmallMoveIndex::get_moves(char anthro, int row, int col)
    {
        map<pair<int, int>, int> index = AocDay23::get_smallboard_index();
        return m_moves[anthro-ANTHRO_A][index[make_pair(row, col)]];
    }
    
    void SmallMoveIndex::add_move(Move move)
    {
        map<pair<int, int>, int> index = AocDay23::get_smallboard_index();
        m_moves[move.anthro-ANTHRO_A][index[make_pair(move.from_row,move.from_col)]].push_back(move);
        
        cout << "Adding move of anthro " << move.anthro << " from " << move.from_row << "," << move.from_col
             << " to " << move.to_row << "," << move.to_col 
             << " with " << move.steps << " steps and " << move.cost << " cost" 
             << " and mask set to [" << move.move_mask << "]" << endl;
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

void AocDay23::parse_input(string filename, SmallBoard & small_board)
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
        move.steps = abs(move.from_row - move.to_row) + abs(move.from_col - move.to_col);
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

string AocDay23::part1(string filename, vector<string> extra_args)
{
    SmallBoard small_board;
    parse_input(filename, small_board);
    small_board.display();
    
    SmallMoveIndex index;
    parse_moveindex("data/day23_smallmoves.txt", index, SMALL_REP_STR_LENGTH);
    
    int lowest = 0;
    
    ostringstream out;
    out << lowest;
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
    
    
}
