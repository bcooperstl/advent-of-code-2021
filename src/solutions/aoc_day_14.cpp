#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "aoc_day_14.h"
#include "file_utils.h"

using namespace std;
using namespace Day14;

AocDay14::AocDay14():AocDay(14)
{
}

AocDay14::~AocDay14()
{
}

void AocDay14::parse_input(string filename, string & polymer, map<string, Rule> & rule_map)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    
    if (!fileutils.read_as_list_of_split_strings(filename, data, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    polymer = data[0][0];
    
    for (int i=2; i<data.size(); i++)
    {
        Rule rule;
        rule.input = data[i][0];
        rule.insertion = data[i][2][0]; // just one character
        rule.output[0] = rule.input;
        rule.output[1] = rule.input;
        rule.output[0][1] = rule.insertion;
        rule.output[1][0] = rule.insertion;
        cout << "Rule " << rule.input << " -> " << rule.insertion << " results in " << rule.output[0] << " and " << rule.output[1] << endl;
        rule_map[rule.input] = rule;
    }
    
    return;
}

string AocDay14::part1(string filename, vector<string> extra_args)
{
    string initial_polymer;
    map<string, Rule> rules;
    
    parse_input(filename, initial_polymer, rules);
    
    map<string, int> current;
    for (int i=0; i<initial_polymer.size() - 1; i++)
    {
        string pair = initial_polymer.substr(i, 2); // get 2 characters starting at i
        if (current.find(pair) != current.end())
        {
            current[pair] = current[pair] + 1;
        }
        else
        {
            current[pair] = 1;
        }
    }
    
    for (int i=0; i<10; i++)
    {
        map<string, int> next;
        for (map<string, int>::iterator pos = current.begin(); pos != current.end(); ++pos)
        {
            Rule rule = rules[pos->first];
            for (int i=0; i<2; i++)
            {
                if (next.find(rule.output[i]) != next.end())
                {
                    next[rule.output[i]] = next[rule.output[i]] + pos->second;
                }
                else
                {
                    next[rule.output[i]] = pos->second;
                }
            }
        }
        current = next;
    }
    
    map<char, int> quantities;
    for (map<string, int>::iterator pos = current.begin(); pos != current.end(); ++pos)
    {
        string pair = pos->first;
        for (int i=0; i<2; i++)
        {
            if (quantities.find(pair[i]) != quantities.end())
            {
                quantities[pair[i]] = quantities[pair[i]] + pos->second;
            }
            else
            {
                quantities[pair[i]] = pos->second;
            
            }
        }
    }
    
    quantities[initial_polymer[0]] = quantities[initial_polymer[0]] + 1;
    quantities[initial_polymer[initial_polymer.size()-1]] = quantities[initial_polymer[initial_polymer.size()-1]] + 1;
    
    for (map<char, int>::iterator pos = quantities.begin(); pos != quantities.end(); ++pos)
    {
        quantities[pos->first] = pos->second/2;
    }
    
    int largest = quantities.begin()->second;
    int smallest = quantities.begin()->second;

    for (map<char, int>::iterator pos = quantities.begin(); pos != quantities.end(); ++pos)
    {
        cout << "Character " << pos->first << " appears " << pos->second << " times." << endl;
        if (pos->second > largest)
        {
            largest = pos->second;
        }
        if (pos->second < smallest)
        {
            smallest = pos->second;
        }
    }
    
    
    ostringstream out;
    out << largest - smallest;
    return out.str();
}
