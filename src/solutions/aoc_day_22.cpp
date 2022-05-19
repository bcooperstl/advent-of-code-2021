#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_22.h"
#include "file_utils.h"

using namespace std;
using namespace Day22;

namespace Day22
{
    Row::Row()
    {
        m_head_pair = NULL;
    }
    
    Row::~Row()
    {
        OnPair * current = m_head_pair;
        OnPair * next;
        while (current != NULL)
        {
            next = current->next;
            delete current;
            current = next;
        }
    }
    
    void Row::turn_on(int min_x, int max_x)
    {
        cout << "    Turning on from " << min_x << " to " << max_x << endl;
        cout << "    BEFORE: ";
        display();
        
        if (m_head_pair == NULL)
        {
            cout << "    Creating first head pair" << endl;
            m_head_pair = new OnPair();
            m_head_pair->min = min_x;
            m_head_pair->max = max_x;
            m_head_pair->next = NULL;
        }
        else
        {
            // plan is to first place this item in order and then adjust as needed
            // check if it should be first
            if (min_x < m_head_pair->min)
            {
                cout << "    Creating new head pair" << endl;
                OnPair * new_head = new OnPair();
                new_head->min = min_x;
                new_head->max = max_x;
                new_head->next = m_head_pair;
                m_head_pair = new_head;
            }
            else
            {
                OnPair * current = m_head_pair;
                OnPair * next = current->next;
                bool added = false;
                while (next != NULL)
                {
                    if (min_x >= current->min && min_x < next->min)
                    {
                        cout << "    Creating pair between " << current->min << "-" << current->max << " and " << next->min << "-" << next->max << endl;
                        OnPair * pair = new OnPair();
                        pair->min = min_x;
                        pair->max = max_x;
                        pair->next = next;
                        current->next = pair;
                        added = true;
                        break;
                    }
                    current = next;
                    next = current->next;
                }
                if (!added)
                {
                    cout << "    Creating pair after final pair " << current->min << "-" << current->max << endl;
                    OnPair * pair = new OnPair();
                    pair->min = min_x;
                    pair->max = max_x;
                    pair->next = NULL;
                    current->next = pair;
                }
            }
            cout << "    INSERTED: ";
            display();
            // now combine as needed
            if (m_head_pair->next != NULL)
            {
                OnPair * current = m_head_pair;
                OnPair * next = current->next;
                while (next != NULL)
                {
                    // the minimum are the same. make current the larger range
                    if (current->min == next->min)
                    {
                        if (current->max < next->max)
                        {
                            current->max = next->max;
                        }
                        cout << "    Combining two equal current ranges" << endl;
                        current->next = next->next;
                        delete next;
                        next = current->next;
                    }
                    // at this point, we know next->min is larger than current->min
                    // if next's min is less than or equal to current's max, expand current to include next
                    else if (next->min <= current->max)
                    {
                        if (next->max <= current->max)
                        {
                            cout << "    Next is currently within current. deleting next" << endl;
                            {
                                current->next = next->next;
                                delete next;
                                next = current->next;
                            }
                        }
                        else
                        {
                            cout << "    Expanding current to include partially-overlapped next" << endl;
                            current->max = next->max;
                            current->next = next->next;
                            delete next;
                            next = current->next;
                        }
                    }
                    // if ranges are adjacent, combine them
                    else if (next->min == current->max + 1)
                    {
                        cout << "    Expanding to combine adjacent regions" << endl;
                        current->max = next->max;
                        current->next = next->next;
                        delete next;
                        next = current->next;
                    }
                    else
                    {
                        current = next;
                        next = current->next;
                    }
                }
            }
        }
        cout << "    AFTER: ";
        display();
        
    }
    
    void Row::turn_off(int min_x, int max_x)
    {
        cout << "    Turning off from " << min_x << " to " << max_x << endl;
        cout << "    BEFORE: ";
        display();
        
        // delete any we need to from the head pair that are fully in region to turn off
        while (m_head_pair != NULL && m_head_pair->min >= min_x && m_head_pair->max <= max_x)
        {
            cout << "    Deleting head pair of " << m_head_pair->min << "-" << m_head_pair->max << endl;
            OnPair * next = m_head_pair->next;
            delete m_head_pair;
            m_head_pair = next;
        }
        
        OnPair * current = m_head_pair;
        while (current != NULL)
        {
            // modify minimum of current if needed
            if (current->min >= min_x && current->min <= max_x)
            {
                cout << "    Incrementing minimum of " << current->min << "-" << current->max << " to " << max_x+1 << endl;
                current->min = max_x+1;
            }
            if (current->max >= min_x && current->max <= max_x)
            {
                cout << "    Decrementing maximum of " << current->min << "-" << current->max << " to " << min_x-1 << endl;
                current->max = min_x-1;
            }
            
            // split out to two ranges if needed
            if (current->min <= min_x && current->max >= max_x)
            {
                cout << "    Splitting current of " << current->min << "-" << current->max << " to " << current->min << "-" << min_x-1 << " and " << max_x+1 << "-" << current->max << endl;
                OnPair * second = new OnPair;
                second->max = current->max;
                second->next = current->next;
                second->min = max_x + 1;
                current->max = min_x - 1;
                current->next = second;
            }
                
            
            current = current->next;
        }
        cout << "    ADJUSTED: ";
        display();
        
        // delete any nodex where maximum < minimum 
        // from first logic, m_head_pair should never need to be deleted
        current = m_head_pair;
        while (current != NULL)
        {
            OnPair * next = current->next;
            if (next != NULL && next->max < next->min)
            {
                cout << "    Deleting invalid pair " << endl;
                current->next = next->next;
                delete next;
            }
            current = current->next;
        }
        
        cout << "    AFTER: ";
        display();
    }
    
    long Row::get_count_on()
    {
        long count = 0;
        OnPair * current = m_head_pair;
        while (current != NULL)
        {
            count += (long)(current->max - current->min + 1); // need to add 1 to get the missing end
            current = current->next;
        }
        return count;
    }
    
    void Row::display()
    {
        if (m_head_pair == NULL)
        {
            cout << "   Empty row" << endl;
        }
        else
        {
            OnPair * current = m_head_pair;
            while (current != NULL)
            {
                cout << "   [" << current->min << "," << current->max << "]  ";
                current = current->next;
            }
            cout << endl;
        }
    }
    
    Plane::Plane()
    {
    }
    
    Plane::~Plane()
    {
        map<int, Row *>::iterator pos = m_rows.begin();
        while (pos != m_rows.end())
        {
            delete pos->second;
            ++pos;
        }
    }
    
    void Plane::turn_on(int min_y, int max_y, int min_x, int max_x)
    {
        cout << " Turning on with rows y from " << min_y << " to " << max_y << endl;
        for (int y=min_y; y<=max_y; y++)
        {
            map<int, Row *>::iterator row_iter = m_rows.find(y);
            Row * row;
            if (row_iter == m_rows.end())
            {
                cout << "  Creating row y=" << y << endl;
                row = new Row();
                m_rows[y] = row;
            }
            else
            {
                cout << "  Using existing row y=" << y << endl;
                row = row_iter->second;
            }
            row->turn_on(min_x, max_x);
        }
    }
    
    void Plane::turn_off(int min_y, int max_y, int min_x, int max_x)
    {
        for (int y=min_y; y<=max_y; y++)
        {
            map<int, Row *>::iterator row_iter = m_rows.find(y);
            if (row_iter != m_rows.end())
            {
                cout << "  Turning off row y=" << y << endl;
                row_iter->second->turn_off(min_x, max_x);
            }
            else
            {
                cout << "  Skipping missing row y=" << y << endl;
            }
        }
    }
    
    long Plane::get_count_on()
    {
        long count = 0;
        map<int, Row *>::iterator pos = m_rows.begin();
        while (pos != m_rows.end())
        {
            count+=pos->second->get_count_on();
            ++pos;
        }
        return count;
    }
    
    Space::Space()
    {
        m_head = NULL;
        m_tail = NULL;
    }
    
    Space::~Space()
    {
        map<int, Plane *>::iterator pos = m_planes.begin();
        while (pos != m_planes.end())
        {
            delete pos->second;
            ++pos;
        }
        
        OnSpace * current = m_head;
        while (current != NULL)
        {
            OnSpace * next = current->next;
            delete current;
            current = next;
        }
    }
    
    void Space::dump_space()
    {
        if (m_head == NULL)
        {
            cout << "SPACE:       No elements" << endl;
        }
        else
        {
            cout << "SPACE:       Displaying elements" << endl;
            OnSpace * current = m_head;
            while (current != NULL)
            {
                cout << "SPACE:         z from " << current->min_z << "-" << current->max_z 
                << " and y from " << current->min_y << "-" << current->max_y
                << " and x from " << current->min_x << "-" << current->max_x << endl;
                current = current->next;
            }
            cout << "SPACE:       Done" << endl;
        }
    }
    
    void Space::turn_on(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x)
    {
        cout << "Turning on with planes z from " << min_z << " to " << max_z << endl;
        for (int z=min_z; z<=max_z; z++)
        {
            map<int, Plane *>::iterator plane_iter = m_planes.find(z);
            Plane * plane;
            if (plane_iter == m_planes.end())
            {
                cout << " Creating plane z=" << z << endl;
                plane = new Plane();
                m_planes[z] = plane;
            }
            else
            {
                cout << " Using existing plane z=" << z << endl;
                plane = plane_iter->second;
            }
            plane->turn_on(min_y, max_y, min_x, max_x);
        }
        
        // use the same turn_off_space code to remove anything in this cube, so that I can insert it as a whole one
        turn_off_space(min_z, max_z, min_y, max_y, min_x, max_x);
        
        // now that the prior on areas have been modified to turn off overlapping cells, create the new On section and put it at the end
        OnSpace * new_space = new OnSpace();
        new_space->min_x = min_x;
        new_space->max_x = max_x;
        new_space->min_y = min_y;
        new_space->max_y = max_y;
        new_space->min_z = min_z;
        new_space->max_z = max_z;
        
        cout << "SPACE: Created new element with z from " << min_z << "-" << max_z 
             << " and y from " << min_y << "-" << max_y
             << " and x from " << min_x << "-" << max_x << endl;
        
        if (m_head == NULL)
        {
            cout << "SPACE:  Head is not defined - Creating first on element" << endl;
            m_head = new_space;
            m_tail = new_space;
            new_space->prev = NULL;
            new_space->next = NULL;
        }
        else
        {
            cout << "SPACE:  Head and tail defined - Adding as last element" << endl;
            m_tail->next = new_space;
            new_space->prev = m_tail;
            new_space->next = NULL;
            m_tail = new_space;
        }
        cout << "SPACE:   Count is " << get_count_on() << endl;
        dump_space();
        if (get_count_on() != get_count_on_old())
        {
            cout << "*************MISMATCH HERE**************" << endl;
        }
    }
    
    void Space::turn_off(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x)
    {
        cout << "Turning off with planes z from " << min_z << " to " << max_z << endl;
        for (int z=min_z; z<=max_z; z++)
        {
            map<int, Plane *>::iterator plane_iter = m_planes.find(z);
            if (plane_iter != m_planes.end())
            {
                cout << " Turning off plane z=" << z << endl;
                plane_iter->second->turn_off(min_y, max_y, min_x, max_x);
            }
            else
            {
                cout << " Skipping missing plane z=" << z << endl;
            }
        }
        turn_off_space(min_z, max_z, min_y, max_y, min_x, max_x);
        cout << "SPACE:   Count is " << get_count_on() << endl;
        dump_space();
        if (get_count_on() != get_count_on_old())
        {
            cout << "*************MISMATCH HERE**************" << endl;
        }
    }
    
    void Space::set_on_off_coordinates(int off_min, int off_max, int space_min, int space_max, OnOffCoordinates & coordinates)
    {
        coordinates.off_min = (off_min >= space_min ? off_min : space_min);
        coordinates.off_max = (off_max <= space_max ? off_max : space_max);

        int off_on_min = off_min - 1; // the on value just below the off_min
        int off_on_max = off_max + 1; // the on value just above the off_max
        
        if (space_min <= off_on_min)
        {
            coordinates.use_low_on = true;
            coordinates.low_on_min = space_min;
            coordinates.low_on_max = off_on_min;
        }
        else
        {
            coordinates.use_low_on = false;
        }
        
        if (space_max >= off_on_max)
        {
            coordinates.use_high_on = true;
            coordinates.high_on_min = off_on_max;
            coordinates.high_on_max = space_max;
        }
        else
        {
            coordinates.use_high_on = false;
        }
    }
    
    void Space::create_on_space(int min_x, int max_x, int min_y, int max_y, int min_z, int max_z, OnSpace ** head, OnSpace ** current)
    {
        cout << "SPACE:     Creating Space z from " << min_z << "-" << max_z 
             << " and y from " << min_y << "-" << max_y
             << " and x from " << min_x << "-" << max_x << endl;
        OnSpace * space = new OnSpace();
        space->min_x = min_x;
        space->max_x = max_x;
        space->min_y = min_y;
        space->max_y = max_y;
        space->min_z = min_z;
        space->max_z = max_z;
        space->prev = *current;
        space->next = NULL;
        if (*current != NULL)
        {
            cout << "Setting current" << endl;
            (*current)->next = space;
        }
        if (*head == NULL)
        {
            cout << "Setting head" << endl;
            *head = space;
        }
        *current = space;
        return;
    }
    
    void Space::make_spaces_from_coordinates(OnOffCoordinates x_coordinates, OnOffCoordinates y_coordinates, OnOffCoordinates z_coordinates, OnSpace ** first, OnSpace ** last)
    {
        OnSpace * ret_head = NULL;
        OnSpace * current = NULL;
        // Make up to 26 spaces
        // X low
        if (x_coordinates.use_low_on)
        {
            // Y low
            if (y_coordinates.use_low_on)
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x low/y low/z low" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x low/y low/z off" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x low/y low/z high" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
            // Y mid
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x low/y off/z low" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x low/y off/z off" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x low/y off/z high" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
            // Y high
            if (y_coordinates.use_high_on)
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x low/y high/z low" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x low/y high/z off" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x low/y high/z high" << endl;
                    create_on_space(x_coordinates.low_on_min, x_coordinates.low_on_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
        }
        // X mid
        {
            // Y low
            if (y_coordinates.use_low_on)
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x mid/y low/z low" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x mid/y low/z off" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x mid/y low/z high" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
            // Y mid
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x mid/y off/z low" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid - this is the area being turned off. skip it
                {
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x mid/y off/z high" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
            // Y high
            if (y_coordinates.use_high_on)
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x mid/y high/z low" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x mid/y high/z off" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x mid/y high/z high" << endl;
                    create_on_space(x_coordinates.off_min, x_coordinates.off_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
        }            
        // X high
        if (x_coordinates.use_high_on)
        {
            // Y low
            if (y_coordinates.use_low_on)
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x high/y low/z low" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x high/y low/z off" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x high/y low/z high" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.low_on_min, y_coordinates.low_on_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
            // Y mid
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x high/y off/z low" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x high/y off/z off" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x high/y off/z high" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.off_min, y_coordinates.off_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
            // Y high
            if (y_coordinates.use_high_on)
            {
                // Z low
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:     Creating x high/y high/z low" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.low_on_min, z_coordinates.low_on_max, &ret_head, &current);
                }
                // Z mid
                {
                    cout << "SPACE:     Creating x high/y high/z off" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.off_min, z_coordinates.off_max, &ret_head, &current);
                }
                // Z high
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:     Creating x high/y high/z high" << endl;
                    create_on_space(x_coordinates.high_on_min, x_coordinates.high_on_max, y_coordinates.high_on_min, y_coordinates.high_on_max, z_coordinates.high_on_min, z_coordinates.high_on_max, &ret_head, &current);
                }
            }
        }
        *first = ret_head;
        *last = current;
        return;
    }
    
    void Space::turn_off_space(int min_z, int max_z, int min_y, int max_y, int min_x, int max_x)
    {
        cout << "SPACE: Turning off with z from " << min_z << "-" << max_z 
             << " and y from " << min_y << "-" << max_y
             << " and x from " << min_x << "-" << max_x << endl;
        if (m_head == NULL)
        {
            cout << "SPACE:  No head defined - nothing to turn off" << endl;
            return;
        }
        
        // need to compare each node in the OnSpace list to see if it has to be turned off or not
        OnSpace * current = m_head;
        while (current != NULL)
        {
            cout << "SPACE:  Comparing against z from " << current->min_z << "-" << current->max_z 
             << " and y from " << current->min_y << "-" << current->max_y
             << " and x from " << current->min_x << "-" << current->max_x << endl;
            
            if (((current->min_x >= min_x && current->min_x <= max_x) || (current->max_x >= min_x && current->max_x <= max_x)) &&
                ((current->min_y >= min_y && current->min_y <= max_y) || (current->max_y >= min_y && current->max_y <= max_y)) &&
                ((current->min_z >= min_z && current->min_z <= max_z) || (current->max_z >= min_z && current->max_z <= max_z)))
            {
                cout << "SPACE:   There is overlap" << endl;
                OnOffCoordinates x_coordinates, y_coordinates, z_coordinates;
                set_on_off_coordinates(min_x, max_x, current->min_x, current->max_x, x_coordinates);
                set_on_off_coordinates(min_y, max_y, current->min_y, current->max_y, y_coordinates);
                set_on_off_coordinates(min_z, max_z, current->min_z, current->max_z, z_coordinates);
                
                if (x_coordinates.use_low_on)
                {
                    cout << "SPACE:    X low on is true; will stay on from " << x_coordinates.low_on_min << "-" << x_coordinates.low_on_max << endl;
                }
                else
                {
                    cout << "SPACE:    X low on is false; no need to keep the low side" << endl;
                }
                if (x_coordinates.use_high_on)
                {
                    cout << "SPACE:    X high on is true; will stay on from " << x_coordinates.high_on_min << "-" << x_coordinates.high_on_max << endl;
                }
                else
                {
                    cout << "SPACE:    X high on is false; no need to keep the high side" << endl;
                }
                if (y_coordinates.use_low_on)
                {
                    cout << "SPACE:    Y low on is true; will stay on from " << y_coordinates.low_on_min << "-" << y_coordinates.low_on_max << endl;
                }
                else
                {
                    cout << "SPACE:    Y low on is false; no need to keep the low side" << endl;
                }
                if (y_coordinates.use_high_on)
                {
                    cout << "SPACE:    Y high on is true; will stay on from " << y_coordinates.high_on_min << "-" << y_coordinates.high_on_max << endl;
                }
                else
                {
                    cout << "SPACE:    Y high on is false; no need to keep the high side" << endl;
                }
                if (z_coordinates.use_low_on)
                {
                    cout << "SPACE:    Z low on is true; will stay on from " << z_coordinates.low_on_min << "-" << z_coordinates.low_on_max << endl;
                }
                else
                {
                    cout << "SPACE:    Z low on is false; no need to keep the low side" << endl;
                }
                if (z_coordinates.use_high_on)
                {
                    cout << "SPACE:    Z high on is true; will stay on from " << z_coordinates.high_on_min << "-" << z_coordinates.high_on_max << endl;
                }
                else
                {
                    cout << "SPACE:    Z high on is false; no need to keep the high side" << endl;
                }
                
                OnSpace * repl_head = NULL;
                OnSpace * repl_tail = NULL;
                make_spaces_from_coordinates(x_coordinates, y_coordinates, z_coordinates, &repl_head, &repl_tail);
                
                cout << repl_head << " " << repl_tail << endl;
                if (repl_head == NULL && repl_tail == NULL)
                {
                    bool start_at_head = false;
                    cout << "SPACE:    No elements left after turning off" << endl;
                    if (current == m_tail)
                    {
                        m_tail = current->prev;
                    }
                    if (current->prev != NULL)
                    {
                        current->prev->next = current->next;
                    }
                    if (current->next != NULL)
                    {
                        current->next->prev = current->prev;
                    }

                    if (current == m_head)
                    {
                        m_head = current->next;
                        delete current;
                        current = m_head; // start at the head on the next time throgh
                    }
                    else
                    {
                        OnSpace * next = current->next;
                        delete current;
                        current = next;
                    }
                }
                else
                {
                    // we now have a linked list from repl_head to repl_tail that we need to sub in place for current
                    // 1) fix up and head/tail variables
                    if (current == m_head)
                    {
                        m_head = repl_head;
                    }
                    if (current == m_tail)
                    {
                        m_tail = repl_tail;
                    }
                    
                    // link the prior element and the head of our new list
                    repl_head->prev = current->prev;
                    if (current->prev != NULL)
                    {
                        current->prev->next = repl_head;
                    }
                    
                    // link the next element and the tail of our new list
                    repl_tail->next = current->next;
                    if (current->next != NULL)
                    {
                        current->next->prev = repl_tail;
                    }
                
                    // finally - delete the current member and set it to be the last one we created. it'll increment outside this if
                    delete current;
                    current = repl_tail->next;
                }
                
            }
            else
            {
                cout << "SPACE:   There is no overlap. Skipping to next space" << endl;
                current = current->next;
            }
        }    
    }
    
    long Space::get_count_on_old()
    {
        long count = 0;
        map<int, Plane *>::iterator pos = m_planes.begin();
        while (pos != m_planes.end())
        {
            count+=pos->second->get_count_on();
            ++pos;
        }
        return count;
    }
    
    long Space::get_count_on(OnSpace * space)
    {
        return ((space->max_x - space->min_x + 1) * (space->max_y - space->min_y + 1) * (space->max_z - space->min_z + 1));
    }
    
    long Space::get_count_on()
    {
        long count = 0;
        OnSpace * current = m_head;
        while (current != NULL)
        {
            count+=get_count_on(current);
            current = current->next;
        }
        return count;
    }
    
    
        
};

AocDay22::AocDay22():AocDay(22)
{
}

AocDay22::~AocDay22()
{
}

/*
off x=9..11,y=9..11,z=9..11
*/

vector<Step> AocDay22::parse_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    vector<Step> steps;
    
    char delimiters[4];
    delimiters[0] = ' ';
    delimiters[1] = '.';
    delimiters[2] = ',';
    delimiters[3] = '=';
    
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delimiters, 4, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return steps;
    }
    
    for (int i=0; i<lines.size(); i++)
    {
        Step step;
        step.method = lines[i][0];
        step.min_x = strtol(lines[i][2].c_str(), NULL, 10);
        step.max_x = strtol(lines[i][3].c_str(), NULL, 10);
        step.min_y = strtol(lines[i][5].c_str(), NULL, 10);
        step.max_y = strtol(lines[i][6].c_str(), NULL, 10);
        step.min_z = strtol(lines[i][8].c_str(), NULL, 10);
        step.max_z = strtol(lines[i][9].c_str(), NULL, 10);
        steps.push_back(step);
    }
    return steps;
}

string AocDay22::part1(string filename, vector<string> extra_args)
{
    Space space;
    vector<Step> steps = parse_input(filename);
    
    for (int i=0; i<steps.size(); i++)
    {
        if (steps[i].min_x >= -50 && steps[i].max_x <= 50 &&
            steps[i].min_y >= -50 && steps[i].max_y <= 50 &&
            steps[i].min_z >= -50 && steps[i].max_z <= 50)
        {
            if (steps[i].method == "on")
            {
                space.turn_on(steps[i].min_z, steps[i].max_z, steps[i].min_y, steps[i].max_y, steps[i].min_x, steps[i].max_x);
            }
            else
            {
                space.turn_off(steps[i].min_z, steps[i].max_z, steps[i].min_y, steps[i].max_y, steps[i].min_x, steps[i].max_x);
            }
        }
    }
    
    ostringstream out;
    out << space.get_count_on();
    return out.str();
}
