#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <climits>

#include "aoc_day_15.h"
#include "file_utils.h"

using namespace std;
using namespace Day15;

namespace Day15
{
    Cavern::Cavern(vector<string> risks)
    {
        m_rows = risks.size();
        m_cols = risks[0].length();
        for (int row=0; row<m_rows; row++)
        {
            for (int col=0; col<m_cols; col++)
            {
                m_points[row][col].risk_level=risks[row][col]-'0';
            }
        }
    }

    Cavern::~Cavern()
    {
    }
    
    int Cavern::get_rows()
    {
        return m_rows;
    }
    
    int Cavern::get_cols()
    {
        return m_cols;
    }
    
    int Cavern::get_min_distance(int row, int col)
    {
        return m_points[row][col].min_risk_to_here;
    }

    void Cavern::init_dijkstra()
    {
        for (int row=0; row<m_rows; row++)
        {
            for (int col=0; col<m_cols; col++)
            {
                m_points[row][col].visited = false;
                m_points[row][col].min_risk_to_here = INT_MAX;
                m_points[row][col].from_direction = DIR_NONE;
            }
        }
    }
        
    // return a non-visited point with the lowest min_risk_to_here value
    bool Cavern::find_next_to_eval(int & next_row, int & next_col)
    {
        int min_found_risk = INT_MAX;
        for (int row=0; row<m_rows; row++)
        {
            for (int col=0; col<m_cols; col++)
            {
                if ((!m_points[row][col].visited) && 
                    (m_points[row][col].min_risk_to_here < min_found_risk))
                {
                    min_found_risk = m_points[row][col].min_risk_to_here;
                    next_row = row;
                    next_col = col;
                }
            }
        }
        return (min_found_risk != INT_MAX); // true if one found; false if not
    }
    
    void Cavern::run_dijkstra(int start_row, int start_col)
    {
        init_dijkstra();
        int row = start_row;
        int col = start_col;
        m_points[row][col].visited = true;
        m_points[row][col].min_risk_to_here = 0;
        m_points[row][col].from_direction = DIR_NONE;
        do
        {
            cout << "Running on " << row << ", " << col << endl;
            
            // check north
            if (row > 0)
            {
                if ((!m_points[row-1][col].visited) &&
                    (m_points[row-1][col].min_risk_to_here > (m_points[row][col].min_risk_to_here + m_points[row-1][col].risk_level)))
                {
                    m_points[row-1][col].min_risk_to_here = m_points[row][col].min_risk_to_here + m_points[row-1][col].risk_level;
                    m_points[row-1][col].from_direction = DIR_SOUTH; // coming from the south
                    cout << " setting " << m_points[row-1][col].min_risk_to_here << " on north at " << row-1 << ", " << col << endl;
                }
            }
            // check south
            if (row < (m_rows - 1))
            {
                if ((!m_points[row+1][col].visited) &&
                    (m_points[row+1][col].min_risk_to_here > (m_points[row][col].min_risk_to_here + m_points[row+1][col].risk_level)))
                {
                    m_points[row+1][col].min_risk_to_here = m_points[row][col].min_risk_to_here + m_points[row+1][col].risk_level;
                    m_points[row+1][col].from_direction = DIR_NORTH; // coming from the north
                    cout << " setting " << m_points[row+1][col].min_risk_to_here << " on south at " << row+1 << ", " << col << endl;
                }
            }
            // check west
            if (col > 0)
            {
                if ((!m_points[row][col-1].visited) &&
                    (m_points[row][col-1].min_risk_to_here > (m_points[row][col].min_risk_to_here + m_points[row][col-1].risk_level)))
                {
                    m_points[row][col-1].min_risk_to_here = m_points[row][col].min_risk_to_here + m_points[row][col-1].risk_level;
                    m_points[row][col-1].from_direction = DIR_EAST; // coming from the east
                    cout << " setting " << m_points[row][col-1].min_risk_to_here << " on west at " << row << ", " << col-1 << endl;
                }
            }
            // check east
            if (col < (m_cols - 1))
            {
                if ((!m_points[row][col+1].visited) &&
                    (m_points[row][col+1].min_risk_to_here > (m_points[row][col].min_risk_to_here + m_points[row][col+1].risk_level)))
                {
                    m_points[row][col+1].min_risk_to_here = m_points[row][col].min_risk_to_here + m_points[row][col+1].risk_level;
                    m_points[row][col+1].from_direction = DIR_WEST; // coming from the west
                    cout << " setting " << m_points[row][col+1].min_risk_to_here << " on west at " << row << ", " << col+1 << endl;
                }
            }
            
            m_points[row][col].visited = true;
        } while (find_next_to_eval(row, col));
    }
};

AocDay15::AocDay15():AocDay(15)
{
}

AocDay15::~AocDay15()
{
}

vector<string> AocDay15::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
        
    return lines;
}


string AocDay15::part1(string filename, vector<string> extra_args)
{
    vector<string> input = read_input(filename);
    Cavern cavern(input);
    
    cavern.run_dijkstra(0,0);
    
    ostringstream out;
    out << cavern.get_min_distance(cavern.get_rows()-1, cavern.get_cols()-1);
    return out.str();
}
