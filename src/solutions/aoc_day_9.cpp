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
