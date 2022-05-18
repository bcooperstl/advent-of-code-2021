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
        public:
            Space();
            ~Space();
            void turn_on(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x);
            void turn_off(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x);
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
        //string part2(string filename, vector<string> extra_args);
};

#endif
