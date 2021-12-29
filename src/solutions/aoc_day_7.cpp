#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cmath>

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

long AocDay7::calculate_part2_fuel(long optimal, long starting)
{
    long steps = labs(optimal - starting);
    long fuel = (steps * (steps + 1))/2;
    
    //cout << "moving from " << starting << " to " << optimal << " uses " << fuel << " fuel" << endl;
    return fuel;
    
}

string AocDay7::part2(string filename, vector<string> extra_args)
{
    vector<long> positions = read_input(filename);
    
    double running_total = 0.0;
    for (int i=0; i<positions.size(); i++)
    {
        running_total += (double)positions[i];
    }
    cout << "running total is " << running_total << endl;
    cout << "There are " << positions.size() << " values" << endl;
    long optimal = lround(running_total / ((double)positions.size()));
    cout << "optimal found at " << optimal << endl;
    
    long total_fuel[3] = {0,0,0};
    
    for (int i=0; i<positions.size(); i++)
    {
        total_fuel[0] += calculate_part2_fuel(optimal-1, positions[i]);
        total_fuel[1] += calculate_part2_fuel(optimal, positions[i]);
        total_fuel[2] += calculate_part2_fuel(optimal+1, positions[i]);
    }
    
    cout << "Using optimal of " << optimal-1 << " results in total fuel of " << total_fuel[0] << endl;
    cout << "Using optimal of " << optimal   << " results in total fuel of " << total_fuel[1] << endl;
    cout << "Using optimal of " << optimal+1 << " results in total fuel of " << total_fuel[2] << endl;
    
    long min = total_fuel[0];
    if (total_fuel[1] < min)
    {
        min = total_fuel[1];
    }
    if (total_fuel[2] < min)
    {
        min = total_fuel[2];
    }
    
    
    ostringstream out;
    out << min;
    return out.str();
}
