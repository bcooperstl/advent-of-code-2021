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
        
        int case_of_match = -1;
        int num_actual_over_threshold = 0;
        vector<MatchingBeaconDistances> matching_distances[6]; // one for each of the 6 cases
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
                            
                            bool valid = true;
                            
                            MatchingBeaconDistances distances;
                            distances.actual_beacons[0] = mapped_actual_beacons[i];
                            distances.actual_beacons[1] = mapped_actual_beacons[j];
                            distances.unmapped_beacons[0] = unmapped_beacons[k];
                            distances.unmapped_beacons[1] = unmapped_beacons[l];
                            distances.actual_distances = actual_distances[i][j];
                            distances.unmapped_distances = unmapped_distances[k][l];
                            
                            if (actual_delta_x == unmapped_delta_x && actual_delta_y == unmapped_delta_y && actual_delta_z == unmapped_delta_z)
                            {
                                cout << "  Case 0: where x1==x2, y1==y2, z1==z2" << endl;
                                matching_distances[0].push_back(distances);
                            }
                            else if (actual_delta_x == unmapped_delta_x && actual_delta_y == unmapped_delta_z && actual_delta_z == unmapped_delta_y)
                            {
                                cout << "  Case 1: where x1==x2, y1==z2, z1==y2" << endl;
                                matching_distances[1].push_back(distances);
                            }
                            else if (actual_delta_x == unmapped_delta_y && actual_delta_y == unmapped_delta_x && actual_delta_z == unmapped_delta_z)
                            {
                                cout << "  Case 2: where x1==y2, y1==x2, z1==z2" << endl;
                                matching_distances[2].push_back(distances);
                            }
                            else if (actual_delta_x == unmapped_delta_y && actual_delta_y == unmapped_delta_z && actual_delta_z == unmapped_delta_x)
                            {
                                cout << "  Case 3: where x1==y2, y1==z2, z1==x2" << endl;
                                matching_distances[3].push_back(distances);
                            }
                            else if (actual_delta_x == unmapped_delta_z && actual_delta_y == unmapped_delta_x && actual_delta_z == unmapped_delta_y)
                            {
                                cout << "  Case 4: where x1==z2, y1==x2, z1==y2" << endl;
                                matching_distances[4].push_back(distances);
                            }
                            else if (actual_delta_x == unmapped_delta_z && actual_delta_y == unmapped_delta_y && actual_delta_z == unmapped_delta_x)
                            {
                                cout << "  Case 5: where x1==z2, y1==y2, z1==x2" << endl;
                                matching_distances[5].push_back(distances);
                            }
                            else 
                            {
                                cout << "   INVALID MATCH" << endl;
                                valid = false;
                            }
                        }
                    }
                }
            }
            for (int c=0; c<=5; c++)
            {
                if (matching_distances[c].size() >= 264) // the case has 264 or more matches (12 * 11 * 2) - 12 first points, 11 second points, 2 directions
                {
                    cout << "** Actual Beacon " << i << " at " << actual_coordinates[i].x << "," << actual_coordinates[i].y << "," << actual_coordinates[i].z
                         << " with matching case " << c << " has " << matching_distances[c].size() << " matching points " << endl;
                    num_actual_over_threshold++;
                    case_of_match = c;
                }
            }
        }
        
        if (case_of_match == -1)
        {
            cout << "No overlap found between mapped scanner " << mapped->get_number() << " and unmapped scanner " << unmapped->get_number() << endl;
            return;
        }
        
        if (num_actual_over_threshold > 1)
        {
            cout << "***************************Multiple overlap cases found between mapped scanner " << mapped->get_number() << " and unmapped scanner " << unmapped->get_number() << endl;
            return;
        }
        
        // Need to find two matches with the same Actual and Unmapped Beacons in index 0, and different ones with different distance values
        MatchingBeaconDistances matches_to_use[2];
        MatchingBeaconDistances alternatives[2];
        matches_to_use[0] = matching_distances[case_of_match][0];
        
        Coordinates temp_coord = matches_to_use[0].actual_beacons[0]->get_coordinates();
        cout << " Using 1st match from actual " << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to ";
        temp_coord = matches_to_use[0].actual_beacons[1]->get_coordinates();
        cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " and unmapped ";
        temp_coord = matches_to_use[0].unmapped_beacons[0]->get_coordinates();
        cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to ";
        temp_coord = matches_to_use[0].unmapped_beacons[1]->get_coordinates();
        cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << endl;
        
        
        // find a different segment from the same starting actual beacon
        // need to have different deltas as well
        for (int i=0; i<matching_distances[case_of_match].size(); i++)
        {
            if ((matches_to_use[0].actual_beacons[0] == matching_distances[case_of_match][i].actual_beacons[0]) && // first actual beacon matches
                (matches_to_use[0].actual_beacons[1] != matching_distances[case_of_match][i].actual_beacons[1]) &&
                (abs(matches_to_use[0].actual_distances.delta_x) != abs(matching_distances[case_of_match][i].unmapped_distances.delta_x)) &&
                (abs(matches_to_use[0].actual_distances.delta_y) != abs(matching_distances[case_of_match][i].unmapped_distances.delta_y)) &&
                (abs(matches_to_use[0].actual_distances.delta_z) != abs(matching_distances[case_of_match][i].unmapped_distances.delta_z)) )
            {
                matches_to_use[1] = matching_distances[case_of_match][i];
                temp_coord = matches_to_use[1].actual_beacons[0]->get_coordinates();
                cout << " Using 2nd match from actual " << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to " ;
                temp_coord = matches_to_use[1].actual_beacons[1]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " and unmapped ";
                temp_coord = matches_to_use[1].unmapped_beacons[0]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to ";
                temp_coord = matches_to_use[1].unmapped_beacons[1]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << endl;
                break; // got one. done
            }
        }

        // find the two alternate segments - same actual beacons, but reversed unmapped beacons
        for (int i=0; i<matching_distances[case_of_match].size(); i++)
        {
            if ((matches_to_use[0].actual_beacons[0] == matching_distances[case_of_match][i].actual_beacons[0]) && // first actual beacon matches
                (matches_to_use[0].actual_beacons[1] == matching_distances[case_of_match][i].actual_beacons[1]) &&
                (matches_to_use[0].unmapped_beacons[0] == matching_distances[case_of_match][i].unmapped_beacons[1]) &&
                (matches_to_use[0].unmapped_beacons[1] == matching_distances[case_of_match][i].unmapped_beacons[0]))
            {
                alternatives[0] = matching_distances[case_of_match][i];
                temp_coord = alternatives[0].actual_beacons[0]->get_coordinates();
                cout << " Setting 1st alternative from actual " << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to " ;
                temp_coord = alternatives[0].actual_beacons[1]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " and unmapped ";
                temp_coord = alternatives[0].unmapped_beacons[0]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to ";
                temp_coord = alternatives[0].unmapped_beacons[1]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << endl;
            }
            
            if ((matches_to_use[1].actual_beacons[0] == matching_distances[case_of_match][i].actual_beacons[0]) && // first actual beacon matches
                (matches_to_use[1].actual_beacons[1] == matching_distances[case_of_match][i].actual_beacons[1]) &&
                (matches_to_use[1].unmapped_beacons[0] == matching_distances[case_of_match][i].unmapped_beacons[1]) &&
                (matches_to_use[1].unmapped_beacons[1] == matching_distances[case_of_match][i].unmapped_beacons[0]))
            {
                alternatives[1] = matching_distances[case_of_match][i];
                temp_coord = alternatives[1].actual_beacons[0]->get_coordinates();
                cout << " Setting 2nd alternative from actual " << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to " ;
                temp_coord = alternatives[1].actual_beacons[1]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " and unmapped ";
                temp_coord = alternatives[1].unmapped_beacons[0]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << " to ";
                temp_coord = alternatives[1].unmapped_beacons[1]->get_coordinates();
                cout << temp_coord.x << "," << temp_coord.y << "," << temp_coord.z << endl;
            }
        }
        
        if (matches_to_use[0].unmapped_beacons[0] == matches_to_use[1].unmapped_beacons[0])
        {
            cout << " Matches are already good. No need to deal with alternatives" << endl;
        }
        else if (matches_to_use[0].unmapped_beacons[0] == alternatives[1].unmapped_beacons[0])
        {
            cout << " Using alternative for second match " << endl;
            matches_to_use[1] = alternatives[1];
        }
        else if (matches_to_use[1].unmapped_beacons[0] == alternatives[0].unmapped_beacons[0])
        {
            cout << " Using alternative for first match " << endl;
            matches_to_use[0] = alternatives[0];
        }
        else if (alternatives[0].unmapped_beacons[0] == alternatives[0].unmapped_beacons[0])
        {
            cout << " Using alternatives for first and second matches " << endl;
            matches_to_use[0] = alternatives[0];
            matches_to_use[1] = alternatives[1];
        }
        else
        {
            cout << "*************FAILED alternative swapping sanity check" << endl;
        }
        
        if (matches_to_use[0].unmapped_beacons[0] != matches_to_use[1].unmapped_beacons[0])
        {
            cout << "*************FAILED post-alternative-swap sanity check" << endl;
        }
    
        MappedToActualTransformations transforms;
        for (int i=0; i<3; i++)
        {
            transforms.x_mult[i] = 0;
            transforms.y_mult[i] = 0;
            transforms.z_mult[i] = 0;
            transforms.offset[i] = 0;
        }
        
        // case 0: x1 = x2, y1 = y2, z1 = z2
        if (case_of_match == 0)
        {
            // unmapped X goes to actual X
            determine_transform(transforms.x_mult[X_INDEX], transforms.offset[X_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().x);
            // unmapped Y goes to actual Y
            determine_transform(transforms.y_mult[Y_INDEX], transforms.offset[Y_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().y);
            // unmapped Z goes to actual Z
            determine_transform(transforms.z_mult[Z_INDEX], transforms.offset[Z_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().z);
        }    
        // case 1: x1 = x2, y1 = z2, z1 = y2
        if (case_of_match == 1)
        {
            // unmapped X goes to actual X
            determine_transform(transforms.x_mult[X_INDEX], transforms.offset[X_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().x);
            // unmapped Z goes to actual Y
            determine_transform(transforms.z_mult[Y_INDEX], transforms.offset[Y_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().z);
            // unmapped Y goes to actual Z
            determine_transform(transforms.y_mult[Z_INDEX], transforms.offset[Z_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().y);
        }    
        // case 2: x1 = y2, y1 = x2, z1 = z2
        if (case_of_match == 2)
        {
            // unmapped Y goes to actual X
            determine_transform(transforms.y_mult[X_INDEX], transforms.offset[X_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().y);
            // unmapped X goes to actual Y
            determine_transform(transforms.x_mult[Y_INDEX], transforms.offset[Y_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().x);
            // unmapped Z goes to actual Z
            determine_transform(transforms.z_mult[Z_INDEX], transforms.offset[Z_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().z);
        }    
        // case 3: x1 = y2, y1 = z2, z1 = x2
        if (case_of_match == 3)
        {
            // unmapped Y goes to actual X
            determine_transform(transforms.y_mult[X_INDEX], transforms.offset[X_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().y);
            // unmapped Z goes to actual Y
            determine_transform(transforms.z_mult[Y_INDEX], transforms.offset[Y_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().z);
            // unmapped X goes to actual Z
            determine_transform(transforms.x_mult[Z_INDEX], transforms.offset[Z_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().x);
        }    
        // case 4: x1 = z2, y1 = x2, z1 = y2
        if (case_of_match == 4)
        {
            // unmapped Z goes to actual X
            determine_transform(transforms.z_mult[X_INDEX], transforms.offset[X_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().z);
            // unmapped X goes to actual Y
            determine_transform(transforms.x_mult[Y_INDEX], transforms.offset[Y_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().x);
            // unmapped Y goes to actual Z
            determine_transform(transforms.y_mult[Z_INDEX], transforms.offset[Z_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().y);
        }    
        // case 5: x1 = z2, y1 = y2, z1 = x2
        if (case_of_match == 5)
        {
            // unmapped Z goes to actual X
            determine_transform(transforms.z_mult[X_INDEX], transforms.offset[X_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().z);
            // unmapped Y goes to actual Y
            determine_transform(transforms.y_mult[Y_INDEX], transforms.offset[Y_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().y, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().y, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().y);
            // unmapped X goes to actual Z
            determine_transform(transforms.x_mult[Z_INDEX], transforms.offset[Z_INDEX], 
                                matches_to_use[0].actual_beacons[0]->get_coordinates().z, 
                                matches_to_use[0].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[1].actual_beacons[1]->get_coordinates().z, 
                                matches_to_use[0].unmapped_beacons[0]->get_coordinates().x, 
                                matches_to_use[0].unmapped_beacons[1]->get_coordinates().x, 
                                matches_to_use[1].unmapped_beacons[1]->get_coordinates().x);
        }
        
        cout << " The final transformation matrix is :" << endl;
        cout << "  X = " << transforms.x_mult[X_INDEX] << "*x + " << transforms.y_mult[X_INDEX] << "*y + " << transforms.z_mult[X_INDEX] << "*z + " << transforms.offset[X_INDEX] << endl;
        cout << "  Y = " << transforms.x_mult[Y_INDEX] << "*x + " << transforms.y_mult[Y_INDEX] << "*y + " << transforms.z_mult[Y_INDEX] << "*z + " << transforms.offset[Y_INDEX] << endl;
        cout << "  Z = " << transforms.x_mult[Z_INDEX] << "*x + " << transforms.y_mult[Z_INDEX] << "*y + " << transforms.z_mult[Z_INDEX] << "*z + " << transforms.offset[Z_INDEX] << endl;
    }
    
    void Region::determine_transform(int & mult, int & offset, int actual_common, int actual_one, int actual_two, int unmapped_common, int unmapped_one, int unmapped_two)
    {
        cout << "Determining transform from actual " << actual_common << " " << actual_one << " " << actual_two << " and unmapped " << unmapped_common << " " << unmapped_one << " " << unmapped_two << endl;
        int actual_difference_one = actual_common - actual_one;
        int unmapped_difference_one = unmapped_common - unmapped_one;
        cout << " actual difference one = " << actual_difference_one << endl;
        cout << " unmapped difference one = " << unmapped_difference_one << endl;
        if (actual_difference_one == unmapped_difference_one)
        {
            cout << " The differences are the same. Multiplier is 1" << endl;
            mult = 1;
        }
        else if (actual_difference_one == -1 * unmapped_difference_one)
        {
            cout << " The differences are opposites. Multiplier is -1" << endl;
            mult = -1;
        }
        else
        {
            cout << "************************ Something ain't right with the differences" << endl;
            mult = 0;
        }
        offset = actual_common - (mult * unmapped_common);
        cout << " The offset is " << offset << endl;
        
        // double-check our work
        int calc_common = unmapped_common * mult + offset;
        int calc_one = unmapped_one * mult + offset;
        int calc_two = unmapped_two * mult + offset;
        cout << " Calculated values are " << calc_common << " " << calc_one << " " << calc_two << endl;
        if (calc_common != actual_common || calc_one != actual_one || calc_two != actual_two)
        {
            cout << "*************************** Error in calculations!!!" << endl;
        }
        return;
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
