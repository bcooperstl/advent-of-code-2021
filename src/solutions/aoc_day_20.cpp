#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include "aoc_day_20.h"
#include "file_utils.h"
#include "screen.h"

#define ALGORITHM_LENGTH 512

#define INDEX_INFINITY_DARK 0
#define INDEX_INFINITY_LIGHT 511

using namespace std;

AocDay20::AocDay20():AocDay(20)
{
}

AocDay20::~AocDay20()
{
}

void AocDay20::parse_input(string filename, char * enhancement_algorithm, Screen & image)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    strncpy(enhancement_algorithm, lines[0].c_str(), ALGORITHM_LENGTH + 1);
    
    lines.erase(lines.begin(), lines.begin()+2); // erase the first two lines
    
    image.load(lines, 0-lines[0].length()/2, 0-lines.size()/2); // center this at 0,0 for some reason
    
    cout << "Loaded image as:" << endl;
    image.display();
    
    return;
}

void AocDay20::process_enhancement(char * enhancement_algorithm, Screen & image, char & infinity_color)
{
    int init_height = image.get_height();
    int init_width = image.get_width();
    int init_min_x = image.get_min_x();
    int init_max_x = image.get_max_x();
    int init_min_y = image.get_min_y();
    int init_max_y = image.get_max_y();
    
    int ** algorithm_index;
    algorithm_index = new int *[init_height];
    for (int i=0; i<init_height; i++)
    {
        algorithm_index[i] = new int [init_width];
        for (int j=0; j<init_width; j++)
        {
            algorithm_index[i][j]=0;
        }
    }
    
    image.expand(infinity_color);
    cout << endl;
    for (int y=init_min_y; y<=init_max_y; y++)
    {
        for (int x=init_min_x; x<=init_max_x; x++)
        {
            // top-left - bit B00000000
            char val = image.get(x-1,y-1);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=256;
            }
            // top-center - bit 0B0000000
            val = image.get(x, y-1);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=128;
            }
            // top-right - bit 00B000000
            val = image.get(x+1, y-1);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=64;
            }
            // mid-left - bit 000B00000
            val = image.get(x-1,y);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=32;
            }
            // mid-center - bit 0000B0000
            val = image.get(x, y);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=16;
            }
            // mid-right - bit 00000B000
            val = image.get(x+1, y);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=8;
            }
            // bottom-left - bit 000000B00
            val = image.get(x-1,y+1);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=4;
            }
            // bottom-center - bit 0000000B0
            val = image.get(x, y+1);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=2;
            }
            // bottom-right - bit 00000000B
            val = image.get(x+1, y+1);
            if (val == PIXEL_LIGHT)
            {
                algorithm_index[y-init_min_y][x-init_min_x]+=1;
            }
        }
    }

    for (int y=init_min_y; y<=init_max_y; y++)
    {
        for (int x=init_min_x; x<=init_max_x; x++)
        {
            image.set(x, y, enhancement_algorithm[algorithm_index[y-init_min_y][x-init_min_x]]);
        }
    }
    
    if (infinity_color == PIXEL_LIGHT)
    {
        infinity_color = enhancement_algorithm[INDEX_INFINITY_LIGHT];
    }
    else // infinity_color == PIXEL_DARK
    {
        infinity_color = enhancement_algorithm[INDEX_INFINITY_DARK];
    }
    
    // now need to set the expanded border to the new infinity color
    int expand_min_x = image.get_min_x();
    int expand_max_x = image.get_max_x();
    int expand_min_y = image.get_min_y();
    int expand_max_y = image.get_max_y();
    
    // set the top and the bottom rows
    for (int x=expand_min_x; x<=expand_max_x; x++)
    {
        image.set(x,expand_min_y,infinity_color);
        image.set(x,expand_max_y,infinity_color);
    }
    
    // set the left and the right
    for (int y=expand_min_y+1; y<=expand_max_y-1; y++) // can skip the top and bottom rows - already set
    {
        image.set(expand_min_x,y,infinity_color);
        image.set(expand_max_x,y,infinity_color);
    }
    
    for (int j=0; j<init_height; j++)
    {
        delete [] algorithm_index[j];
    }
    delete [] algorithm_index;
}

string AocDay20::part1(string filename, vector<string> extra_args)
{
    char enhancement_algorithm[ALGORITHM_LENGTH+1];
    Screen image;
    char infinity_color = PIXEL_DARK;
    
    parse_input(filename, enhancement_algorithm, image);
    
    cout << "Before expansion, image has height " << image.get_height() << " from " << image.get_min_y() << " to " << image.get_max_y()
         << " and width " << image.get_width() << " from " << image.get_min_x() << " to " << image.get_max_x() << endl;
    
    image.display();
    
    cout << "After expansion, image has height " << image.get_height() << " from " << image.get_min_y() << " to " << image.get_max_y()
         << " and width " << image.get_width() << " from " << image.get_min_x() << " to " << image.get_max_x() << endl;
    
    image.expand(infinity_color);
    
    for (int i=0; i<2; i++)
    {
        process_enhancement(enhancement_algorithm, image, infinity_color);
        image.display();
        cout << "After round " << i+1 << ", image has height " << image.get_height() << " from " << image.get_min_y() << " to " << image.get_max_y()
            << " and width " << image.get_width() << " from " << image.get_min_x() << " to " << image.get_max_x() << endl;
    }
    
    ostringstream out;
    out << image.num_matching(PIXEL_LIGHT);
    return out.str();
}

string AocDay20::part2(string filename, vector<string> extra_args)
{
    char enhancement_algorithm[ALGORITHM_LENGTH+1];
    Screen image;
    char infinity_color = PIXEL_DARK;
    
    parse_input(filename, enhancement_algorithm, image);
    
    cout << "Before expansion, image has height " << image.get_height() << " from " << image.get_min_y() << " to " << image.get_max_y()
         << " and width " << image.get_width() << " from " << image.get_min_x() << " to " << image.get_max_x() << endl;
    
    image.display();
    
    cout << "After expansion, image has height " << image.get_height() << " from " << image.get_min_y() << " to " << image.get_max_y()
         << " and width " << image.get_width() << " from " << image.get_min_x() << " to " << image.get_max_x() << endl;
    
    image.expand(infinity_color);
    
    for (int i=0; i<50; i++)
    {
        process_enhancement(enhancement_algorithm, image, infinity_color);
        image.display();
        cout << "After round " << i+1 << ", image has height " << image.get_height() << " from " << image.get_min_y() << " to " << image.get_max_y()
            << " and width " << image.get_width() << " from " << image.get_min_x() << " to " << image.get_max_x() << endl;
    }
    
    ostringstream out;
    out << image.num_matching(PIXEL_LIGHT);
    return out.str();
}