#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <string>
#include <vector>
#include <utility>

using namespace std;

class Screen
{
    protected:
        char ** m_textmap;
        int m_min_x;
        int m_max_x;
        int m_min_y;
        int m_max_y;
        int m_width;
        int m_height;
        char m_start_char;
        void allocate_textmap();
        void deallocate_textmap();
    public:
        Screen(char start_char=' ', int min_x=-10, int max_x=10, int min_y=-10, int max_y=10);
        virtual ~Screen();
        Screen(const Screen & other);
        Screen & operator = (const Screen & other);
        int get_min_x();
        int get_max_x();
        int get_min_y();
        int get_max_y();
        int get_width();
        int get_height();
        void load(vector<string> lines);
        //void load_at_offset(int start_x, int start_y); Add this later if needed
        void display();
        bool equals(const Screen & other);
        char get(int x, int y);
        void set(int x, int y, char value);
        void set_multi(vector<pair<int, int>> points, char value);
        int num_matching_neighbors(int x, int y, char target, bool include_diagonals = true);
        int num_matching(char target);
};

#endif
