#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_21.h"
#include "file_utils.h"

using namespace std;
using namespace Day21;

namespace Day21
{
    Die::Die()
    {
        m_num_rolls = 0;
    }
    
    Die::~Die()
    {
    }
    
    int Die::get_num_rolls()
    {
        return m_num_rolls;
    }
    
    DeterministicDie::DeterministicDie(int min, int max):Die()
    {
        m_min = min;
        m_max = max;
        m_next = min;
    }
    
    DeterministicDie::~DeterministicDie()
    {
    }
    
    int DeterministicDie::roll()
    {
        int value = m_next;
        m_next++;
        if (m_next > m_max)
        {
            m_next = m_min;
        }
        m_num_rolls++;
        return value;
    }
    
    Game::Game(Die * die, int player1_position, int player2_position)
    {
        m_die = die;
        for (int i=0; i<NUM_SPACES; i++)
        {
            m_board[i].index = i;
            m_board[i].value = i;
        }
        m_board[0].value=NUM_SPACES;
        
        if (player1_position != NUM_SPACES)
        {
            m_positions[1] = player1_position;
        }
        else
        {
            m_positions[1] = 0;
        }
        if (player2_position != NUM_SPACES)
        {
            m_positions[2] = player2_position;
        }
        else
        {
            m_positions[2] = 0;
        }
        
        m_scores[1] = 0;
        m_scores[2] = 0;
    }
    
    Game::~Game()
    {
    }
    
    int Game::get_score(int player_number)
    {
        return m_scores[player_number];
    }
    
    void Game::play_turn(int player)
    {
        cout << "Player " << player << " rolls ";
        for (int i=0; i<3; i++)
        {
            int roll = m_die->roll();
            cout << roll;
            if (i < 2)
            {
                cout << '+';
            }
            m_positions[player] += roll;
        }
        m_positions[player] = m_positions[player]%10;
        
        m_scores[player] += m_board[m_positions[player]].value;
        cout << " and moves to space " << m_positions[player] << " for a total score of " << m_scores[player] << '.' << endl;
    }
    
    int Game::play_to_winner(int winning_score)
    {
        int player = 1;
        while (m_scores[1] < winning_score && m_scores[2] < winning_score)
        {
            play_turn(player);
            player = (player == 1 ? 2 : 1); // switch players
        }
        return (m_scores[1] >= winning_score ? 1 : 2); // return the winning player
    }
};

AocDay21::AocDay21():AocDay(21)
{
}

AocDay21::~AocDay21()
{
}

/*
Player 1 starting position: 4
Player 2 starting position: 8
*/

void AocDay21::parse_input(string filename, int & player1_start, int & player2_start)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    
    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    
    player1_start = strtol(lines[0][4].c_str(), NULL, 10);
    player2_start = strtol(lines[1][4].c_str(), NULL, 10);
}

string AocDay21::part1(string filename, vector<string> extra_args)
{
    int start1, start2;
    parse_input(filename, start1, start2);
    
    DeterministicDie d100(1, 100);
    
    Game game(&d100, start1, start2);
    int winner = game.play_to_winner(1000);
    int loser_score = (winner == 1 ? game.get_score(2) : game.get_score(1));
    int rolls = d100.get_num_rolls();
    
    
    ostringstream out;
    out << loser_score * rolls;
    return out.str();
}
