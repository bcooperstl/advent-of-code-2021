#ifndef __AOC_DAY_16__
#define __AOC_DAY_16__

#include "aoc_day.h"

#include <vector>
#include <map>
#include <utility>

namespace Day16
{
    class Packet
    {
        protected:
            int m_version;
            int m_type_id;
        public:
            Packet(int version, int type_id);
            ~Packet();
            int get_version();
            int get_type_id();
            virtual int get_version_sum() = 0;
            virtual int get_value() = 0;
    };
    
    class LiteralPacket : public Packet
    {
        protected:
            int m_value;
        public:
            LiteralPacket(int version, int type_id, int value);
            virtual ~LiteralPacket();
            virtual int get_version_sum();
            virtual int get_value();
    };
    
    class OperatorPacket : public Packet
    {
        protected:
            vector<Packet *> m_subpackets;
        public:
            OperatorPacket(int version, int type_id);
            virtual ~OperatorPacket();
            void add_subpacket(Packet * packet);
            virtual int get_version_sum();
            virtual int get_value();
    };
}

using namespace Day16;

class AocDay16 : public AocDay
{
    private:
        map<char, string> m_hex_to_binary_map;
        string read_input(string filename);
        string convert_hex_to_binary(string hex);
        Packet * create_packet(string input, int start_position, int & bits_consumed);
    public:
        AocDay16();
        ~AocDay16();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
