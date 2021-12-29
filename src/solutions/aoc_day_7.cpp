#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include "aoc_day_7.h"
#include "file_utils.h"

using namespace std;

AocDay7::AocDay7():AocDay(7)
{
}

AocDay7::~AocDay7()
{
}

vector<long> AocDay7::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> values;
    if (!fileutils.read_as_list_of_split_longs(filename, values, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    
    return values[0];
}

string AocDay7::part1(string filename, vector<string> extra_args)
{
    vector<long> positions = read_input(filename);
    sort(positions.begin(), positions.end());
    
    long optimal = positions[positions.size()/2];
    long total_fuel = 0;
    
    for (int i=0; i<positions.size(); i++)
    {
        total_fuel += labs(positions[i] - optimal);
    }
    
    ostringstream out;
    out << total_fuel;
    return out.str();
}

//string AocDay7::part2(string filename, vector<string> extra_args)
//{
//    return part1(filename, extra_args);
//}
