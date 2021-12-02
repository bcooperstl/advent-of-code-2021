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
