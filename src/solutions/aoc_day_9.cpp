#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_9.h"
#include "file_utils.h"

using namespace std;

AocDay9::AocDay9():AocDay(9)
{
}

AocDay9::~AocDay9()
{
}

void AocDay9::parse_input(string filename, char (&heightmap)[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE], int &num_rows, int & num_cols)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    num_rows = lines.size();
    num_cols = lines[0].length();
    for (int i=0; i<num_rows; i++)
    {
        for (int j=0; j<num_cols; j++)
        {
            heightmap[i][j] = lines[i][j]-'0';
        }
    }
    return;
}

void AocDay9::clear_basin_members(bool (&basinmap)[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE])
{
    for (int i=0; i<MAX_HEIGHTMAP_EDGE; i++)
    {
        for (int j=0; j<MAX_HEIGHTMAP_EDGE; j++)
        {
            basinmap[i][j] = false;
        }
    }
}

int AocDay9::count_basin_members(bool (&basinmap)[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE])
{
    int count = 0;
    for (int i=0; i<MAX_HEIGHTMAP_EDGE; i++)
    {
        for (int j=0; j<MAX_HEIGHTMAP_EDGE; j++)
        {
            if (basinmap[i][j])
            {
                count++;
            }
        }
    }
    return count;
}

void AocDay9::check_neighbors(char (&heightmap)[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE], bool (&basinmap)[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE], int num_rows, int num_cols, int row, int col)
{
    // check north (row-1, col)
    if (row > 0)
    {
        if ((!basinmap[row-1][col]) &&
            (heightmap[row-1][col] > heightmap[row][col]) &&
            (heightmap[row-1][col] < 9))
        {
            basinmap[row-1][col] = true;
            check_neighbors(heightmap, basinmap, num_rows, num_cols, row-1, col);
        }
    }

    // check south (row+1, col)
    if (row < (num_rows - 1))
    {
        if ((!basinmap[row+1][col]) &&
            (heightmap[row+1][col] > heightmap[row][col]) &&
            (heightmap[row+1][col] < 9))
        {
            basinmap[row+1][col] = true;
            check_neighbors(heightmap, basinmap, num_rows, num_cols, row+1, col);
        }
    }
    
    // check west (row, col-1)
    if (col > 0)
    {
        if ((!basinmap[row][col-1]) &&
            (heightmap[row][col-1] > heightmap[row][col]) &&
            (heightmap[row][col-1] < 9))
        {
            basinmap[row][col-1] = true;
            check_neighbors(heightmap, basinmap, num_rows, num_cols, row, col-1);
        }
    }

    // check east (row, col+1)
    if (col < (num_cols - 1))
    {
        if ((!basinmap[row][col+1]) &&
            (heightmap[row][col+1] > heightmap[row][col]) &&
            (heightmap[row][col+1] < 9))
        {
            basinmap[row][col+1] = true;
            check_neighbors(heightmap, basinmap, num_rows, num_cols, row, col+1);
        }
    }
    return;
}

string AocDay9::part1(string filename, vector<string> extra_args)
{
    char heightmap[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE];
    int num_rows, num_cols;
    
    parse_input(filename, heightmap, num_rows, num_cols);
    
    int risk_sum = 0;
    
    int max_row = num_rows-1;
    int max_col = num_cols-1;
    
    for (int i=0; i<num_rows; i++)
    {
        for (int j=0; j<num_cols; j++)
        {
            if ((i == 0 || heightmap[i][j] < heightmap[i-1][j]) && // check top, unless at row 0
                (i == max_row || heightmap[i][j] < heightmap[i+1][j]) && // check bottom, unless at max_row
                (j == 0 || heightmap[i][j] < heightmap[i][j-1]) && // check left, unless at col 0
                (j == max_col || heightmap[i][j] < heightmap[i][j+1])) // check right, unless at max_col
            {
                cout << "Lowpoint found at row=" << i << " col=" << j << " with value=" << (int)heightmap[i][j] << endl;
                risk_sum += (1 + heightmap[i][j]);
            }
        }
    }
    
    ostringstream out;
    out << risk_sum;
    return out.str();
}

string AocDay9::part2(string filename, vector<string> extra_args)
{
    char heightmap[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE];
    bool basinmap[MAX_HEIGHTMAP_EDGE][MAX_HEIGHTMAP_EDGE];
    int num_rows, num_cols;
    
    parse_input(filename, heightmap, num_rows, num_cols);
    
    int max_row = num_rows-1;
    int max_col = num_cols-1;
        
    int largest_basins[3] = {0,0,0};
    
    for (int i=0; i<num_rows; i++)
    {
        for (int j=0; j<num_cols; j++)
        {
            if ((i == 0 || heightmap[i][j] < heightmap[i-1][j]) && // check top, unless at row 0
                (i == max_row || heightmap[i][j] < heightmap[i+1][j]) && // check bottom, unless at max_row
                (j == 0 || heightmap[i][j] < heightmap[i][j-1]) && // check left, unless at col 0
                (j == max_col || heightmap[i][j] < heightmap[i][j+1])) // check right, unless at max_col
            {
                clear_basin_members(basinmap);
                basinmap[i][j] = true;
                check_neighbors(heightmap, basinmap, num_rows, num_cols, i, j);
                int size = count_basin_members(basinmap);
                cout << "Lowpoint found at row=" << i << " col=" << j << " with size=" << size << endl;
                if (size > largest_basins[0])
                {
                    largest_basins[2] = largest_basins[1];
                    largest_basins[1] = largest_basins[0];
                    largest_basins[0] = size;
                }
                else if (size > largest_basins[1])
                {
                    largest_basins[2] = largest_basins[1];
                    largest_basins[1] = size;
                }
                else if (size > largest_basins[2])
                {
                    largest_basins[2] = size;
                }                
                cout << " The three largest basins are " << largest_basins[0] << " " << largest_basins[1] << " " << largest_basins[2] << endl;
            }
        }
    }
    
    ostringstream out;
    out << (largest_basins[0] * largest_basins[1] * largest_basins[2]);
    return out.str();
}
