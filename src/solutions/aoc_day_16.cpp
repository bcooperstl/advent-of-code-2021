#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <climits>
#include <utility>

#include "aoc_day_16.h"
#include "file_utils.h"

using namespace std;
using namespace Day16;

namespace Day16
{
    Packet::Packet(int version, int type_id)
    {
        m_version=version;
        m_type_id=type_id;
    };
    
    Packet::~Packet()
    {
    }
    
    int Packet::get_version()
    {
        return m_version;
    }
    
    int Packet::get_type_id()
    {
        return m_type_id;
    }
    
    LiteralPacket::LiteralPacket(int version, int type_id, int value):Packet(version, type_id)
    {
        m_value = value;
    }
    
    LiteralPacket::~LiteralPacket()
    {
    }
    
    int LiteralPacket::get_value()
    {
        return m_value;
    }
    
    int LiteralPacket::get_version_sum()
    {
        return get_version();
    }
    
    OperatorPacket::OperatorPacket(int version, int type_id):Packet(version, type_id)
    {
    }
    
    OperatorPacket::~OperatorPacket()
    {
        for (int i=0; i<m_subpackets.size(); i++)
        {
            delete m_subpackets[i];
        }
    }
    
    void OperatorPacket::add_subpacket(Packet * packet)
    {
        m_subpackets.push_back(packet);
    }
    
    int OperatorPacket::get_version_sum()
    {
        int sum = get_version();
        for (int i=0; i<m_subpackets.size(); i++)
        {
            sum+=m_subpackets[i]->get_version_sum();
        }
        return sum;
    }
    
    int OperatorPacket::get_value()
    {
        
        return 0;
    }
};

AocDay16::AocDay16():AocDay(16)
{
    m_hex_to_binary_map['0']="0000";
    m_hex_to_binary_map['1']="0001";
    m_hex_to_binary_map['2']="0010";
    m_hex_to_binary_map['3']="0011";
    m_hex_to_binary_map['4']="0100";
    m_hex_to_binary_map['5']="0101";
    m_hex_to_binary_map['6']="0110";
    m_hex_to_binary_map['7']="0111";
    m_hex_to_binary_map['8']="1000";
    m_hex_to_binary_map['9']="1001";
    m_hex_to_binary_map['A']="1010";
    m_hex_to_binary_map['B']="1011";
    m_hex_to_binary_map['C']="1100";
    m_hex_to_binary_map['D']="1101";
    m_hex_to_binary_map['E']="1110";
    m_hex_to_binary_map['F']="1111";
}

AocDay16::~AocDay16()
{
}

string AocDay16::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
        
    return lines[0];
}

string AocDay16::convert_hex_to_binary(string hex)
{
    ostringstream binary;
    for (int i=0; i<hex.length(); i++)
    {
        binary << m_hex_to_binary_map[hex[i]];
    }
    return binary.str();
}

Packet * AocDay16::create_packet(string input, int start_position, int & bits_consumed)
{
    string version_str = input.substr(start_position, 3);
    string type_id_str = input.substr(start_position+3, 3);
    int version = strtol(version_str.c_str(), NULL, 2);
    int type_id = strtol(type_id_str.c_str(), NULL, 2);
    
    cout << "Converted version " << version_str << " to " << version << endl;
    cout << "Converted type id " << type_id_str << " to " << type_id << endl;
    
    bits_consumed = 6; // 3 for version and 3 for type id
    Packet * packet = NULL;
    
    if (type_id == 4) // literal packet
    {
        int value = 0;
        char group_ind = '0';
        do
        {
            group_ind = input[start_position+bits_consumed];
            string group_value_str = input.substr(start_position+bits_consumed+1, 4);
            int group_value = strtol(group_value_str.c_str(), NULL, 2);
            value *= 16;
            value += group_value;
            bits_consumed += 5;
            cout << "Literal group - indicator is " << group_ind 
                 << " results in group value " << group_value_str 
                 << " converted to " << group_value 
                 << " and value increased to " << value << endl;
        } while (group_ind == '1');
        packet = new LiteralPacket(version, type_id, value);
    }
    else // operator packet
    {
        OperatorPacket * operator_packet = new OperatorPacket(version, type_id);
        packet = operator_packet;
        char length_type = input[start_position+bits_consumed];
        bits_consumed++;
        if (length_type == '0') // bit length counting
        {
            string subpacket_bits_str = input.substr(start_position+bits_consumed, 15);
            int subpacket_bits = strtol(subpacket_bits_str.c_str(), NULL, 2);
            bits_consumed += 15;
            cout << "OperatorPacket using bit length counting for " << subpacket_bits_str << " converted to " << subpacket_bits << " bits." << endl;
            int used_subpacket_bits = 0;
            while (used_subpacket_bits < subpacket_bits)
            {
                int current_subpacket_bits;
                Packet * subpacket = create_packet(input, start_position+bits_consumed, current_subpacket_bits);
                bits_consumed += current_subpacket_bits;
                used_subpacket_bits += current_subpacket_bits;
                operator_packet->add_subpacket(subpacket);
                cout << " There were " << current_subpacket_bits << " of " << subpacket_bits << " bits used creating a subpacket" << endl;
            }
        }
        else // subpacket counting
        {
            string subpackets_str = input.substr(start_position+bits_consumed, 11);
            bits_consumed += 11;
            int subpackets = strtol(subpackets_str.c_str(), NULL, 2);
            cout << "OperatorPacket using subpacket counting for " << subpackets_str << " converted to " << subpackets << " subpackets." << endl;
            for (int i=0; i<subpackets; i++)
            {
                int current_subpacket_bits;
                Packet * subpacket = create_packet(input, start_position+bits_consumed, current_subpacket_bits);
                bits_consumed += current_subpacket_bits;
                operator_packet->add_subpacket(subpacket);
                cout << " There were " << current_subpacket_bits << " bits used for subpacket " << i+1 << " of " << subpackets << " subpackets" << endl;
            }
        }
    }
    return packet;
}


string AocDay16::part1(string filename, vector<string> extra_args)
{
    string input_hex = read_input(filename);
    string binary = convert_hex_to_binary(input_hex);
    
    int bits_used;
    Packet * packet = create_packet(binary, 0, bits_used);
    cout << "There were " << bits_used << " of " << binary.length() << " bits used in creating the outer packet" << endl;
    
    ostringstream out;
    out << packet->get_version_sum();
    delete packet;
    return out.str();
}
