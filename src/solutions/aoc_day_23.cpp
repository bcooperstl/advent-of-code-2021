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
    void Board::display()
    {
        display(0);
    }
    
    void Board::display(int padding)
    {
        string pad_str(padding, ' ');
        for (int i=0; i<5; i++)
        {
            cout << pad_str << layout[i] << endl;
        }
    }
    
    bool Move::is_final()
    {
        for (int i=0; i<NUM_ANTHROS; i++)
        {
            if (!is_final(i))
            {
                return false;
            }
        }
        return true;
    }
    
    bool Move::is_final(int position)
    {
        int final_col;
        
        switch (positions[position].anthro)
        {
            case ANTHRO_A:
                final_col = COL_A;
                break;
            case ANTHRO_B:
                final_col = COL_B;
                break;
            case ANTHRO_C:
                final_col = COL_C;
                break;
            case ANTHRO_D:
                final_col = COL_D;
                break;
        }
        if (positions[position].col != final_col)
        {
            return false;
        }
        
        // we are in the right column
        // if we are at the bottom row, we are in a final position
        if (positions[position].row == BOTTOM_ROW)
        {
            return true;
        }
        
        // if we are at the top row, we are only at a final position if the one below is matches our type
        if (positions[position].row == TOP_ROW)
        {
            return (positions[position].anthro == board.layout[BOTTOM_ROW][final_col]);
        }
        // not in a final row so can't be final
        return false;
    }
    
    bool Move::can_move(int position, int to_row, int to_col, int & steps)
    {
        string pad_str(depth, ' ');
        
        steps = 0;
        
        int from_row = positions[position].row;
        int from_col = positions[position].col;
        char anthro = positions[position].anthro;
        
        //cout << pad_str << "checking if anthro " << anthro << " can move from " << from_row << ", " << from_col << " to " << to_row << "," << to_col << endl;
        // can only move into an open space
        if (board.layout[to_row][to_col] != OPEN)
        {
            //cout << pad_str << " destination is not open. no move" << endl;
            return false;
        }
        
        // cannot stop in the hallway above a room
        if (to_row == HALLWAY_ROW && (to_col == COL_A || to_col == COL_B || to_col == COL_C || to_col == COL_D))
        {
            //cout << pad_str << " destination is in hallway above a room. no move" << endl;
            return false;
        }
        
        // never move from a hallway into a room unless that is their destination room
        if (from_row == HALLWAY_ROW && to_row == HALLWAY_ROW)
        {
            //cout << pad_str << " source and destination are both in hallway. no move" << endl;
            return false;
        }
        
        // if leaving hallway, can only move from hallway to their respective room
        if (to_row != HALLWAY_ROW &&
            ((anthro == ANTHRO_A && to_col != COL_A) ||
             (anthro == ANTHRO_B && to_col != COL_B) ||
             (anthro == ANTHRO_C && to_col != COL_C) ||
             (anthro == ANTHRO_D && to_col != COL_D)))
        {
            //cout << pad_str << " leaving in hallway and destination is not the right anthro's room column. no move" << endl;
            return false;
        }

        steps = 0;
        // first step, if moving up to the hallway, handle that and increment steps
        
        if (from_row == BOTTOM_ROW)
        {
            if (board.layout[TOP_ROW][from_col] != OPEN)
            {
                //cout << pad_str << " source is bottom row, but top row is blocked. no move" << endl;
                return false;
            }
            steps = 2; // 2 steps to get to the hallway
        }
        if (from_row == TOP_ROW)
        {
            steps = 1; // 1 step to get ot the hallway
        }

        
        // room must be empty or only have another anthro of its type.
        if (to_row == BOTTOM_ROW)
        {
            if (board.layout[TOP_ROW][to_col] != OPEN)
            {
                //cout << pad_str << " destination is bottom row in room, but top row is occupied. no move" << endl;
                return false;
            }
            steps += 2; // 2 steps to get from hallway to bottom
        }
        if (to_row == TOP_ROW)
        {
            if (board.layout[BOTTOM_ROW][to_col] != anthro)
            {
                //cout << pad_str << " destination is top row in room, but bottom row is not the anthro. no move" << endl;
                return false;
            }
            steps += 1; // 1 step to get from hallway to top
        }
        // the room is clear. need to make sure path down hallway is clear
        int col_stepper = (from_col < to_col ? -1 : 1); // working backwards from the to_col to the from_col. 
        
        for (int col = to_col; col != from_col; col+=col_stepper)
        {
            if (board.layout[HALLWAY_ROW][col] != OPEN)
            {
                //cout << pad_str << " move blocked because of non-open space in hallway at column " << col << ". no move" << endl;
                return false;
            }
            steps++;
        }
        //cout << pad_str << " move is possible in " << steps << " steps" << endl;
        return true;
    }
    
    void Move::create_next_move(Move & next, int anthro, int to_row, int to_col, int steps)
    {
        // copy the board and current positions
        for (int i=0; i<5; i++)
        {
            strncpy(next.board.layout[i], board.layout[i], 14);
        }
        for (int i=0; i<NUM_ANTHROS; i++)
        {
            next.positions[i].row = positions[i].row;
            next.positions[i].col = positions[i].col;
            next.positions[i].anthro = positions[i].anthro;
        }
        
        // do the move
        next.board.layout[to_row][to_col] = positions[anthro].anthro;
        next.board.layout[positions[anthro].row][positions[anthro].col] = OPEN;
        next.positions[anthro].row = to_row;
        next.positions[anthro].col = to_col;
        
        // add the cost
        switch (next.positions[anthro].anthro)
        {
            case ANTHRO_A:
                next.cost = cost + (steps * COST_A);
                break;
            case ANTHRO_B:
                next.cost = cost + (steps * COST_B);
                break;
            case ANTHRO_C:
                next.cost = cost + (steps * COST_C);
                break;
            case ANTHRO_D:
                next.cost = cost + (steps * COST_D);
                break;
        }
        
        // set the new depth
        next.depth = depth + 1;
        
        // set the parent
        next.parent = this;
    }
    
    bool Move::has_matching_parent()
    {
        Move * current = parent;
        while (current != NULL)
        {
            for (int i=0; i<5; i++)
            {
                if (strncmp(board.layout[i], current->board.layout[i], 14) != 0)
                {
                    return false;
                }
            }
            current = current->parent;
        }
        cout << "matching parent found" << endl;
        return true;
    }
    
    vector<Move> Move::get_all_possible_moves()
    {
        string pad_str(depth, ' ');
        vector<Move> moves;
        for (int anthro=0; anthro<NUM_ANTHROS; anthro++)
        {
            if (!is_final(anthro))
            {
                Move next;
                int steps;
                // check all hallway moves
                for (int col=COL_HALLWAY_LEFT; col <= COL_HALLWAY_RIGHT; col++)
                {
                    if (can_move(anthro, HALLWAY_ROW, col, steps))
                    {
                        create_next_move(next, anthro, HALLWAY_ROW, col, steps);
                        if (next.has_matching_parent())
                        {
                            cout << pad_str << " move blocked because of matching parent found " << endl;
                        }
                        else
                        {
                            moves.push_back(next);
                        }
                    }
                }
                if (can_move(anthro, BOTTOM_ROW, COL_A, steps))
                {
                    create_next_move(next, anthro, BOTTOM_ROW, COL_A, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, BOTTOM_ROW, COL_B, steps))
                {
                    create_next_move(next, anthro, BOTTOM_ROW, COL_B, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, BOTTOM_ROW, COL_C, steps))
                {
                    create_next_move(next, anthro, BOTTOM_ROW, COL_C, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, BOTTOM_ROW, COL_D, steps))
                {
                    create_next_move(next, anthro, BOTTOM_ROW, COL_D, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, TOP_ROW, COL_A, steps))
                {
                    create_next_move(next, anthro, TOP_ROW, COL_A, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, TOP_ROW, COL_B, steps))
                {
                    create_next_move(next, anthro, TOP_ROW, COL_B, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, TOP_ROW, COL_C, steps))
                {
                    create_next_move(next, anthro, TOP_ROW, COL_C, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
                if (can_move(anthro, TOP_ROW, COL_D, steps))
                {
                    create_next_move(next, anthro, TOP_ROW, COL_D, steps);
                    if (next.has_matching_parent())
                    {
                        cout << pad_str << " move blocked because of matching parent found " << endl;
                    }
                    else
                    {
                        moves.push_back(next);
                    }
                }
            }
        }
        return moves;
    }
    
};

AocDay23::AocDay23():AocDay(23)
{
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

Move AocDay23::parse_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    
    Move move;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return move;
    }
    
    for (int i=0; i<3; i++)
    {
        strncpy(move.board.layout[i], lines[i].c_str(), 14);
    }
    
    for (int i=3; i<5; i++)
    {
        strncpy(move.board.layout[i], lines[i].c_str(), 12);
        move.board.layout[i][11]=' ';
        move.board.layout[i][12]=' ';
        move.board.layout[i][13]='\0';
    }
    
    move.positions[0].row=2;
    move.positions[0].col=3;
    move.positions[0].anthro=move.board.layout[2][3];
    move.positions[1].row=2;
    move.positions[1].col=5;
    move.positions[1].anthro=move.board.layout[2][5];
    move.positions[2].row=2;
    move.positions[2].col=7;
    move.positions[2].anthro=move.board.layout[2][7];
    move.positions[3].row=2;
    move.positions[3].col=9;
    move.positions[3].anthro=move.board.layout[2][9];
    move.positions[4].row=3;
    move.positions[4].col=3;
    move.positions[4].anthro=move.board.layout[3][3];
    move.positions[5].row=3;
    move.positions[5].col=5;
    move.positions[5].anthro=move.board.layout[3][5];
    move.positions[6].row=3;
    move.positions[6].col=7;
    move.positions[6].anthro=move.board.layout[3][7];
    move.positions[7].row=3;
    move.positions[7].col=9;
    move.positions[7].anthro=move.board.layout[3][9];
    
    for (int i=0; i<NUM_ANTHROS; i++)
    {
        cout << "Anthro " << i << " is at row "<< move.positions[i].row << ", column " << move.positions[i].col << ", with type " << move.positions[i].anthro << endl;
        if (move.is_final(i))
        {
            cout << "  This is in final position" << endl;
        }
    }    
    
    move.depth = 0;
    move.cost = 0;
    move.parent = NULL;
    move.board.display();
    return move;
    
    
}

void AocDay23::find_best_move_depth_first_search(Move parent, int & lowest)
{
    string pad_str(parent.depth, ' ');
    vector<Move> moves = parent.get_all_possible_moves();
    
    for (int i=0; i<moves.size(); i++)
    {
        //moves[i].board.display(moves[i].depth);
        if (moves[i].is_final())
        {
            //cout << pad_str << "FINAL MOVE DETECTED with cost " << moves[i].cost << endl;
            if (lowest == 0 || moves[i].cost < lowest)
            {
                lowest = moves[i].cost;
                cout << pad_str << "*** NEW BEST COST ***" << lowest << endl;
            }
        }
        else if (lowest == 0 || moves[i].cost < lowest)
        {
            find_best_move_depth_first_search(moves[i], lowest);
        }
    }
}

string AocDay23::part1(string filename, vector<string> extra_args)
{
    Move move = parse_input(filename);
    move.board.display();
    int lowest = 0;
    
    find_best_move_depth_first_search(move, lowest);
    
    ostringstream out;
    out << lowest;
    return out.str();
}

