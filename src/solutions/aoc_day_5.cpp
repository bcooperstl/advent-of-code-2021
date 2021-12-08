#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_5.h"
#include "file_utils.h"

using namespace std;
using namespace Day5;

namespace Day5
{
    bool LineSegment::is_horizontal_or_vertical()
    {
        return ((x1 == x2) || (y1 == y2));
    }
    
    Map::Map()
    {
        for (int y=0; y<MAP_SIZE; y++)
        {
            for (int x=0; x<MAP_SIZE; x++)
            {
                m_map[y][x] = 0;
            }
        }
    }
    
    Map::~Map()
    {
    }
    
    void Map::display()
    {
        for (int y=0; y<MAP_SIZE; y++)
        {
            for (int x=0; x<MAP_SIZE; x++)
            {
                if (m_map[y][x]==0)
                {
                    cout << '.';
                }
                else if (m_map[y][x]>9)
                {
                    cout << '*';
                }
                else
                {
                    cout << m_map[y][x];
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    
    void Map::map_segment(LineSegment segment)
    {
        int delta_x = 0;
        int delta_y = 0;
        int x = segment.x1;
        int y = segment.y1;
        
        // Vertical line
        if (segment.x1 == segment.x2)
        {
            if (segment.y2 > segment.y1)
            {
                delta_y = 1; // going down, so increment the y each time
            }
            else
            {
                delta_y = -1; // goign up, so decrement the y each time
            }
        }
        
        // Horizontal line
        {
            if (segment.y1 == segment.y2)
            {
                if (segment.x2 > segment.x1)
                {
                    delta_x = 1; // going right, so increment the x each time
                }
                else
                {    
                    delta_x = -1; // going left, so decrement the x each time
                }
            }
        }
        
        cout << "Segment " << segment.x1 << "," << segment.y1 << " to " 
                           << segment.x2 << "," << segment.y2 << " delta " 
                           << delta_x << "," << delta_y << endl;
        
        while (!((x == segment.x2) && (y == segment.y2)))
        {
            // mark the point
            m_map[y][x]++;
            
            // apply the delta
            x+=delta_x;
            y+=delta_y;
        }
        
        // mark the last point
        m_map[y][x]++;
        
    }
    
    int Map::count_dangerous()
    {
        int count = 0;
        for (int y=0; y<MAP_SIZE; y++)
        {
            for (int x=0; x<MAP_SIZE; x++)
            {
                if (m_map[y][x] >= 2)
                {
                    count++;
                }
            }
        }
        return count;
    }
};

AocDay5::AocDay5():AocDay(5)
{
}

AocDay5::~AocDay5()
{
}

void AocDay5::parse_input(string filename, vector<LineSegment> & line_segments)
{
    FileUtils fileutils;
    vector<vector<long>> lines;
    
    line_segments.clear();
    
    char delims[4];
    delims[0]=' ';
    delims[1]=',';
    delims[2]='-';
    delims[3]='>';
    
    if (!fileutils.read_as_list_of_split_longs(filename, lines, delims, 4, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
        
    for (int i=0; i<lines.size(); i++)
    {
        LineSegment ls;
        ls.x1 = lines[i][0];
        ls.y1 = lines[i][1];
        ls.x2 = lines[i][2];
        ls.y2 = lines[i][3];
        
        line_segments.push_back(ls);
    }
    
    return;
}

string AocDay5::part1(string filename, vector<string> extra_args)
{
    vector<LineSegment> vents;
    
    parse_input(filename, vents);
    
    Map map;
    
    //map.display();
    
    for (int i=0; i<vents.size(); i++)
    {
        if (vents[i].is_horizontal_or_vertical())
        {
            map.map_segment(vents[i]);
            //map.display();
        }
    }
    
    map.display();
    
    ostringstream out;
    out << map.count_dangerous();
    return out.str();
}
