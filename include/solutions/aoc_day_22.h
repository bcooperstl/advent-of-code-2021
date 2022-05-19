#ifndef __AOC_DAY_22__
#define __AOC_DAY_22__

#include "aoc_day.h"

#include <vector>
#include <map>

#define NUM_SPACES 10

namespace Day22
{
    struct Step
    {
        string method;
        int min_x;
        int max_x;
        int min_y;
        int max_y;
        int min_z;
        int max_z;
    };
    
    struct OnPair
    {
        int min;
        int max;
        OnPair * next;
    };
    
    struct OnSpace
    {
        long min_x;
        long max_x;
        long min_y;
        long max_y;
        long min_z;
        long max_z;
        OnSpace * prev;
        OnSpace * next;
    };
    
    struct OnOffCoordinates
    {
        bool use_low_on;
        int low_on_min;
        int low_on_max;
        bool use_high_on;
        int high_on_min;
        int high_on_max;
        int off_min;
        int off_max;
    };
    
    class Row
    {
        private:
            OnPair * m_head_pair;
        public:
            Row();
            ~Row();
            void turn_on(int min_x, int max_x);
            void turn_off(int min_x, int max_x);
            long get_count_on();
            void display();
    };
    
    class Plane
    {
        private:
            map<int, Row *> m_rows;
        public:
            Plane();
            ~Plane();
            void turn_on(int min_y, int max_y, int min_x, int max_x);
            void turn_off(int min_y, int max_y, int min_x, int max_x);
            long get_count_on();
    };
    
    class Space
    {
        private:
            map<int, Plane *> m_planes;
            OnSpace * m_head;
            OnSpace * m_tail;
            long get_count_on(OnSpace * space);
            void set_on_off_coordinates(int off_min, int off_max, int space_min, int space_max, OnOffCoordinates & coordinates);
            void create_on_space(int min_x, int max_x, int min_y, int max_y, int min_z, int max_z, OnSpace ** head, OnSpace ** current);
            void make_spaces_from_coordinates(OnOffCoordinates x_coordinates, OnOffCoordinates y_coordinates, OnOffCoordinates z_coordinates, OnSpace ** first, OnSpace ** last);
            void dump_space();
        public:
            Space();
            ~Space();
            void turn_on(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x);
            void turn_off(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x);
            void turn_off_space(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x);
            long get_count_on_old();
            long get_count_on();
            
    };
        
}

using namespace Day22;

class AocDay22 : public AocDay
{
    private:
        vector<Step> parse_input(string Filename);
    public:
        AocDay22();
        ~AocDay22();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
