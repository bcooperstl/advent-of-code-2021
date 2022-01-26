#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include "aoc_day_10.h"
#include "file_utils.h"

using namespace std;

AocDay10::AocDay10():AocDay(10)
{
    m_score_values[CLOSE_PAREN] = 3;
    m_score_values[CLOSE_SQUARE] = 57;
    m_score_values[CLOSE_CURLY] = 1197;
    m_score_values[CLOSE_MATH] = 25137;
}

AocDay10::~AocDay10()
{
}

vector<string> AocDay10::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
        
    return lines;
}

bool AocDay10::is_opening_chunk_char(char ch)
{
    return ((ch == OPEN_PAREN) || (ch == OPEN_SQUARE) || (ch == OPEN_CURLY) || (ch == OPEN_MATH));
}

bool AocDay10::is_closing_chunk_char(char ch)
{
    return ((ch == CLOSE_PAREN) || (ch == CLOSE_SQUARE) || (ch == CLOSE_CURLY) || (ch == CLOSE_MATH));
}

bool AocDay10::is_matching_chunk_chars(char open, char close)
{
    return ((open == OPEN_PAREN && close == CLOSE_PAREN) ||
            (open == OPEN_SQUARE && close == CLOSE_SQUARE) ||
            (open == OPEN_CURLY && close == CLOSE_CURLY) ||
            (open == OPEN_MATH && close == CLOSE_MATH));
}

bool AocDay10::check_corrupted(string input, char & invalid_char)
{
    char stack[MAX_LENGTH];
    int stack_pos = -1;
    for (int i=0; i<input.length(); i++)
    {
        char curr = input[i];
        if (is_opening_chunk_char(curr))
        {
            ++stack_pos;
            stack[stack_pos] = curr;
        }
        if (is_closing_chunk_char(curr))
        {
            if (stack_pos == -1)
            {
                invalid_char = curr;
                return true;
            }
            if (is_matching_chunk_chars(stack[stack_pos], curr))
            {
                stack_pos--;
            }
            else
            {
                invalid_char = curr;
                return true;
            }
        }
    }
    return false;
};

string AocDay10::part1(string filename, vector<string> extra_args)
{
    vector<string> lines = read_input(filename);
    int sum_score = 0;
    char invalid_char;
    bool is_corrupted;
    
    for (int i=0; i<lines.size(); i++)
    {
        is_corrupted = check_corrupted(lines[i], invalid_char);
        if (is_corrupted)
        {
            sum_score += m_score_values[invalid_char];
        }
    }
    
    ostringstream out;
    out << sum_score;
    return out.str();
}
