#ifndef __AOC_DAY_19__
#define __AOC_DAY_19__

#include "aoc_day.h"

#include <vector>
#include <map>

#define MAX_SCANNERS 30

namespace Day19
{
    class Scanner;
    class ActualBeacon;
    class ObservedBeacon;
    
    struct Coordinates
    {
        int x;
        int y;
        int z;
    };
    
    struct BeaconDistances
    {
        int delta_x;
        int delta_y;
        int delta_z;
        int computed_distance;
    };
    
    struct MatchingBeaconDistances
    {
        ActualBeacon * actual_beacons[2];
        ObservedBeacon * unmapped_beacons[2];
        BeaconDistances actual_distances;
        BeaconDistances unmapped_distances;
    };
    
#define X_INDEX 0
#define Y_INDEX 1
#define Z_INDEX 2
    // adj_value = x_mult * x + y_mult * y + z_mult * z + offset
    struct MappedToActualTransformations
    {
        // there are three of everything which are used to compute the final values for x, y, and z
        int x_mult[3];
        int y_mult[3];
        int z_mult[3];
        int offset[3];
    };
    
    class ObservedBeacon
    {
        private:
            Coordinates m_coordinates;
            Scanner * m_owner;
            ActualBeacon * m_actual_beacon;
        public:
            ObservedBeacon(int x, int y, int z, Scanner * owner);
            ~ObservedBeacon();
            Coordinates get_coordinates();
            Scanner * get_owner();
            void set_actual_beacon(ActualBeacon * beacon);
            ActualBeacon * get_actual_beacon();
    };
    
    class ActualBeacon
    {
        private:
            Coordinates m_coordinates;
            vector<ObservedBeacon *> m_corresponding_beacons;
        public:
            ActualBeacon(int x, int y, int z);
            ActualBeacon(Coordinates coordinates);
            ~ActualBeacon();
            Coordinates get_coordinates();
            void add_corresponding_beacon(ObservedBeacon * beacon);
            vector<ObservedBeacon *> get_corresponding_beacons();
    };
    
    class Scanner
    {
        private:
            int m_number;
            Coordinates m_observed_coordinates;
            Coordinates m_actual_coordinates;
            vector<ObservedBeacon *> m_beacons;
            bool m_actual_determined;
        public:
            Scanner(int number);
            ~Scanner();
            int get_number();
            void add_beacon(ObservedBeacon * beacon);
            vector<ObservedBeacon *> get_beacons();
            Coordinates get_observed_coordinates();
            void set_actual_coordinates(Coordinates coordinates);
            Coordinates get_actual_coordinates();            
            bool get_actual_determined();
    };
    
    class Region
    {
        private:
            Scanner * m_scanners[MAX_SCANNERS];
            int m_num_scanners;
            vector<ActualBeacon *> m_actual_beacons;
            bool not_all_determined();
            void check_overlap(Scanner * mapped, Scanner * unmapped);
            void determine_transform(int & mult, int & offset, int actual_common, int actual_one, int actual_two, int unmapped_common, int unmapped_one, int unmapped_two);
        public:
            Region();
            ~Region();
            void set_scanners(vector<Scanner *> scanners);
            Scanner * get_scanner(int number);
            vector<ActualBeacon *> get_actual_beacons();
            void add_actual_beacon(ActualBeacon * beacon);
            void set_scanner_zero_as_origin();
            void map_overlaps();
            
    };
}

using namespace Day19;

class AocDay19 : public AocDay
{
    private:
        vector<Scanner *> parse_input(string filename);
    public:
        AocDay19();
        ~AocDay19();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
