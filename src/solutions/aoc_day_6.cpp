#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_6.h"
#include "file_utils.h"

using namespace std;

AocDay6::AocDay6():AocDay(6)
{
}

AocDay6::~AocDay6()
{
}

vector<long> AocDay6::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> values;
    if (!fileutils.read_as_list_of_split_longs(filename, values, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    
    return values[0];
}

string AocDay6::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 6 Part 1 requires one extra argument for the number of days" << endl;
        return "";
    }
    
    vector<long> ages = read_input(filename);
    int days = strtoul(extra_args[0].c_str(), 0, 10);
    
    long counters[9];
    for (int i=0; i<9; i++)
    {
        counters[i] = 0;
    }
    
    for (int i=0; i<ages.size(); i++)
    {
        counters[ages[i]]++;
    }
    
    cout << "Initial Counts:";
    for (int i=0; i<9; i++)
    {
        cout << ' ' << counters[i];
    }
    cout << endl;
    
    for (int i=1; i<=days; i++)
    {
        long cloning = counters[0];
        for (int j=1; j<=8; j++)
        {
            counters[j-1] = counters[j];
        }
        counters[6]+=cloning;
        counters[8]=cloning;
        
        cout << "Counters after day " << i << ':';
        for (int i=0; i<9; i++)
        {
            cout << ' ' << counters[i];
        }
        cout << endl;
    }
    
    long total = 0;
    for (int i=0; i<9; i++)
    {
        total+=counters[i];
    }
    
    ostringstream out;
    out << total;
    return out.str();
}

string AocDay6::part2(string filename, vector<string> extra_args)
{
    return part1(filename, extra_args);
}
