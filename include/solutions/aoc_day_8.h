#ifndef __AOC_DAY_8__
#define __AOC_DAY_8__

#include "aoc_day.h"

#include <vector>
#include <map>

#define NUM_PATTERNS 10
#define NUM_OUTPUTS 4

namespace Day8
{
    class Display
    {
        private:
            string m_patterns[NUM_PATTERNS];
            string m_outputs[NUM_OUTPUTS];
            map<char, char> m_segment_matches;
            string m_values[NUM_PATTERNS];
        public:
            Display(vector<string> patterns, vector<string> outputs);
            ~Display();
            void process_patterns();
            void dump();
            int get_part1_count();
    };
}

using namespace Day8;

class AocDay8 : public AocDay
{
    private:
        void parse_input(string filename, vector<Display> & displays);
    public:
        AocDay8();
        ~AocDay8();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
