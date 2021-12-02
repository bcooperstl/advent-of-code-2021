#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_1.h"
#include "file_utils.h"

using namespace std;

AocDay1::AocDay1():AocDay(1)
{
}

AocDay1::~AocDay1()
{
}

vector<long> AocDay1::read_input(string filename)
{
    FileUtils fileutils;
    vector<long> data;
    if (!fileutils.read_as_list_of_longs(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

vector<long> AocDay1::create_sliding_windows(vector<long> & measurements)
{
    vector<long> window_values;
    for (int i=0; i<=measurements.size()-3; i++)
    {
        window_values.push_back(measurements[i] + measurements[i+1] + measurements[i+2]);
    }
    return window_values;
}

string AocDay1::part1(string filename, vector<string> extra_args)
{
    vector<long> measurements = read_input(filename);
    long increase_counter = 0;
    for (int i=1; i<measurements.size(); i++)
    {
        if (measurements[i] > measurements[i-1])
        {
            increase_counter++;
        }
    }
    ostringstream out;
    out << increase_counter;
    return out.str();
}

string AocDay1::part2(string filename, vector<string> extra_args)
{
    vector<long> measurements = read_input(filename);
    vector<long> window_values = create_sliding_windows(measurements);
    long increase_counter = 0;
    for (int i=1; i<window_values.size(); i++)
    {
        if (window_values[i] > window_values[i-1])
        {
            increase_counter++;
        }
    }
    ostringstream out;
    out << increase_counter;
    return out.str();
}
