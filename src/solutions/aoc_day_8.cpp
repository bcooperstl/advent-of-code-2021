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
            cout << m_patterns[i] << ' ';
        }
        cout << endl;
        
        cout << " Outputs: ";
        for (int i=0; i<NUM_OUTPUTS; i++)
        {
            cout << m_outputs[i] << ' ';
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

        cout << " Digits: " << endl;
        for (map<string, int>::iterator pos = m_digits.begin(); pos != m_digits.end(); ++pos)
        {
            cout << "  " << pos->first << " --> " << pos->second << endl;
        }
        
    }
    
    // See day8.md for the logic.
    void Display::process_patterns()
    {
        string five_patterns[3]; // only three with five segments
        string six_patterns[3];  // only three with six segents
        
        int five_idx=0;
        int six_idx=0;
        
        for (int i=0; i<NUM_PATTERNS; i++)
        {
            int len = m_patterns[i].length();
            switch (len)
            {
                case 2: // this is a 1
                    m_values[1] = m_patterns[i];
                    m_digits[m_patterns[i]] = 1;
                    break;
                case 3: // this is a 7
                    m_values[7] = m_patterns[i];
                    m_digits[m_patterns[i]] = 7;
                    break;
                case 4: // this is a 4
                    m_values[4] = m_patterns[i];
                    m_digits[m_patterns[i]] = 4;
                    break;
                case 5:
                    five_patterns[five_idx] = m_patterns[i];
                    five_idx++;
                    break;
                case 6:
                    six_patterns[six_idx] = m_patterns[i];
                    six_idx++;
                    break;
                case 7: // this is a 8
                    m_values[8] = m_patterns[i];
                    m_digits[m_patterns[i]] = 8;
                    break;
            }
        }
        
        // Identify segment A by finding the segment in `values[7]` that is not in `values[1]`. 
        {
            string seven = m_values[7];
            string one = m_values[1];
            for (int i=0; i<3; i++)
            {
                bool matched = false;
                for (int j=0; j<2; j++)
                {
                    if (seven[i] == one[j])
                    {
                        matched = true;
                    }
                }
                if (matched == false)
                {
                    m_segment_matches[seven[i]] = 'A';
                    m_segment_matches['A'] = seven[i];
                    break;
                }
            }
        }
        
        // Find the pattern for 6 by looking at the three options in `patterns_sixes` that does not have both of the segments of 1
        {
            string one = m_values[1];
            for (int i=0; i<3; i++)
            {
                string possibility = six_patterns[i];
                int count = 0;
                for (int j=0; j<6; j++) // i know these are length six
                {
                    if (possibility[j] == one[0] || possibility[j] == one[1])
                    {
                        count++;
                    }
                }
                if (count != 2) // both not found - this is the matche
                {
                    m_values[6] = possibility;
                    m_digits[possibility] = 6;
                    break;
                }
            }
        }
        
        // Find the segment in `values[1]` that is also in `values[6]`. This is the segment F.
        // Find the segment in `values[1]` that is not in `values[6]`. This is the segment C.
        {
            string one = m_values[1];
            string six = m_values[6];
            
            bool found_first = false;
            
            for (int i=0; i<6; i++)
            {
                if (six[i] == one[0])
                {
                    found_first = true;
                    break;
                }
                else if (six[i] == one[1])
                {
                    // leave found_first to false
                    break;
                }
            }
            if (found_first) // first character is F, second character is C.
            {
                m_segment_matches[one[0]] = 'F';
                m_segment_matches['F'] = one[0];
                m_segment_matches[one[1]] = 'C';
                m_segment_matches['C'] = one[1];
            }
            else // first character is C, second character is F.
            {
                m_segment_matches[one[0]] = 'C';
                m_segment_matches['C'] = one[0];
                m_segment_matches[one[1]] = 'F';
                m_segment_matches['F'] = one[1];
            }                
        }

        // Using the knowledge of segments C and F, loop over the three items in five_patterns.
        // If the item has both C and F, then it is 3.
        // If the item has only C, then it is 2.
        // If the item has only F, then it is 5.
        {
            char actual_c = m_segment_matches['C'];
            char actual_f = m_segment_matches['F'];
            for (int i=0; i<3; i++) // 3 items in five_patterns
            {
                bool has_actual_c = false;
                bool has_actual_f = false;
                string pattern = five_patterns[i];
                for (int j=0; j<5; j++) // 5 chars in each pattern
                {
                    if (pattern[j] == actual_c)
                    {
                        has_actual_c = true;
                    }
                    if (pattern[j] == actual_f)
                    {
                        has_actual_f = true;
                    }
                }
                if (has_actual_c && has_actual_f)
                {
                    m_values[3] = pattern;
                    m_digits[pattern] = 3;
                }
                else if (has_actual_c && (!has_actual_f))
                {
                    m_values[2] = pattern;
                    m_digits[pattern] = 2;
                }
                else if (has_actual_f && (!has_actual_c))
                {
                    m_values[5] = pattern;
                    m_digits[pattern] = 5;
                }
            }
        }
        
        // create a list dg_possibilities containing the segments in 3 that are not matched to A,C, or F.
        // the segment in 2 that is not A, or C, and is not in dg_possibilities, maps to E.
        // the segment in 5 that is not A, or F, and is not in dg_possibilities, maps to B.
        // Loop through six_patterns to find the element that only contains one of dg_possibilities. This is 0.
        //  The one of the two matched in dg_possibilities maps to G. The other maps to D.

        {
            char dg_possibilities[2];
            int dg_idx = 0;
            string three = m_values[3];
            for (int i=0; i<5; i++) // three is length 5
            {
                if (!(three[i] == m_segment_matches['A'] || 
                      three[i] == m_segment_matches['C'] || 
                      three[i] == m_segment_matches['F']))
                {
                    dg_possibilities[dg_idx] = three[i];
                    dg_idx++;
                }
            }
            string two = m_values[2];
            for (int i=0; i<5; i++) // two is length 5
            {
                if (!(two[i] == m_segment_matches['A'] || 
                      two[i] == m_segment_matches['C'] || 
                      two[i] == dg_possibilities[0] || 
                      two[i] == dg_possibilities[1]))
                {
                    m_segment_matches[two[i]] = 'E';
                    m_segment_matches['E'] = two[i];
                    break;
                }
            }
            string five = m_values[5];
            for (int i=0; i<5; i++) // two is length 5
            {
                if (!(five[i] == m_segment_matches['A'] || 
                      five[i] == m_segment_matches['F'] || 
                      five[i] == dg_possibilities[0] || 
                      five[i] == dg_possibilities[1]))
                {
                    m_segment_matches[five[i]] = 'B';
                    m_segment_matches['B'] = five[i];
                    break;
                }
            }
            
            for (int i=0; i<3; i++)
            {
                string pattern = six_patterns[i];
                int count = 0;
                for (int j=0; j<6; j++)
                {
                    if (pattern[j] == dg_possibilities[0] || pattern[j] == dg_possibilities[1])
                    {
                        count++;
                    }
                }
                if (count == 1)
                {
                    m_values[0] = pattern;
                    m_digits[pattern] = 0;
                    
                    for (int j=0; j<6; j++)
                    {
                        if (pattern[j] == dg_possibilities[0]) // matched on first. this is G and second is D
                        {
                            m_segment_matches[dg_possibilities[0]] = 'G';
                            m_segment_matches['G'] = dg_possibilities[0];
                            m_segment_matches[dg_possibilities[1]] = 'D';
                            m_segment_matches['D'] = dg_possibilities[1];
                            break;
                        }
                        if (pattern[j] == dg_possibilities[1]) // matched on first. this is G and first is D
                        {
                            m_segment_matches[dg_possibilities[1]] = 'G';
                            m_segment_matches['G'] = dg_possibilities[1];
                            m_segment_matches[dg_possibilities[0]] = 'D';
                            m_segment_matches['D'] = dg_possibilities[0];
                            break;
                        }
                    }
                    break;
                }
            }
        }
        
        // Loop through the six_patterns to find the element that isn't matched to anything. This is 9.
        {
            string zero = m_values[0];
            string six = m_values[6];
            for (int i=0; i<3; i++)
            {
                if (six_patterns[i] != zero && six_patterns[i] != six)
                {
                    m_values[9] = six_patterns[i];
                    m_digits[six_patterns[i]] = 9;
                    break;
                }
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
    
    int Display::get_output_value()
    {
        return 0;
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

string AocDay8::part2(string filename, vector<string> extra_args)
{
    vector<Display> displays;
    
    parse_input(filename, displays);
    
    int output_sum = 0;
    
    for (int i=0; i<displays.size(); i++)
    {
        displays[i].process_patterns();
        output_sum += displays[i].get_output_value();
    }
    
    ostringstream out;
    out << output_sum;
    return out.str();
}
