#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_12.h"
#include "file_utils.h"

#define NAME_START "start"
#define NAME_END "end"

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
    
    SmallCave::SmallCave(string name):Cave(name)
    {
    }
    
    SmallCave::~SmallCave()
    {
    }
    
    bool SmallCave::is_small()
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

void AocDay12::find_paths(Path & current_path, vector<Path> & completed_paths)
{
    cout << "Find_paths for ";
    current_path.dump();
    
    if (current_path.is_complete())
    {
        completed_paths.push_back(current_path);
        cout << " Adding completed path ";
        current_path.dump();
        return;
    }
    Cave * current_cave = current_path.get_current_cave();
    vector<Cave *> neighbors = current_cave->get_neighbors();
    for (int i=0; i<neighbors.size(); i++)
    {
        if (current_path.can_visit(neighbors[i]))
        {
            Path next_path(current_path);
            next_path.visit(neighbors[i]);
            find_paths(next_path, completed_paths);
        }
    }
    return;
}

Cave * AocDay12::create_cave(string name)
{
    if (name == NAME_START)
    {
        return new StartCave(name);
    }
    
    if (name == NAME_END)
    {
        return new EndCave(name);
    }
    
    if (isupper(name[0]))
    {
        return new BigCave(name);
    }
    
    return new SmallCave(name);
}

string AocDay12::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> input = read_input(filename);
    
    map<string, Cave *> lookups;
    vector<Cave *> caves;
    
    for (int i=0; i<input.size(); i++)
    {
        cout << "Processing " << input[i][0] << "-" << input[i][1] << endl;
        Cave * left;
        Cave * right;
        map<string, Cave *>::iterator it = lookups.find(input[i][0]);
        if (it == lookups.end()) // not in map
        {
            left = create_cave(input[i][0]);
            lookups[input[i][0]]=left;
            caves.push_back(left);
        }
        else
        {
            left = it->second;
        }
        
        it = lookups.find(input[i][1]);
        if (it == lookups.end()) // not in map
        {
            right = create_cave(input[i][1]);
            lookups[input[i][1]]=right;
            caves.push_back(right);
        }
        else
        {
            right = it->second;
        }
        
        if (left->is_start())
        {
            left->add_neighbor(right);
            cout << "Mapping " << left->get_name() << " --> " << right->get_name() << endl;
        }
        else if (right->is_start())
        {
            right->add_neighbor(left);
            cout << "Mapping " << right->get_name() << " --> " << left->get_name() << endl;
        }
        else if (left->is_end())
        {
            right->add_neighbor(left);
            cout << "Mapping " << right->get_name() << " --> " << left->get_name() << endl;
        }
        else if (right->is_end())
        {
            left->add_neighbor(right);
            cout << "Mapping " << left->get_name() << " --> " << right->get_name() << endl;
        }
        else // both are some combination of big and small
        {
            left->add_neighbor(right);
            right->add_neighbor(left);
            cout << "Mapping " << left->get_name() << " --> " << right->get_name() << endl;
            cout << "Mapping " << right->get_name() << " --> " << left->get_name() << endl;
        }
    }
    
    vector<Path> completed_paths;
    Path start_path;
    start_path.visit(lookups[NAME_START]);
    
    find_paths(start_path, completed_paths);
        
    ostringstream out;
    out << completed_paths.size();
    
    for (int i=0; i<caves.size(); i++)
    {
        delete caves[i];
    }
    
    return out.str();
}

