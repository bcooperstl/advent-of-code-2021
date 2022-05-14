#ifndef __AOC_DAY_21__
#define __AOC_DAY_21__

#include "aoc_day.h"

#include <vector>
#include <map>

#define NUM_SPACES 10

namespace Day21
{
    class Die
    {
        protected:
            int m_num_rolls;
        public:
            Die();
            ~Die();
            int get_num_rolls();
            virtual int roll() = 0; // rolls die and returns the value
    };
    
    class DeterministicDie : public Die
    {
        protected:
            int m_min;
            int m_max;
            int m_next;
        public:
            DeterministicDie(int min, int max);
            virtual ~DeterministicDie();
            virtual int roll();
    };
    
    struct Space
    {
        int index;
        int value;
    };
    
    class Game
    {
        private:
            int m_positions[3];
            int m_scores[3];
            Space m_board[NUM_SPACES];
            Die * m_die;
        public:
            Game(Die * die, int player1_position, int player2_position);
            ~Game();
            void play_turn(int player);
            int play_to_winner(int winning_score); // returns the player number (1 or 2) of the winner
            int get_score(int player_number);
    };
    
    
    struct Universe
    {
        int position[3];
        int score[3];
        long count;
        int winner;
    };
}

using namespace Day21;

class AocDay21 : public AocDay
{
    private:
        void parse_input(string Filename, int & player1_start, int & player2_start);
    public:
        AocDay21();
        ~AocDay21();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
