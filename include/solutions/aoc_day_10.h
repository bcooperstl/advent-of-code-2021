#ifndef __AOC_DAY_10__
#define __AOC_DAY_10__

#include "aoc_day.h"

#include <vector>
#include <map>

#define MAX_LENGTH 256

#define OPEN_PAREN '('
#define CLOSE_PAREN ')'
#define OPEN_SQUARE '['
#define CLOSE_SQUARE ']'
#define OPEN_CURLY '{'
#define CLOSE_CURLY '}'
#define OPEN_MATH '<'
#define CLOSE_MATH '>'

class AocDay10 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        map<char, int> m_score_values;
        
        bool is_opening_chunk_char(char ch);
        bool is_closing_chunk_char(char ch);
        bool is_matching_chunk_chars(char open, char close);
        bool check_corrupted(string input, char & invalid_char);
        
    public:
        AocDay10();
        ~AocDay10();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
