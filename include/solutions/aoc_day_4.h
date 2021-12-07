#ifndef __AOC_DAY_4__
#define __AOC_DAY_4__

#include "aoc_day.h"

#include <vector>

#define BINGO_CARD_SIZE 5

namespace Day4
{
    struct Spot
    {
        int value;
        bool marked;
    };
    
    class Card
    {
        private:
            Spot m_spots[BINGO_CARD_SIZE][BINGO_CARD_SIZE];
        public:
            Card(vector<vector<long>> inputs);
            ~Card();
            void display();
            void mark_value (int value);
            bool has_bingo();
            int calculate_score(int last_value);
    };
}

using namespace Day4;

class AocDay4 : public AocDay
{
    private:
        void parse_input(string filename, vector<int> & drawn_numbers, vector<Card> & cards);
    public:
        AocDay4();
        ~AocDay4();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
