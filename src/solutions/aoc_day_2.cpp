#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_2.h"
#include "file_utils.h"

using namespace std;
using namespace Day2;

AocDay2::AocDay2():AocDay(2)
{
}

AocDay2::~AocDay2()
{
}

/*
    "forward 2" would have operation Forward, amount 2
    "down 3" would have operation Down, amount 3
    "up 5" would have operation Up, amount 5
*/
vector<Command> AocDay2::parse_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    vector<Command> commands;
    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return commands;
    }
        
    for (int i=0; i<lines.size(); i++)
    {
        vector<string> words = lines[i];
        Command command;
        if (words[0] == "forward")
        {
            command.operation=Forward;
            command.amount=strtol(words[1].c_str(), NULL, 10);
        }
        else if (words[0] == "down")
        {
            command.operation=Down;
            command.amount=strtol(words[1].c_str(), NULL, 10);
        }
        else if (words[0] == "up")
        {
            command.operation=Up;
            command.amount=strtol(words[1].c_str(), NULL, 10);
        }
        else
        {
            cerr << "*****Invalid command on line " << i << endl;
            continue;
        }
        commands.push_back(command);
    }
    
    return commands;
}

string AocDay2::part1(string filename, vector<string> extra_args)
{
    vector<Command> commands = parse_input(filename);
    int horizonal_position = 0;
    int depth = 0;
    for (int i=0; i<commands.size(); i++)
    {
        switch (commands[i].operation)
        {
            case Forward:
                horizonal_position += commands[i].amount;
                break;
            case Down:
                depth += commands[i].amount;
                break;
            case Up:
                depth -= commands[i].amount;
                break;
        }
    }
    ostringstream out;
    out << horizonal_position * depth;
    return out.str();
}
