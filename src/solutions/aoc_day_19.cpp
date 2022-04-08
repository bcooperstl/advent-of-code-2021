#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_19.h"
#include "file_utils.h"

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
    
    BeaconPairDistance::BeaconPairDistance(ObservedBeacon * first, ObservedBeacon * second)
    {
        m_beacons[0] = first;
        m_beacons[1] = second;
        calculate_simple_distance();
    }
    
    BeaconPairDistance::~BeaconPairDistance()
    {
    }
    
    void BeaconPairDistance::calculate_simple_distance()
    {
        Coordinates first = m_beacons[0]->get_coordinates();
        Coordinates second = m_beacons[1]->get_coordinates();
        
        m_simple_distance = ((first.x-second.x)*(first.x-second.x)) +
                            ((first.y-second.y)*(first.y-second.y)) +
                            ((first.z-second.z)*(first.z-second.z));
    }
    
    int BeaconPairDistance::get_simple_distance()
    {
        return m_simple_distance;
    }
    
    void BeaconPairDistance::get_actual_distance(int & x, int & y, int & z)
    {
        Coordinates first = m_beacons[0]->get_coordinates();
        Coordinates second = m_beacons[1]->get_coordinates();
        
        x = first.x-second.x;
        y = first.y-second.y;
        z = first.z-second.z;
    }
    
    void BeaconPairDistance::reverse()
    {
        ObservedBeacon * temp = m_beacons[1];
        m_beacons[1] = m_beacons[0];
        m_beacons[0] = temp;
    }
    
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
    
    vector<BeaconPairDistance> Scanner::get_beacon_pair_distances()
    {
        vector<BeaconPairDistance> distances;
        cout << "Calculating distances for scanner " << m_number << endl;
        for (int i=0; i<m_beacons.size()-1; i++)
        {
            Coordinates first = m_beacons[i]->get_coordinates();
            for (int j=i+1; j<m_beacons.size(); j++)
            {
                BeaconPairDistance distance(m_beacons[i], m_beacons[j]);
                Coordinates second = m_beacons[j]->get_coordinates();
                cout << " The distance between " << first.x << "," << first.y << "," << first.z << " and "
                     << second.x << "," << second.y << "," << second.z << " is " << distance.get_simple_distance() << endl;
                distances.push_back(distance);
            }
        }
        return distances;
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
        vector<BeaconPairDistance> mapped_distances = mapped->get_beacon_pair_distances();
        vector<BeaconPairDistance> unmapped_distances = unmapped->get_beacon_pair_distances();
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
