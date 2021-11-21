#ifndef __OVERLAY_H__
#define __OVERLAY_H__

#include <string>
#include <vector>
#include <utility>

#include "screen.h"

using namespace std;

class Overlay : public Screen
{
    private:
        Screen * m_base_screen;
        char m_blank_char;
    public:
        Overlay(Screen * base_screen, char blank_char=' ');
        ~Overlay();
        Overlay(const Overlay & other);
        Overlay & operator = (const Overlay & other);
        void display_overlay();
        bool equals(const Overlay & other);
        char get(int x, int y);
        void clear(int x, int y);
};

#endif
