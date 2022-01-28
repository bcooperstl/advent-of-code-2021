#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include "aoc_day_12.h"
#include "file_utils.h"

using namespace std;
using namespace Day12;

namespace Day12
{
    Cave::Cave(string name)
    {
        m_name = name;
    }
    
    Cave::~Cave()
    {
    }
    
    bool Cave::is_big()
    {
        return false;
    }
    
    bool Cave::is_small()
    {
        return false;
    }
    
    bool Cave::is_start()
    {
        return false;
    }
    
    bool Cave::is_end()
    {
        return false;
    }
    
    string Cave::get_name()
    {
        return m_name;
    }
    
    vector<Cave *> Cave::get_neighbors()
    {
        return m_neighbors;
    }
    
    void Cave::add_neighbor(Cave * cave)
    {
        m_neighbors.push_back(cave);
    }
    
    StartCave::StartCave(string name):Cave(name)
    {
    }
    
    StartCave::~StartCave()
    {
    }
    
    bool StartCave::is_start()
    {
        return true;
    }
    
    EndCave::EndCave(string name):Cave(name)
    {
    }
    
    EndCave::~EndCave()
    {
    }
    
    bool EndCave::is_end()
    {
        return true;
    }
    
    BigCave::BigCave(string name):Cave(name)
    {
    }
    
    BigCave::~BigCave()
    {
    }
    
    bool BigCave::is_big()
    {
        return true;
    }
    
    LittleCave::LittleCave(string name):Cave(name)
    {
    }
    
    LittleCave::~LittleCave()
    {
    }
    
    bool LittleCave::is_little()
    {
        return true;
    }
    
    Path::Path()
    {
        m_next_path = 0;
    }
    
    Path::~Path()
    {
    }
    
    bool Path::can_visit(Cave * next)
    {
        if (next->is_small()) // can only visit small caves if they haven't been visited
        {
            for (int i=0; i<m_next_path; i++)
            {
                if (m_path[i] == next)
                {
                    return false;
                }
            }
        }
        return true; // no other restrictions
    }
    
    void Path::visit(Cave * next)
    {
        m_path[m_next_path] = next;
        m_next_path++;
    }
    
    bool Path::is_complete()
    {
        // must have at least 2 items to be complete - start and end
        if (m_next_path < 2)
        {
            return false;
        }
        return (m_path[0]->is_start() && m_path[m_next_path-1]->is_end());
    }
    
    Cave * Path::get_current_cave()
    {
        if (m_next_path == 0)
        {
            return NULL;
        }
        return m_path[m_next_path-1];
    }
    
    void Path::dump()
    {
        for (int i=0; i<m_next_path; i++)
        {
            cout << m_path[i]->get_name();
            if (i != m_next_path-1)
            {
                cout << "-";
            }
        }
        cout << endl;
    }
};

AocDay12::AocDay12():AocDay(12)
{
}

AocDay12::~AocDay12()
{
}

vector<vector<string>> AocDay12::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    
    if (!fileutils.read_as_list_of_split_strings(filename, data, '-', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
        
    return data;
}


string AocDay12::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> input = read_input(filename);
    
    ostringstream out;
    out << "";
    return out.str();
}

