#ifndef __AOC_DAY_18__
#define __AOC_DAY_18__

#include "aoc_day.h"

#include <vector>
#include <map>
#include <utility>

#define LEFT 0
#define RIGHT 1

#define TYPE_NUMBER 2
#define TYPE_PAIR 3

namespace Day18
{
    class Node
    {
        public:
            Node();
            ~Node();
            virtual string to_string() = 0;
            //virtual Node * clone() = 0;
            virtual int get_type() = 0;
    };
    
    class Number : public Node
    {
        protected:
            long m_value;
        public:
            Number(long value);
            virtual ~Number();
            long get_value();
            void set_value(long value);
            virtual string to_string();
            //virtual Node * clone();
            virtual int get_type();
    };
    
    class Pair : public Node
    {
        protected:
            Node * m_members[2];
            int m_depth;
        public:
            Pair();
            virtual ~Pair();
            Node * get_member(int which);
            void set_member(Node * value, int which);
            
            int get_depth();
            void set_depth(int depth);
            
            virtual string to_string();
            //virtual Node * clone();
            virtual int get_type();
            
            void build_number_list(vector<Number *> & numbers);
            Pair * find_parent(Node * target);
            Pair * find_first_to_explode();
            Number * find_first_to_split();
    };
}

using namespace Day18;

class AocDay18 : public AocDay
{
    private:
        vector<Pair *> parse_input(string filename);
        Pair * convert_line(string input);
        void explode(Pair * base, Pair * target);
        void split(Pair * base, Number * target);
        string run_test(string filename, string test);
    public:
        AocDay18();
        ~AocDay18();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
