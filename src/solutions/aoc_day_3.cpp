#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_3.h"
#include "file_utils.h"

using namespace std;

#define MAX_LENGTH 20

AocDay3::AocDay3():AocDay(3)
{
}

AocDay3::~AocDay3()
{
}

vector<long> AocDay3::read_input(string filename, int & length)
{
    FileUtils fileutils;
    vector<string> lines;
    vector<long> values;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return values;
    }
    length = lines[0].length();
    for (int i=0; i<lines.size(); i++)
    {
        values.push_back(strtol(lines[i].c_str(), NULL, 2));
    }
    
    return values;
}

string AocDay3::part1(string filename, vector<string> extra_args)
{
    int length;
    vector<long> data = read_input(filename, length);
    int ones[MAX_LENGTH];
    long masks[MAX_LENGTH];
    
    for (int i=0; i<length; i++)
    {
        ones[i] = 0;
        masks[i]=1 << (length-1-i);
    }
    
    for (int idx=0; idx<data.size(); idx++)
    {
        long item = data[idx];
        for (int i=0; i<length; i++)
        {
            if (item & masks[i])
            {
                ones[i]++;
            }
            else
            {
                ones[i]--;
            }
        }
    }
    
    long gamma=0;
    long epsilon=0;
    
    for (int i=0; i<length; i++)
    {
        if (ones[i] > 0)
        {
            gamma+=masks[i];
        }
        else if (ones[i] < 0)
        {
            epsilon+=masks[i];
        }
        else
        {
            cerr << "ones[" << i << "] is zero. investigate!!" << endl;
        }
    }
    
    cout << " Gamma " << gamma << " * epsilon " << epsilon << " = " << gamma * epsilon << endl;
    
    ostringstream out;
    out << gamma * epsilon;
    return out.str();
}
