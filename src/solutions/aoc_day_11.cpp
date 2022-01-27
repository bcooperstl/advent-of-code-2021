#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include "aoc_day_11.h"
#include "file_utils.h"

using namespace std;
using namespace Day11;

#define PART_1_STEPS 100

namespace Day11
{
    Grid::Grid(vector<string> energy)
    {
        for (int row=0; row<SIDE_LENGTH; row++)
        {
            for (int col=0; col<SIDE_LENGTH; col++)
            {
                m_energy[row][col]=energy[row][col]-'0';
            }
        }
    }

    Grid::~Grid()
    {
    }
    
    void Grid::dump()
    {
        for (int row=0; row<SIDE_LENGTH; row++)
        {
            for (int col=0; col<SIDE_LENGTH; col++)
            {
                cout << (int)m_energy[row][col];
            }
            cout << endl;
        }
        cout << endl;
    }
    
    int Grid::run_step()
    {
        char flash_queue[SIDE_LENGTH*SIDE_LENGTH][2];
        bool in_flash_queue[SIDE_LENGTH][SIDE_LENGTH];
        int insert_pos=0;
        int work_pos=0;
        for (int row=0; row<SIDE_LENGTH; row++)
        {
            for (int col=0; col<SIDE_LENGTH; col++)
            {
                in_flash_queue[row][col] = false;
                m_energy[row][col]++;
                if (m_energy[row][col] > FLASH_THRESHOLD)
                {
                    //cout << "queueing at " << row << "," << col << endl;
                    flash_queue[insert_pos][ROW] = row;
                    flash_queue[insert_pos][COL] = col;
                    insert_pos++;
                    in_flash_queue[row][col] = true;
                }
            }
        }
        while (work_pos < insert_pos)
        {
            int row=flash_queue[work_pos][ROW];
            int col=flash_queue[work_pos][COL];
            //cout << "processing neighbors of " << row << "," << col << endl;
            // above row
            if (row > 0)
            {
                // center
                m_energy[row-1][col]++;
                if (m_energy[row-1][col] > FLASH_THRESHOLD && (!in_flash_queue[row-1][col]))
                {
                    
                    flash_queue[insert_pos][ROW] = row-1;
                    flash_queue[insert_pos][COL] = col;
                    insert_pos++;
                    in_flash_queue[row-1][col] = true;
                }
                
                // left
                if (col > 0)
                {
                    m_energy[row-1][col-1]++;
                    if (m_energy[row-1][col-1] > FLASH_THRESHOLD && (!in_flash_queue[row-1][col-1]))
                    {
                        flash_queue[insert_pos][ROW] = row-1;
                        flash_queue[insert_pos][COL] = col-1;
                        insert_pos++;
                        in_flash_queue[row-1][col-1] = true;
                    }
                }

                // right
                if (col < (SIDE_LENGTH - 1))
                {
                    m_energy[row-1][col+1]++;
                    if (m_energy[row-1][col+1] > FLASH_THRESHOLD && (!in_flash_queue[row-1][col+1]))
                    {
                        flash_queue[insert_pos][ROW] = row-1;
                        flash_queue[insert_pos][COL] = col+1;
                        insert_pos++;
                        in_flash_queue[row-1][col+1] = true;
                    }
                }
            }
                
            // below row
            if (row < (SIDE_LENGTH -1))
            {
                // center
                m_energy[row+1][col]++;
                if (m_energy[row+1][col] > FLASH_THRESHOLD && (!in_flash_queue[row+1][col]))
                {
                    flash_queue[insert_pos][ROW] = row+1;
                    flash_queue[insert_pos][COL] = col;
                    insert_pos++;
                    in_flash_queue[row+1][col] = true;
                }
                
                // left
                if (col > 0)
                {
                    m_energy[row+1][col-1]++;
                    if (m_energy[row+1][col-1] > FLASH_THRESHOLD && (!in_flash_queue[row+1][col-1]))
                    {
                        flash_queue[insert_pos][ROW] = row+1;
                        flash_queue[insert_pos][COL] = col-1;
                        insert_pos++;
                        in_flash_queue[row+1][col-1] = true;
                    }
                }

                // right
                if (col < (SIDE_LENGTH - 1))
                {
                    m_energy[row+1][col+1]++;
                    if (m_energy[row+1][col+1] > FLASH_THRESHOLD && (!in_flash_queue[row+1][col+1]))
                    {
                        flash_queue[insert_pos][ROW] = row+1;
                        flash_queue[insert_pos][COL] = col+1;
                        insert_pos++;
                        in_flash_queue[row+1][col+1] = true;
                    }
                }
            }
            
            // same row
            // left
            if (col > 0)
            {
                m_energy[row][col-1]++;
                if (m_energy[row][col-1] > FLASH_THRESHOLD && (!in_flash_queue[row][col-1]))
                {
                    flash_queue[insert_pos][ROW] = row;
                    flash_queue[insert_pos][COL] = col-1;
                    insert_pos++;
                    in_flash_queue[row][col-1] = true;
                }
            }

            // right
            if (col < (SIDE_LENGTH - 1))
            {
                m_energy[row][col+1]++;
                if (m_energy[row][col+1] > FLASH_THRESHOLD && (!in_flash_queue[row][col+1]))
                {
                    flash_queue[insert_pos][ROW] = row;
                    flash_queue[insert_pos][COL] = col+1;
                    insert_pos++;
                    in_flash_queue[row][col+1] = true;
                }
            }
            work_pos++;
        }
        
        for (int row=0; row<SIDE_LENGTH; row++)
        {
            for (int col=0; col<SIDE_LENGTH; col++)
            {
                if (m_energy[row][col] > FLASH_THRESHOLD)
                {
                    m_energy[row][col] = 0;
                }
            }
        }
        
        return insert_pos;
    }
    
    long Grid::run_steps (int num_steps)
    {
        long total_flashes = 0;
        for (int i=0; i<num_steps; i++)
        {
            long current_flashes = run_step();
            total_flashes += current_flashes;
            cout << "Step " << i+1 << " had " << current_flashes << " bringing the total up to " << total_flashes << endl;
            dump();
        }
        
        return total_flashes;
    }
};

AocDay11::AocDay11():AocDay(11)
{
}

AocDay11::~AocDay11()
{
}

vector<string> AocDay11::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
        
    return lines;
}


string AocDay11::part1(string filename, vector<string> extra_args)
{
    vector<string> input = read_input(filename);
    Grid grid(input);
    grid.dump();
    
    long total_steps = grid.run_steps(PART_1_STEPS);
    
    ostringstream out;
    out << total_steps;
    return out.str();
}
