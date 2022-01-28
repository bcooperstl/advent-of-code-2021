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
    
    bool Path::can_visit(Cave * next, bool with_one_small_repeat)
    {
        if (!with_one_small_repeat)
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
        else
        {
            if (next->is_small()) // assuming path is already valid, so if the new one is not a small, we are good
            {
                Cave * small_caves[32];
                int small_cave_counter[32];
                int small_cave_pos = 0;
                bool already_has_two = false;
                for (int i=0; i<m_next_path; i++)
                {
                    if (m_path[i]->is_small())
                    {
                        bool small_found = false;
                        
                        for (int j=0; j<small_cave_pos; j++)
                        {
                            if (small_caves[j] == m_path[i])
                            {
                                small_cave_counter[j]++;
                                small_found = true;
                                already_has_two = true;
                            }
                        }
                        if (!small_found)
                        {
                            small_caves[small_cave_pos] = m_path[i];
                            small_cave_counter[small_cave_pos] = 1;
                            small_cave_pos++;
                        }
                    }
                }
                if (!already_has_two) // short circuit - if none have two, we are good
                {
                    return true;
                }
                // at this point, one item is already in there twice. 
                // We need to make sure that the new small cave has not yet been visited, 
                // or else it would now be a second one visited twice
                for (int i=0; i<small_cave_pos; i++)
                {
                    if (small_caves[i] == next)
                    {
                        return false;
                    }
                }
                // not a match, good to add.
                return true;
            }
        }
        return true;
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

void AocDay12::find_paths(Path & current_path, vector<Path> & completed_paths, bool with_one_small_repeat)
{
    //cout << "Find_pathms for ";
    //current_path.dump();
    
    if (current_path.is_complete())
    {
        completed_paths.push_back(current_path);
        //cout << " Adding completed path ";
        //current_path.dump();
        return;
    }
    Cave * current_cave = current_path.get_current_cave();
    vector<Cave *> neighbors = current_cave->get_neighbors();
    for (int i=0; i<neighbors.size(); i++)
    {
        if (current_path.can_visit(neighbors[i], with_one_small_repeat))
        {
            Path next_path(current_path);
            next_path.visit(neighbors[i]);
            find_paths(next_path, completed_paths, with_one_small_repeat);
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
    
    find_paths(start_path, completed_paths, false);
        
    ostringstream out;
    out << completed_paths.size();
    
    for (int i=0; i<caves.size(); i++)
    {
        delete caves[i];
    }
    
    return out.str();
}

string AocDay12::part2(string filename, vector<string> extra_args)
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
    
    find_paths(start_path, completed_paths, true);
        
    ostringstream out;
    out << completed_paths.size();
    
    for (int i=0; i<caves.size(); i++)
    {
        delete caves[i];
    }
    
    return out.str();
}
