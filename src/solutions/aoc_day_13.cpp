#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_13.h"
#include "file_utils.h"
#include "screen.h"

using namespace std;
using namespace Day13;

#define SCREEN_NO_DOT ' '
#define SCREEN_DOT '#'

namespace Day13
{
    bool Dot::operator <(const Dot & other) const
    {
        if (x!=other.x)
        {
            return (x < other.x);
        }
        return (y < other.y);
    }
};

AocDay13::AocDay13():AocDay(13)
{
}

AocDay13::~AocDay13()
{
}

void AocDay13::parse_input(string filename, set<Dot> & dots, vector<FoldInstruction> & folds)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    
    char delimiters[3];
    delimiters[0]=' ';
    delimiters[1]=',';
    delimiters[2]='=';
    
    if (!fileutils.read_as_list_of_split_strings(filename, data, delimiters, 3, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    int line = 0;
    // do points until I get a blank line
    while (data[line][0].size() > 0)
    {
        Dot dot;
        dot.x=strtol(data[line][0].c_str(), NULL, 10);
        dot.y=strtol(data[line][1].c_str(), NULL, 10);
        dots.insert(dot);
        line++;
    }
    
    // at this point done with dots and line points to the blank line. need to increment to get past it
    line++;
    cout << "line is " << line << endl;
    // Now do the folds
    while (line < data.size())
    {
        FoldInstruction fold;
        fold.direction = data[line][2][0]; // grab the first character for x or y
        fold.position = strtol(data[line][3].c_str(), NULL, 10);
        folds.push_back(fold);
        line++;
    }
    
    return;
}

void AocDay13::display_sheet(set<Dot> & dots)
{
    int max_x=0;
    int max_y=0;
    set<Dot>::iterator pos = dots.begin();
    while (pos != dots.end())
    {
        if ((*pos).x > max_x)
        {
            max_x = (*pos).x;
        }
        if ((*pos).y > max_y)
        {
            max_y = (*pos).y;
        }
        ++pos;
    }
    
    Screen display(SCREEN_NO_DOT, 0, max_x, 0, max_y);
    pos = dots.begin();
    while (pos != dots.end())
    {
        display.set((*pos).x, (*pos).y, SCREEN_DOT);
        ++pos;
    }
    display.display();
}

set<Dot> AocDay13::perform_fold(set<Dot> & input_dots, FoldInstruction fold)
{
    set<Dot> new_dots;
    set<Dot>::iterator pos = input_dots.begin();
    while (pos != input_dots.end())
    {
        if (fold.direction == FOLD_HORIZONTAL)
        {
            if ((*pos).y < fold.position)
            {
                new_dots.insert(*pos);
            }
            else
            {
                Dot dot;
                dot.x=(*pos).x;
                dot.y=fold.position - ((*pos).y - fold.position);
                new_dots.insert(dot);
            }
        }
        
        if (fold.direction == FOLD_VERTICAL)
        {
            if ((*pos).x < fold.position)
            {
                new_dots.insert(*pos);
            }
            else
            {
                Dot dot;
                dot.y=(*pos).y;
                dot.x=fold.position - ((*pos).x - fold.position);
                new_dots.insert(dot);
            }
        }
        ++pos;
    }
    return new_dots;
}

string AocDay13::part1(string filename, vector<string> extra_args)
{
    set<Dot> dots;
    vector<FoldInstruction> folds;
    
    parse_input(filename, dots, folds);
    cout << "There are " << dots.size() << " dots and " << folds.size() << " folds" << endl;
    
    display_sheet(dots);
    
    dots = perform_fold(dots, folds[0]);
    
    cout << "Performing fold " << folds[0].direction << "=" << folds[0].position << " resulted in " << dots.size() << " dots remaining" << endl;
    
    display_sheet(dots);
    
    int result = dots.size();
    
    // For testing
    //dots = perform_fold(dots, folds[1]);
    //cout << "Performing fold " << folds[1].direction << "=" << folds[1].position << " resulted in " << dots.size() << " dots remaining" << endl;
    //display_sheet(dots);
    // End testing
    
    ostringstream out;
    out << result;
    return out.str();
}

string AocDay13::part2(string filename, vector<string> extra_args)
{
    set<Dot> dots;
    vector<FoldInstruction> folds;
    
    parse_input(filename, dots, folds);
    cout << "There are " << dots.size() << " dots and " << folds.size() << " folds" << endl;
    
    for (int i=0; i<folds.size(); i++)
    {
        dots = perform_fold(dots, folds[i]);
        cout << "Performing fold " << folds[i].direction << "=" << folds[i].position << " resulted in " << dots.size() << " dots remaining" << endl;
    }
    
    cout << "FIND THE 8 CAPITAL LETTERS IN THIS OUTPUT FOR THE PUZZLE RESULT!!!" << endl << endl;
    display_sheet(dots);
    
    ostringstream out;
    out << 0;
    return out.str();
}
