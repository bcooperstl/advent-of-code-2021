#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_19.h"
#include "file_utils.h"

#define COMPUTE_DISTANCE(L,R) (((L.x-R.x)*(L.x-R.x))+((L.y-R.y)*(L.y-R.y))+((L.z-R.z)*(L.z-R.z)))

using namespace std;
using namespace Day19;

namespace Day19
{
    ObservedBeacon::ObservedBeacon(int x, int y, int z, Scanner * owner)
    {
        m_coordinates.x = x;
        m_coordinates.y = y;
        m_coordinates.z = z;
        m_owner = owner;
        m_actual_beacon = NULL;
    }
    
    ObservedBeacon::~ObservedBeacon()
    {
    }
    
    Coordinates ObservedBeacon::get_coordinates()
    {
        return m_coordinates;
    }
    
    Scanner * ObservedBeacon::get_owner()
    {
        return m_owner;
    }
    
    void ObservedBeacon::set_actual_beacon(ActualBeacon * beacon)
    {
        m_actual_beacon = beacon;
    }
    
    ActualBeacon * ObservedBeacon::get_actual_beacon()
    {
        return m_actual_beacon;
    }
    
    // void BeaconPairDistance::calculate_simple_distance()
    // {
        // Coordinates first = m_beacons[0]->get_coordinates();
        // Coordinates second = m_beacons[1]->get_coordinates();
        
        // m_simple_distance = ((first.x-second.x)*(first.x-second.x)) +
                            // ((first.y-second.y)*(first.y-second.y)) +
                            // ((first.z-second.z)*(first.z-second.z));
    // }
    
    ActualBeacon::ActualBeacon(int x, int y, int z)
    {
        m_coordinates.x = x;
        m_coordinates.y = y;
        m_coordinates.z = z;
    }
    
    ActualBeacon::ActualBeacon(Coordinates coordinates)
    {
        m_coordinates = coordinates;
    }
    
    ActualBeacon::~ActualBeacon()
    {
    }
    
    Coordinates ActualBeacon::get_coordinates()
    {
        return m_coordinates;
    }
    
    void ActualBeacon::add_corresponding_beacon(ObservedBeacon * beacon)
    {
        m_corresponding_beacons.push_back(beacon);
    }
    
    vector<ObservedBeacon *> ActualBeacon::get_corresponding_beacons()
    {
        return m_corresponding_beacons;
    }
    
    Scanner::Scanner(int number)
    {
        m_number = number;
        // all appear to be at 0,0,0 to themselves
        m_observed_coordinates.x = 0;
        m_observed_coordinates.y = 0;
        m_observed_coordinates.z = 0;
        m_actual_determined = false;
    }
    
    Scanner::~Scanner()
    {
        for (int i=0; i<m_beacons.size(); i++)
        {
            delete m_beacons[i];
        }
    }
    
    int Scanner::get_number()
    {
        return m_number;
    }
    
    void Scanner::add_beacon(ObservedBeacon * beacon)
    {
        m_beacons.push_back(beacon);
    }
    
    vector<ObservedBeacon *> Scanner::get_beacons()
    {
        return m_beacons;
    }
    
    Coordinates Scanner::get_observed_coordinates()
    {
        return m_observed_coordinates;
    }
    
    void Scanner::set_actual_coordinates(Coordinates coordinates)
    {
        m_actual_coordinates = coordinates;
        m_actual_determined = true;
    }
    
    Coordinates Scanner::get_actual_coordinates()
    {
        return m_actual_coordinates;
    }
    
    bool Scanner::get_actual_determined()
    {
        return m_actual_determined;
    }
    
    Region::Region()
    {
        m_num_scanners = 0;
    }
    
    Region::~Region()
    {
        for (int i=0; i<m_num_scanners; i++)
        {
            delete m_scanners[i];
        }
        
        for (int i=0; i<m_actual_beacons.size(); i++)
        {
            delete m_actual_beacons[i];
        }
    }
    
    void Region::set_scanners(vector<Scanner *> scanners)
    {
        m_num_scanners = scanners.size();
        for (int i=0; i<m_num_scanners; i++)
        {
            Scanner * current = scanners[i];
            m_scanners[current->get_number()]=current;
            //cout << "Scanner " << i << " has " << current->get_beacons().size() << " beacons" << endl;
        }
        cout << "Added " << m_num_scanners << " scanners" << endl;
    }
    
    Scanner * Region::get_scanner(int number)
    {
        return m_scanners[number];
    }
    
    vector<ActualBeacon *> Region::get_actual_beacons()
    {
        return m_actual_beacons;
    }
    
    void Region::add_actual_beacon(ActualBeacon * beacon)
    {
        m_actual_beacons.push_back(beacon);
    }
    
    void Region::set_scanner_zero_as_origin()
    {
        cout << "Setting scanner zero to origin" << endl;
        Scanner * zero = m_scanners[0];
        zero->set_actual_coordinates(zero->get_observed_coordinates());
        cout << " Set scanners actual location to 0,0,0" << endl;
        vector<ObservedBeacon *> observed_beacons = zero->get_beacons();
        for (int i=0; i<observed_beacons.size(); i++)
        {
            ObservedBeacon * observed = observed_beacons[i];
            Coordinates coordinates = observed->get_coordinates();
            ActualBeacon * actual = new ActualBeacon(coordinates);
            observed->set_actual_beacon(actual);
            actual->add_corresponding_beacon(observed);
            add_actual_beacon(actual);
            cout << " Added beacon at " << coordinates.x << "," << coordinates.y << "," << coordinates.z << endl;
        }
    }
    
    bool Region::not_all_determined()
    {
        for (int i=0; i<m_num_scanners; i++)
        {
            if (!m_scanners[i]->get_actual_determined())
            {
                return true;
            }
        }
        return false;
    }
    
    void Region::map_overlaps()
    {
        while (not_all_determined())
        {
            for (int det_pos=0; det_pos < m_num_scanners; det_pos++)
            {
                for (int undet_pos=0; undet_pos < m_num_scanners; undet_pos++)
                {
                    if (m_scanners[det_pos]->get_actual_determined() && !m_scanners[undet_pos]->get_actual_determined())
                    {
                        check_overlap(m_scanners[det_pos], m_scanners[undet_pos]);
                    }
                }
            }
            break; // for testing
        }
    }
    
    void Region::check_overlap(Scanner * mapped, Scanner * unmapped)
    {
        cout << "Checking for overlap between mapped scanner " << mapped->get_number() << " and unmapped scanner " << unmapped->get_number() << endl;
        
        ActualBeacon * mapped_actual_beacons[MAX_SCANNERS];
        int num_mapped_actual_beacons;
        
        
        vector<ObservedBeacon *> tmp = mapped->get_beacons();
        num_mapped_actual_beacons = tmp.size();
        for (int i=0; i<num_mapped_actual_beacons; i++)
        {
            mapped_actual_beacons[i] = tmp[i]->get_actual_beacon();
        }
        
        ObservedBeacon * unmapped_beacons[MAX_SCANNERS];
        int num_unmapped_beacons;

        tmp = unmapped->get_beacons();
        num_unmapped_beacons = tmp.size();
        for (int i=0; i<num_unmapped_beacons; i++)
        {
            unmapped_beacons[i] = tmp[i];
        }
        
        // TODO: If speedup is needed, Cache These in Scanner object
        BeaconDistances actual_distances[MAX_SCANNERS][MAX_SCANNERS];
        BeaconDistances unmapped_distances[MAX_SCANNERS][MAX_SCANNERS];
        
        Coordinates actual_coordinates[MAX_SCANNERS];
        for (int i=0; i<num_mapped_actual_beacons; i++)
        {
            actual_coordinates[i] = mapped_actual_beacons[i]->get_coordinates();
        }
        
        for (int i=0; i<num_mapped_actual_beacons; i++)
        {
            for (int j=0; j<num_mapped_actual_beacons; j++)
            {
                actual_distances[i][j].delta_x = actual_coordinates[i].x-actual_coordinates[j].x;
                actual_distances[i][j].delta_y = actual_coordinates[i].y-actual_coordinates[j].y;
                actual_distances[i][j].delta_z = actual_coordinates[i].z-actual_coordinates[j].z;
                actual_distances[i][j].computed_distance = COMPUTE_DISTANCE(actual_coordinates[i], actual_coordinates[j]);
                cout << " The distance between " << actual_coordinates[i].x << "," << actual_coordinates[i].y << "," << actual_coordinates[i].z << " and "
                                                 << actual_coordinates[j].x << "," << actual_coordinates[j].y << "," << actual_coordinates[j].z << " is " << actual_distances[i][j].computed_distance << endl;
            }
        }            
        
        Coordinates unmapped_coordinates[MAX_SCANNERS];
        for (int i=0; i<num_unmapped_beacons; i++)
        {
            unmapped_coordinates[i] = unmapped_beacons[i]->get_coordinates();
        }
        
        for (int i=0; i<num_unmapped_beacons; i++)
        {
            for (int j=0; j<num_unmapped_beacons; j++)
            {
                unmapped_distances[i][j].delta_x = unmapped_coordinates[i].x-unmapped_coordinates[j].x;
                unmapped_distances[i][j].delta_y = unmapped_coordinates[i].y-unmapped_coordinates[j].y;
                unmapped_distances[i][j].delta_z = unmapped_coordinates[i].z-unmapped_coordinates[j].z;
                unmapped_distances[i][j].computed_distance = COMPUTE_DISTANCE(unmapped_coordinates[i], unmapped_coordinates[j]);
                cout << " The distance between " << unmapped_coordinates[i].x << "," << unmapped_coordinates[i].y << "," << unmapped_coordinates[i].z << " and "
                                                 << unmapped_coordinates[j].x << "," << unmapped_coordinates[j].y << "," << unmapped_coordinates[j].z << " is " << unmapped_distances[i][j].computed_distance << endl;
            }
        }
        
        for (int i=0; i<num_mapped_actual_beacons; i++)
        {
            for (int j=0; j<num_mapped_actual_beacons; j++)
            {
                if (i==j) // skip if same actual beacon
                {
                    continue;
                }
                for (int k=0; k<num_unmapped_beacons; k++)
                {
                    for (int l=0; l<num_unmapped_beacons; l++)
                    {
                        if (k==l) // skip if same unmapped
                        {
                            continue;
                        }
                        if (actual_distances[i][j].computed_distance == unmapped_distances[k][l].computed_distance)
                        {
                            cout << " Distance match between mapped "
                                 << actual_coordinates[i].x << "," << actual_coordinates[i].y << "," << actual_coordinates[i].z << " and "
                                 << actual_coordinates[j].x << "," << actual_coordinates[j].y << "," << actual_coordinates[j].z << "  is " 
                                 << actual_distances[i][j].delta_x << "," << actual_distances[i][j].delta_y << "," << actual_distances[i][j].delta_z << " = " << actual_distances[i][j].computed_distance 
                                 << endl << "  and unmapped "
                                 << unmapped_coordinates[k].x << "," << unmapped_coordinates[k].y << "," << unmapped_coordinates[k].z << " and "
                                 << unmapped_coordinates[l].x << "," << unmapped_coordinates[l].y << "," << unmapped_coordinates[l].z << " is "
                                 << unmapped_distances[k][l].delta_x << "," << unmapped_distances[k][l].delta_y << "," << unmapped_distances[k][l].delta_z << " = " << unmapped_distances[k][l].computed_distance << endl;
                            
                            int actual_delta_x = abs(actual_distances[i][j].delta_x);
                            int actual_delta_y = abs(actual_distances[i][j].delta_y);
                            int actual_delta_z = abs(actual_distances[i][j].delta_z);
                            int unmapped_delta_x = abs(unmapped_distances[k][l].delta_x);
                            int unmapped_delta_y = abs(unmapped_distances[k][l].delta_y);
                            int unmapped_delta_z = abs(unmapped_distances[k][l].delta_z);
                            
                            if (actual_delta_x == unmapped_delta_x && actual_delta_y == unmapped_delta_y && actual_delta_z == unmapped_delta_z)
                            {
                                cout << "  Case 1: where x1==x2, y1==y2, z1==z2" << endl;
                            }
                            else if (actual_delta_x == unmapped_delta_x && actual_delta_y == unmapped_delta_z && actual_delta_z == unmapped_delta_y)
                            {
                                cout << "  Case 2: where x1==x2, y1==z2, z1==y2" << endl;
                            }
                            else if (actual_delta_x == unmapped_delta_y && actual_delta_y == unmapped_delta_x && actual_delta_z == unmapped_delta_z)
                            {
                                cout << "  Case 3: where x1==y2, y1==x2, z1==z2" << endl;
                            }
                            else if (actual_delta_x == unmapped_delta_y && actual_delta_y == unmapped_delta_z && actual_delta_z == unmapped_delta_x)
                            {
                                cout << "  Case 4: where x1==y2, y1==z2, z1==x2" << endl;
                            }
                            else if (actual_delta_x == unmapped_delta_z && actual_delta_y == unmapped_delta_x && actual_delta_z == unmapped_delta_y)
                            {
                                cout << "  Case 5: where x1==z2, y1==x2, z1==y2" << endl;
                            }
                            else if (actual_delta_x == unmapped_delta_z && actual_delta_y == unmapped_delta_y && actual_delta_z == unmapped_delta_x)
                            {
                                cout << "  Case 6: where x1==z2, y1==y2, z1==x2" << endl;
                            }
                            else 
                            {
                                cout << "   INVALID MATCH" << endl;
                            }
                        }
                    }
                }
            }
        }
    }
};

AocDay19::AocDay19():AocDay(19)
{
}

AocDay19::~AocDay19()
{
}

vector<Scanner *> AocDay19::parse_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    vector<Scanner *> scanners;
    
    char delimiters[2];
    delimiters[0]=' ';
    delimiters[1]=',';
    
    if (!fileutils.read_as_list_of_split_strings(filename, data, delimiters, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return scanners;
    }
    
    Scanner * current_scanner = NULL;
    for (int i=0; i<data.size(); i++)
    {
        vector<string> parts = data[i];
        if (parts.size() == 4) // --- scanner 0 ---
        {
            current_scanner = new Scanner(strtol(parts[2].c_str(), NULL, 10));
            scanners.push_back(current_scanner);
        }
        if (parts.size() == 3) // x,y,z
        {
            current_scanner->add_beacon(new ObservedBeacon(strtol(parts[0].c_str(), NULL, 10), 
                                                           strtol(parts[1].c_str(), NULL, 10), 
                                                           strtol(parts[2].c_str(), NULL, 10),
                                                           current_scanner));
        }
        // skip any others - size is 0
    }
    
    return scanners;
}

string AocDay19::part1(string filename, vector<string> extra_args)
{
    Region region;
    vector<Scanner *> scanners = parse_input(filename); // these become owned by region
    
    region.set_scanners(scanners);
    region.set_scanner_zero_as_origin();
    region.map_overlaps();
    
    ostringstream out;
    out << region.get_actual_beacons().size();
    return out.str();
}
