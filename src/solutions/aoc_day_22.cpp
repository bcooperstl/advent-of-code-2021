#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_22.h"
#include "file_utils.h"

using namespace std;
using namespace Day22;

namespace Day22
{
    Row::Row()
    {
        m_head_pair = NULL;
    }
    
    Row::~Row()
    {
        OnPair * current = m_head_pair;
        OnPair * next;
        while (current != NULL)
        {
            next = current->next;
            delete current;
            current = next;
        }
    }
    
    void Row::turn_on(int min_x, int max_x)
    {
        cout << "    Turning on from " << min_x << " to " << max_x << endl;
        cout << "    BEFORE: ";
        display();
        
        cout << "    AFTER: ";
        display();
        
    }
    
    void Row::turn_off(int min_x, int max_x)
    {
        cout << "    Turning off from " << min_x << " to " << max_x << endl;
        cout << "    BEFORE: ";
        display();
        
        cout << "    AFTER: ";
        display();
    
    }
    
    long Row::get_count_on()
    {
        long count = 0;
        OnPair * current = m_head_pair;
        while (current != NULL)
        {
            count += (long)(current->max - current->min + 1); // need to add 1 to get the missing end
            current = current->next;
        }
        return count;
    }
    
    void Row::display()
    {
        if (m_head_pair == NULL)
        {
            cout << "   Empty row" << endl;
        }
        else
        {
            OnPair * current = m_head_pair;
            while (current != NULL)
            {
                cout << "   [" << current->min << "," << current->max << "]  " << endl;
            }
        }
    }
    
    Plane::Plane()
    {
    }
    
    Plane::~Plane()
    {
        map<int, Row *>::iterator pos = m_rows.begin();
        while (pos != m_rows.end())
        {
            delete pos->second;
            ++pos;
        }
    }
    
    void Plane::turn_on(int min_y, int max_y, int min_x, int max_x)
    {
        cout << " Turning on with rows y from " << min_y << " to " << max_y << endl;
        for (int y=min_y; y<=max_y; y++)
        {
            map<int, Row *>::iterator row_iter = m_rows.find(y);
            Row * row;
            if (row_iter == m_rows.end())
            {
                cout << "  Creating row y=" << y << endl;
                row = new Row();
                m_rows[y] = row;
            }
            else
            {
                cout << "  Using existing row y=" << y << endl;
                row = row_iter->second;
            }
            row->turn_on(min_x, max_x);
        }
    }
    
    void Plane::turn_off(int min_y, int max_y, int min_x, int max_x)
    {
        for (int y=min_y; y<=max_y; y++)
        {
            map<int, Row *>::iterator row_iter = m_rows.find(y);
            if (row_iter != m_rows.end())
            {
                cout << "  Turning off row y=" << y << endl;
                row_iter->second->turn_off(min_x, max_x);
            }
            else
            {
                cout << "  Skipping missing row y=" << y << endl;
            }
        }
    }
    
    long Plane::get_count_on()
    {
        long count = 0;
        map<int, Row *>::iterator pos = m_rows.begin();
        while (pos != m_rows.end())
        {
            count+=pos->second->get_count_on();
            ++pos;
        }
        return count;
    }
    
    Space::Space()
    {
    }
    
    Space::~Space()
    {
        map<int, Plane *>::iterator pos = m_planes.begin();
        while (pos != m_planes.end())
        {
            delete pos->second;
            ++pos;
        }
    }
    
    void Space::turn_on(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x)
    {
        cout << "Turning on with planes z from " << min_z << " to " << max_z << endl;
        for (int z=min_z; z<=max_z; z++)
        {
            map<int, Plane *>::iterator plane_iter = m_planes.find(z);
            Plane * plane;
            if (plane_iter == m_planes.end())
            {
                cout << " Creating plane z=" << z << endl;
                plane = new Plane();
                m_planes[z] = plane;
            }
            else
            {
                cout << " Using existing plane z=" << z << endl;
                plane = plane_iter->second;
            }
            plane->turn_on(min_y, max_y, min_x, max_x);
        }
    }
    
    void Space::turn_off(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x)
    {
        cout << "Turning off with planes z from " << min_z << " to " << max_z << endl;
        for (int z=min_z; z<=max_z; z++)
        {
            map<int, Plane *>::iterator plane_iter = m_planes.find(z);
            if (plane_iter != m_planes.end())
            {
                cout << " Turning off plane z=" << z << endl;
                plane_iter->second->turn_off(min_y, max_y, min_x, max_x);
            }
            else
            {
                cout << " Skipping missing plane z=" << z << endl;
            }
        }
    }
    
    long Space::get_count_on()
    {
        long count = 0;
        map<int, Plane *>::iterator pos = m_planes.begin();
        while (pos != m_planes.end())
        {
            count+=pos->second->get_count_on();
            ++pos;
        }
        return count;
    }
        
};

AocDay22::AocDay22():AocDay(22)
{
}

AocDay22::~AocDay22()
{
}

/*
off x=9..11,y=9..11,z=9..11
*/

vector<Step> AocDay22::parse_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    vector<Step> steps;
    
    char delimiters[4];
    delimiters[0] = ' ';
    delimiters[1] = '.';
    delimiters[2] = ',';
    delimiters[3] = '=';
    
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delimiters, 4, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return steps;
    }
    
    for (int i=0; i<lines.size(); i++)
    {
        Step step;
        step.method = lines[i][0];
        step.min_x = strtol(lines[i][2].c_str(), NULL, 10);
        step.max_x = strtol(lines[i][3].c_str(), NULL, 10);
        step.min_y = strtol(lines[i][5].c_str(), NULL, 10);
        step.max_y = strtol(lines[i][6].c_str(), NULL, 10);
        step.min_z = strtol(lines[i][8].c_str(), NULL, 10);
        step.max_z = strtol(lines[i][9].c_str(), NULL, 10);
        steps.push_back(step);
    }
    return steps;
}

string AocDay22::part1(string filename, vector<string> extra_args)
{
    Space space;
    vector<Step> steps = parse_input(filename);
    
    for (int i=0; i<steps.size(); i++)
    {
        if (steps[i].min_x >= -50 && steps[i].max_x <= 50 &&
            steps[i].min_y >= -50 && steps[i].max_y <= 50 &&
            steps[i].min_z >= -50 && steps[i].max_z <= 50)
        {
            if (steps[i].method == "on")
            {
                space.turn_on(steps[i].min_z, steps[i].max_z, steps[i].min_y, steps[i].max_y, steps[i].min_x, steps[i].max_x);
            }
            else
            {
                space.turn_off(steps[i].min_z, steps[i].max_z, steps[i].min_y, steps[i].max_y, steps[i].min_x, steps[i].max_x);
            }
        }
    }
            
    ostringstream out;
    out << space.get_count_on();
    return out.str();
}
