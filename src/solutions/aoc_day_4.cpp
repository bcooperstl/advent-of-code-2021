#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_4.h"
#include "file_utils.h"

using namespace std;
using namespace Day4;

namespace Day4
{
    Card::Card(vector<vector<long>> inputs)
    {
        for (int row=0; row<BINGO_CARD_SIZE; row++)
        {
            for (int col=0; col<BINGO_CARD_SIZE; col++)
            {
                m_spots[row][col].value = inputs[row][col];
                m_spots[row][col].marked = false;
            }
        }
    }
    
    Card::~Card()
    {
    }
    
    void Card::display()
    {
        for (int row=0; row<BINGO_CARD_SIZE; row++)
        {
            for (int col=0; col<BINGO_CARD_SIZE; col++)
            {
                if (m_spots[row][col].marked)
                {
                    cout << "x" << m_spots[row][col].value << "x ";
                }
                else
                {
                    cout << " " << m_spots[row][col].value << "  ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    
    void Card::mark_value(int value)
    {
        for (int row=0; row<BINGO_CARD_SIZE; row++)
        {
            for (int col=0; col<BINGO_CARD_SIZE; col++)
            {
                if (m_spots[row][col].value == value)
                {
                    m_spots[row][col].marked = true;
                }
            }
        }
    }
    
    bool Card::has_bingo()
    {
        int row_marked[BINGO_CARD_SIZE];
        int col_marked[BINGO_CARD_SIZE];
        
        for (int i=0; i<BINGO_CARD_SIZE; i++)
        {
            row_marked[i] = 0;
            col_marked[i] = 0;
        }
        for (int row=0; row<BINGO_CARD_SIZE; row++)
        {
            for (int col=0; col<BINGO_CARD_SIZE; col++)
            {
                if (m_spots[row][col].marked)
                {
                    row_marked[row]++;
                    col_marked[col]++;
                }
            }
        }
        for (int i=0; i<BINGO_CARD_SIZE; i++)
        {
            if ((row_marked[i] == BINGO_CARD_SIZE) || (col_marked[i] == BINGO_CARD_SIZE))
            {
                return true;
            }
        }
        return false;
    }
    
    int Card::calculate_score(int last_value)
    {
        int unmarked_sum = 0;
        for (int row=0; row<BINGO_CARD_SIZE; row++)
        {
            for (int col=0; col<BINGO_CARD_SIZE; col++)
            {
                if (!m_spots[row][col].marked)
                {
                    unmarked_sum += m_spots[row][col].value;
                }
            }
        }
        return unmarked_sum * last_value;
    }
};

AocDay4::AocDay4():AocDay(4)
{
}

AocDay4::~AocDay4()
{
}

void AocDay4::parse_input(string filename, vector<int> & drawn_numbers, vector<Card> & cards)
{
    FileUtils fileutils;
    vector<vector<long>> lines;
    
    drawn_numbers.clear();
    cards.clear();
    
    char delims[2];
    delims[0]=' ';
    delims[1]=',';

    if (!fileutils.read_as_list_of_split_longs(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
        
    for (int i=0; i<lines[0].size(); i++)
    {
        drawn_numbers.push_back(lines[0][i]);
    }
    
    // cards start at line 2, and go for 5 lines, plus one blank lines.
    for (int card_start=2; card_start < lines.size(); card_start += 6)
    {
        Card card(vector<vector<long>>(lines.begin()+card_start, lines.begin()+card_start+5));
        //card.display();
        cards.push_back(card);
    }
    
    return;
}

string AocDay4::part1(string filename, vector<string> extra_args)
{
    vector<int> drawn_numbers;
    vector<Card> cards;
    
    parse_input(filename, drawn_numbers, cards);
    
    int winning_score = 0;
    for (int num_idx=0; num_idx < drawn_numbers.size() && !winning_score; num_idx++)
    {
        int value = drawn_numbers[num_idx];
        cout << "processing number " << value << endl;
        for (int card_idx=0; card_idx < cards.size() && !winning_score; card_idx++)
        {
            cards[card_idx].mark_value(value);
            if (cards[card_idx].has_bingo())
            {
                cards[card_idx].display();
                winning_score = cards[card_idx].calculate_score(value);
            }
        }
    }
    
    ostringstream out;
    out << winning_score;
    return out.str();
}

string AocDay4::part2(string filename, vector<string> extra_args)
{
    vector<int> drawn_numbers;
    vector<Card> cards;
    
    parse_input(filename, drawn_numbers, cards);
    
    int winning_score = 0;
    for (int num_idx=0; num_idx < drawn_numbers.size(); num_idx++)
    {
        int value = drawn_numbers[num_idx];
        cout << "processing number " << value << endl;
        for (int card_idx=0; card_idx < cards.size(); card_idx++)
        {
            if (!cards[card_idx].has_bingo())
            {
                cards[card_idx].mark_value(value);
                if (cards[card_idx].has_bingo())
                {
                    cards[card_idx].display();
                    winning_score = cards[card_idx].calculate_score(value);
                }
            }
        }
    }
    
    ostringstream out;
    out << winning_score;
    return out.str();
}
