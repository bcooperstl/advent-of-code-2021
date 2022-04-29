#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>

#include "screen.h"

/* For the screen, the data is stored in a 2-d list, rows, then columns.
 * This means that unlike algebra, the data is accessed by row and then by column.
 * Also, as per standard Advent of Code problems, row numbers increase from top-to-bottom. Columns increase left-to-right
 */

Screen::Screen(char start_char, int min_x, int max_x, int min_y, int max_y)
{
    m_start_char = start_char;
    m_min_x = min_x;
    m_max_x = max_x;
    m_min_y = min_y;
    m_max_y = max_y;
    m_width = m_max_x - m_min_x + 1; // Need to the add the +1 for the 0 column
    m_height = m_max_y - m_min_y + 1; // Need to the add the +1 for the 0 row
    m_textmap = NULL;
    allocate_textmap(&m_textmap, m_height, m_width);
}

Screen::~Screen()
{
    deallocate_textmap(&m_textmap, m_height);
}

Screen::Screen(const Screen & other)
{
    m_start_char = other.m_start_char;
    m_min_x = other.m_min_x;
    m_max_x = other.m_max_x;
    m_min_y = other.m_min_y;
    m_max_y = other.m_max_y;
    m_width = other.m_width;
    m_height = other.m_height;
    m_textmap = NULL;
    allocate_textmap(&m_textmap, m_height, m_width);
    for (int y=0; y<m_height; y++)
    {
        for (int x=0; x<m_width; x++)
        {
            m_textmap[y][x]=other.m_textmap[y][x];
        }
    }
}

Screen & Screen::operator =(const Screen & other)
{
    deallocate_textmap(&m_textmap, m_height);
    m_start_char = other.m_start_char;
    m_min_x = other.m_min_x;
    m_max_x = other.m_max_x;
    m_min_y = other.m_min_y;
    m_max_y = other.m_max_y;
    m_width = other.m_width;
    m_height = other.m_height;
    allocate_textmap(&m_textmap, m_height, m_width);
    for (int y=0; y<m_height; y++)
    {
        for (int x=0; x<m_width; x++)
        {
            m_textmap[y][x]=other.m_textmap[y][x];
        }
    }
    return *this;
}

void Screen::allocate_textmap(char *** textmap, int height, int width)
{
    *textmap = new char *[height];
    for (int y=0; y<height; y++)
    {
        (*textmap)[y] = new char[width];
        for (int x=0; x<width; x++)
        {
            (*textmap)[y][x]=m_start_char;
        }
    }
}

void Screen::deallocate_textmap(char *** textmap, int height)
{
    for (int y=0; y<height; y++)
    {
        delete [] (*textmap)[y];
    }
    delete [] (*textmap);
}

int Screen::get_min_x()
{
    return m_min_x;
}

int Screen::get_max_x()
{
    return m_max_x;
}

int Screen::get_min_y()
{
    return m_min_y;
}

int Screen::get_max_y()
{
    return m_max_y;
}

int Screen::get_width()
{
    return m_width;
}

int Screen::get_height()
{
    return m_height;
}

void Screen::load(vector<string> lines)
{
    load(lines, 0, 0);
}    

void Screen::load(vector<string> lines, int min_x, int min_y)
{
    deallocate_textmap(&m_textmap, m_height);
    
    m_min_y = min_y;
    m_max_y = min_y + lines.size() - 1;
    m_height = lines.size();
    
    m_min_x = min_x;
    m_max_x = min_x + lines[0].size() - 1;
    m_width = lines[0].size();
    
    allocate_textmap(&m_textmap, m_height, m_width);
    for (int y=0; y<m_height; y++)
    {
        string line = lines[y];
        for (int x=0; x<m_width; x++)
        {
            m_textmap[y][x]=line[x];
        }
    }
}

// Starting this by dumping to cout. Later might change to pass in an ostream
void Screen::display()
{
    ostringstream buffer;
    for (int y=0; y<m_height; y++)
    {
        for (int x=0; x<m_width; x++)
        {
            buffer << m_textmap[y][x];
        }
        buffer << endl;
    }
    cout << buffer.str();
}

bool Screen::equals(const Screen & other)
{
    if ((m_min_x != other.m_min_x) ||
        (m_max_x != other.m_max_x) ||
        (m_min_y != other.m_min_y) ||
        (m_max_y != other.m_max_y))
    {
        return false;
    }
    
    for (int y=0; y<m_height; y++)
    {
        for (int x=0; x<m_width; x++)
        {
            if (m_textmap[y][x] != other.m_textmap[y][x])
            {
                return false;
            }
        }
    }
    
    return true;
}

char Screen::get(int x, int y)
{
    if ((x < m_min_x) || (x > m_max_x))
    {
        cerr << "***X value " << x << " is out of range (" << m_min_x << "," << m_max_x << ")" << endl;
        return m_start_char;
    }
    if ((y < m_min_y) || (y > m_max_y))
    {
        cerr << "***Y value " << y << " is out of range (" << m_min_y << "," << m_max_y << ")" << endl;
        return m_start_char;
    }
    return m_textmap[y-m_min_y][x-m_min_x];
}

void Screen::set(int x, int y, char value)
{
    if ((x < m_min_x) || (x > m_max_x))
    {
        cerr << "***X value " << x << " is out of range (" << m_min_x << "," << m_max_x << ")" << endl;
        return;
    }
    if ((y < m_min_y) || (y > m_max_y))
    {
        cerr << "***Y value " << y << " is out of range (" << m_min_y << "," << m_max_y << ")" << endl;
        return;
    }
    m_textmap[y-m_min_y][x-m_min_x] = value;
}

void Screen::set_multi(vector<pair<int, int>> points, char value)
{
    for (int i=0; i<points.size(); i++)
    {
        set(points[i].first, points[i].second, value);
    }
}

int Screen::num_matching(char target)
{
    int count=0;
    for (int y=0; y<m_height; y++)
    {
        for (int x=0; x<m_width; x++)
        {
            if (m_textmap[y][x]==target)
            {
                count++;
            }
        }
    }
    return count;
}

int Screen::num_matching_neighbors(int x, int y, char target, bool include_diagonals)
{
    int count=0;
    
    if ((x < m_min_x) || (x > m_max_x))
    {
        cerr << "***X value " << x << " is out of range (" << m_min_x << "," << m_max_x << ")" << endl;
        return 0;
    }
    if ((y < m_min_y) || (y > m_max_y))
    {
        cerr << "***Y value " << y << " is out of range (" << m_min_y << "," << m_max_y << ")" << endl;
        return 0;
    }
    
    // up
    if (y > m_min_y)
    {
        if (m_textmap[y-m_min_y-1][x-m_min_x] == target)
        {
            count++;
        }
    }
    
    // down
    if (y < m_max_y)
    {
        if (m_textmap[y-m_min_y+1][x-m_min_x] == target)
        {
            count++;
        }
    }
    
    // left
    if (x > m_min_x)
    {
        if (m_textmap[y-m_min_y][x-m_min_x-1] == target)
        {
            count++;
        }
    }
    
    // right
    if (x < m_max_x)
    {
        if (m_textmap[y-m_min_y][x-m_min_x+1] == target)
        {
            count++;
        }
    }
    
    if (include_diagonals)
    {
        // up-left
        if ((y > m_min_y) && (x > m_min_x))
        {
            if (m_textmap[y-m_min_y-1][x-m_min_x-1] == target)
            {
                count++;
            }
        }
        
        // up-right
        if ((y > m_min_y) && (x < m_max_x))
        {
            if (m_textmap[y-m_min_y-1][x-m_min_x+1] == target)
            {
                count++;
            }
        }
        
        // down-left
        if ((y < m_max_y) && (x > m_min_x))
        {
            if (m_textmap[y-m_min_y+1][x-m_min_x-1] == target)
            {
                count++;
            }
        }
        
        // down-right
        if ((y < m_max_y) && (x < m_max_x))
        {
            if (m_textmap[y-m_min_y+1][x-m_min_x+1] == target)
            {
                count++;
            }
        }
    }
    return count;
}

void Screen::expand(char expand_char)
{
    char ** old_textmap = m_textmap;
    
    char ** new_textmap = NULL;
    allocate_textmap(&new_textmap, m_height+2, m_width+2);
    
    // copy the existing data first and add expand_chars to first and last column
    for (int y=0; y<m_height; y++)
    {
        for (int x=0; x<m_width; x++)
        {
            new_textmap[y+1][x+1]=old_textmap[y][x];
        }
        new_textmap[y+1][0] = expand_char;
        new_textmap[y+1][m_width+1] = expand_char;
    }
    
    // add expand char to top and bottom rows
    for (int x=0; x<m_width+2; x++)
    {
        new_textmap[0][x] = expand_char;
        new_textmap[m_height+1][x] = expand_char;
    }
    
    
    deallocate_textmap(&old_textmap, m_height);
    
    m_textmap = new_textmap;
    m_height+=2;
    m_min_y--;
    m_max_y++;
    m_width+=2;
    m_min_x--;
    m_max_x++;
}
