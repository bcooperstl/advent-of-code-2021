#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include "aoc_day_8.h"
#include "file_utils.h"

using namespace std;
using namespace Day8;

namespace Day8
{
    Display::Display(vector<string> patterns, vector<string> outputs)
    {
        for (int i=0; i<NUM_PATTERNS; i++)
        {
            m_patterns[i] = patterns[i];
            sort(m_patterns[i].begin(), m_patterns[i].end());
            
        }
        for (int i=0; i<NUM_OUTPUTS; i++)
        {
            m_outputs[i] = outputs[i];
            sort(m_outputs[i].begin(), m_outputs[i].end());
        }
    }
    
    Display::~Display()
    {
    }
    
    void Display::dump()
    {
        cout << "--------------------------------" << endl;
        cout << " Patterns: ";
        for (int i=0; i<NUM_PATTERNS; i++)
        {
            m_patterns[i];
        }
        cout << endl;
        
        cout << " Outputs: ";
        for (int i=0; i<NUM_OUTPUTS; i++)
        {
            cout << m_outputs[i];
        }
        cout << endl;
        
        cout << " Segment Matches: " << endl;
        for (map<char, char>::iterator pos = m_segment_matches.begin(); pos != m_segment_matches.end(); ++pos)
        {
            cout << "  " << pos->first << " --> " << pos->second << endl;
        }
        
        cout << " Values: " << endl;
        for (int i=0; i<NUM_PATTERNS; i++)
        {
            cout << "  " << i << " --> " << m_values[i] << endl;
        }
    }
    
    void Display::process_patterns()
    {
        // For part 1, all we need to do are identify the 1, 4, 7, and 8 values.
        for (int i=0; i<NUM_PATTERNS; i++)
        {
            int len = m_patterns[i].length();
            switch (len)
            {
                case 2: // this is a 1
                    m_values[1] = m_patterns[i];
                    break;
                case 3: // this is a 7
                    m_values[7] = m_patterns[i];
                    break;
                case 4: // this is a 4
                    m_values[4] = m_patterns[i];
                    break;
                case 7: // this is a 8
                    m_values[8] = m_patterns[i];
                    break;
            }
        }
        dump();
    }
    
    int Display::get_part1_count()
    {
        // Return the count of 1, 4, 7, or 8 values in the outputs
        int count = 0;
        for (int i=0; i<NUM_OUTPUTS; i++)
        {
            if (m_outputs[i] == m_values[1] || 
                m_outputs[i] == m_values[4] || 
                m_outputs[i] == m_values[7] || 
                m_outputs[i] == m_values[8])
            {
                count++;
            }
        }
        return count;
        
    }
};

AocDay8::AocDay8():AocDay(8)
{
}

AocDay8::~AocDay8()
{
}

void AocDay8::parse_input(string filename, vector<Display> & displays)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    
    displays.clear();
    
    char delims[2];
    delims[0]=' ';
    delims[1]='|';
    
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
        
    for (int i=0; i<lines.size(); i++)
    {
        vector<string>::iterator start = lines[i].begin();
        vector<string>::iterator middle = lines[i].begin() + NUM_PATTERNS;
        vector<string>::iterator end = lines[i].begin() + NUM_PATTERNS + NUM_OUTPUTS;
        
        vector<string> patterns(start, middle);
        vector<string> outputs(middle, end);
        
        displays.push_back(Display(patterns, outputs));
    }
    
    return;
}

string AocDay8::part1(string filename, vector<string> extra_args)
{
    vector<Display> displays;
    
    parse_input(filename, displays);
    
    int counter = 0;
    
    for (int i=0; i<displays.size(); i++)
    {
        displays[i].process_patterns();
        counter += displays[i].get_part1_count();
    }
    
    ostringstream out;
    out << counter;
    return out.str();
}
