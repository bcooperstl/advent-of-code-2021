#ifndef __AOC_DAY_19__
#define __AOC_DAY_19__

#include "aoc_day.h"

#include <vector>
#include <map>

#define MAX_SCANNERS 30

namespace Day19
{
    struct Coordinates
    {
        int x;
        int y;
        int z;
    };
    
    class Scanner;
    
    class ObservedBeacon
    {
        private:
            Coordinates m_coordinates;
            Scanner * m_owner;
        public:
            ObservedBeacon(int x, int y, int z, Scanner * owner);
            ~ObservedBeacon();
            Coordinates get_coordinates();
            Scanner * get_owner();
    };
    
    class BeaconPairDistance
    {
        private:
            ObservedBeacon * m_beacons[2];
            int m_simple_distance;
            void calculate_simple_distance();
        public:
            BeaconPairDistance(ObservedBeacon * first, ObservedBeacon * second);
            ~BeaconPairDistance();
            int get_simple_distance();
            void get_actual_distance(int & x, int & y, int & z);
            void reverse();
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
            vector<BeaconPairDistance> get_beacon_pair_distances();
    };
    
    class Region
    {
        private:
            Scanner * m_scanners[MAX_SCANNERS];
            int m_num_scanners;
            vector<ActualBeacon *> m_actual_beacons;
            bool not_all_determined();
            void check_overlap(Scanner * first, Scanner * second);
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
