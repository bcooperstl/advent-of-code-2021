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

namespace Day24
{
    struct FullState
    {
        long w;
        long x;
        long y;
        long z;
        void display();
    };
    
    // every input is put in w, and then x is immedilately set to 0. so y and z are all we need for this simple state
    struct SimpleState
    {
        long y;
        long z;
        void display();
    };
    
    struct Path
    {
        int depth;
        bool to_here[NUM_INPUTS][10]; // only care about 1-9; will make it 10 for ease
        void display();
    };
    
    struct Combination
    {
        SimpleState state;
        vector<Path> paths;
        void display();
    };
    
    struct Instruction
    {
        string type;
        char dest;
        char source_char;
        long source_val;
        bool use_source_char;
        void display();
    };
}

using namespace Day24;

class AocDay24 : public AocDay
{
    private:
        void parse_input(string filename, vector<Instruction> & instructions);
        void split_instructions(vector<Instruction> & all, vector<vector<Instruction>> & split);
        void run_instruction(Instruction inst, FullState & state, int next_input);
        void work_combinations(vector<Combination> initial, vector<Combination> & results, vector<Instruction> instructions);
    public:
        AocDay24();
        ~AocDay24();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif

