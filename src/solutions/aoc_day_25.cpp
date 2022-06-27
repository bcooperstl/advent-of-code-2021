#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_25.h"
#include "file_utils.h"
#include "screen.h"

using namespace std;

AocDay25::AocDay25():AocDay(25)
{
}

AocDay25::~AocDay25()
{
}

vector<string> AocDay25::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> data;
    
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    
    return data;
}

int AocDay25::move_sea_cucumbers(Screen & from, Screen & to)
{
    int num_moved = 0;
    
    to = from;
    
    int num_cols = from.get_width();
    int num_rows = from.get_height();
    
    // moving EAST first
    for (int row=from.get_min_y(); row<=from.get_max_y(); row++)
    {
        for (int col=from.get_min_x(); col<=from.get_max_x(); col++)
        {
            if ((from.get(col, row) == EAST) && (from.get((col+1)%num_cols, row) == EMPTY)) // the east one at col,row can move to col+1,row
            {
                to.set((col+1)%num_cols, row, EAST);
                to.set(col, row, EMPTY);
                num_moved++;
            }
        }
    }
    
    from = to; // need to reset for all the ones moved EAST before considering how to move south;
    
    // moving SOUTH second
    for (int row=from.get_min_y(); row<=from.get_max_y(); row++)
    {
        for (int col=from.get_min_x(); col<=from.get_max_x(); col++)
        {
            if ((from.get(col, row) == SOUTH) && (from.get(col, (row+1)%num_rows) == EMPTY)) // the south one at col,row can move to col,row+1
            {
                to.set(col, (row+1)%num_rows, SOUTH);
                to.set(col, row, EMPTY);
                num_moved++;
            }
        }
    }
    
    return num_moved;
}

string AocDay25::part1(string filename, vector<string> extra_args)
{
    vector<string> input = read_input(filename);
    
    Screen current, next;
    
    current.load(input);
    cout << "Initial layout is" << endl;
    current.display();
    
    int step_counter = 0;
    int num_moved = 0;
    do
    {
        num_moved = move_sea_cucumbers(current, next);
        current = next;
        step_counter++;
        cout << "At step " << step_counter << " there were " << num_moved << " sea cucumbers moved" << endl;
        current.display();
    } while (num_moved != 0);
    
    cout << "Final layout is" << endl;
    current.display();
    
    ostringstream out;
    out << step_counter;
    return out.str();
}

