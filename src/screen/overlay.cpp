#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>

#include "overlay.h"

Overlay::Overlay(Screen * base_screen, char blank_char):Screen(blank_char, base_screen->get_min_x(), base_screen->get_max_x(), base_screen->get_min_y(), base_screen->get_max_y())
{
    m_base_screen = base_screen;
    m_blank_char = blank_char;
}

// Screen class has virtual destructor which will be called
Overlay::~Overlay()
{
}

Overlay::Overlay(const Overlay & other):Screen(other)
{
    m_base_screen = other.m_base_screen;
    m_blank_char = other.m_blank_char;
}

Overlay & Overlay::operator =(const Overlay & other)
{
    Screen::operator =(other);
    m_base_screen = other.m_base_screen;
    m_blank_char = other.m_blank_char;
    return *this;
}

// Starting this by dumping to cout. Later might change to pass in an ostream
void Overlay::display_overlay()
{
    ostringstream buffer;
    for (int y=0; y<m_width; y++)
    {
        for (int x=0; x<m_height; x++)
        {
            buffer << get(x,y);
        }
        buffer << endl;
    }
    cout << buffer.str();
}

char Overlay::get(int x, int y)
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
    if (m_textmap[y-m_min_y][x-m_min_x] == m_blank_char)
    {
        return m_base_screen->get(x,y);
    }
    return m_textmap[y-m_min_y][x-m_min_x];
}

void Overlay::clear(int x, int y)
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
    m_textmap[y-m_min_y][x-m_min_x] = m_blank_char;
}

