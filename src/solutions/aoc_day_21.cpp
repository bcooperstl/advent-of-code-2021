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

string AocDay21::part2(string filename, vector<string> extra_args)
{
    int start1, start2;
    parse_input(filename, start1, start2);
    
    // player, position, score
    long wins[3]; // total wins per player
    long boards[3][10][24]; // absolute max score for a player is 23
    
    /*
    111 = 3  211 = 4  311 = 5
    112 = 4  212 = 5  312 = 6
    113 = 5  213 = 6  313 = 7
    121 = 4  221 = 5  321 = 6
    122 = 5  222 = 6  322 = 7
    123 = 6  223 = 7  323 = 8
    131 = 5  231 = 6  331 = 7
    132 = 6  232 = 7  332 = 8
    133 = 7  233 = 8  333 = 9
    
    of the 27 combinations:
        3) has 1
        4) has 3
        5) has 6
        6) has 7
        7) has 6
        8) has 3
        9) has 1
    */
    
    long split_offsets[10] = { 0, 0, 0, 1, 3, 6, 7, 6, 3, 1};
    
    long values[10];
    for (int i=1; i<10; i++)
    {
        values[i] = i;
    }
    values[0] = 10;
    
    vector<Universe> active_universes;
    vector<Universe> finished_universes[3];
    
    Universe starting;
    starting.count = 1;
    starting.winner = 0;
    
    starting.position[1] = start1;
    starting.position[2] = start2;
    starting.score[1] = 0;
    starting.score[2] = 0;
    
    active_universes.push_back(starting);
    int player = 1;
    int other  = 2;
    while (active_universes.size() > 0)
    {
        cout << "PLAYER " << player << " TURN" << endl;
        vector<Universe> next_active_universes;
        for (int i=0; i<active_universes.size(); i++)
        {
            Universe current = active_universes[i];
            //cout << "Working current universe with " << current.count << " copies of player 1 at " << values[current.position[1]] << " with score " << current.score[1] 
            //     << " and player 2 at " << values[current.position[2]] << " with score " << current.score[2] << endl;
            for (int i=0; i<10; i++)
            {
                if (split_offsets[i] > 0)
                {
                    Universe next;
                    next.count = current.count * split_offsets[i];
                    next.position[other] = current.position[other];
                    next.score[other] = current.score[other];
                    next.position[player] = (current.position[player]+i)%10;
                    next.score[player] = current.score[player] + values[next.position[player]];
                    //cout << " Created universe with " << next.count << " copies of player 1 at " << values[next.position[1]] << " with score " << next.score[1] 
                    //    << " and player 2 at " << values[next.position[2]] << " with score " << next.score[2] << endl;
                    bool found = false;
                    // check for winners
                    if (next.score[player] >= 21)
                    {
                        //cout << "  Player " << player << " has won in " << next.count << " universes" << endl;
                        next.winner = player;
                        finished_universes[player].push_back(next);
                    }
                    else // no winner; add to next round
                    {
                        for (int j=0; j<next_active_universes.size(); j++)
                        {
                            if (next_active_universes[j].position[1] == next.position[1] && 
                                next_active_universes[j].position[2] == next.position[2] &&
                                next_active_universes[j].score[1] == next.score[1] &&
                                next_active_universes[j].score[2] == next.score[2])
                            {
                                next_active_universes[j].count += next.count;
                                //cout << "  Adding to existing next element to bring its count to " << next_active_universes[j].count << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            next_active_universes.push_back(next);
                            //cout << "  Setting as new next universe element" << endl;
                        }
                    }
                }
            }
        }
        int tmp = player;
        player = other;
        other = tmp;
        active_universes = next_active_universes;
    }
    
    long total_wins[3];
    for (int i=1; i<3; i++)
    {
        total_wins[i] = 0;
        for (int j=0; j<finished_universes[i].size(); j++)
        {
            total_wins[i] += finished_universes[i][j].count;
        }
        cout << "Player " << i << " won in " << total_wins[i] << " universes" << endl;
    }
    
    ostringstream out;
    out << (total_wins[1] > total_wins[2] ? total_wins[1] : total_wins[2]);
    return out.str();
}
