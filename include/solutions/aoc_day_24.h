#ifndef __AOC_DAY_24__
#define __AOC_DAY_24__

#include "aoc_day.h"

#include <vector>
#include <map>

#define INPUT "inp"
#define ADD "add"
#define MULTIPLY "mul"
#define DIVIDE "div"
#define MODULO "mod"
#define EQUAL "eql"

#define NUM_INPUTS 14

#define W 0
#define X 1
#define Y 2
#define Z 3

namespace Day24
{
    // every input is put in w, and then x is immedilately set to 0. so y and z are all we need for this simple state
    struct SimpleState
    {
        long y;
        long z;
        void display();
        bool operator == (const SimpleState & other);
    };
    
    class CompState
    {
        private:
            long m_variables[4]; // W,X,Y,Z
            long m_next_input;
        public:
            CompState(long next_input);
            CompState(SimpleState simple, long next_input);
            ~CompState();
            long get(int which);
            SimpleState get_simple_state();
            void set(int which, long value);
            void display();
            void do_input(int a);
            void do_add_variable(int a, int b);
            void do_add_constant(int a, long value);
            void do_multiply_variable(int a, int b);
            void do_multiply_constant(int a, long value);
            void do_divide_variable(int a, int b);
            void do_divide_constant(int a, long value);
            void do_modulo_variable(int a, int b);
            void do_modulo_constant(int a, long value);
            void do_equals_variable(int a, int b);
            void do_equals_constant(int a, long value);
    };
    
    
    class PathStep
    {
        private:
            SimpleState m_state;
            PathStep * m_next_steps[9]; // corresponds for 1-9
            int m_depth;
        public:
            PathStep(int depth, SimpleState state);
            ~PathStep();
            void set_next(int which, PathStep * next);
            SimpleState get_state();
            int get_depth();
            PathStep * get_next(int which);
            void display();
    };
    
    class PathCache
    {
        // starting with map from y to a vector. could go to map from y to a map from z to the item if need speedup
        private:
            map<int, map<int, PathStep *>> m_paths; // go from y value to a vector of those with that y value
        public:
            PathCache();
            ~PathCache();
            void put(PathStep * step);
            PathStep * get(SimpleState state);
    };
    
    struct Instruction
    {
        string type;
        int dest;
        int source_var;
        long source_val;
        bool use_source_var;
        void display();
        char get_letter(int which);
        int get_value(char ch);
        
    };
}

using namespace Day24;

class AocDay24 : public AocDay
{
    private:
        void parse_input(string filename, vector<Instruction> & instructions);
        void split_instructions(vector<Instruction> & all, vector<vector<Instruction>> & split);
        void work_section(vector<PathStep *> & from, vector<PathStep *> & to, vector<Instruction> instructions);
    public:
        AocDay24();
        ~AocDay24();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif

