# Day 16 #

## Part 1 Requirements ##

You are given a hexidecimal input containing one packet, which can contain many other packets.

The first step is to convert the hexideimcal to binary, using the standard representation. There may be a few extra zero bits at the end, which can be ignored.

Every packet starts with a header:
- Bits 1-3 encode the packet version, most significant-bit first (100 = 4).
- Bits 4-6 encoded the packet type ID, most significant-bit first.

Packets with type 4 are literal value packets, which encode a single binary number:
- Groups of 4 binary digits in the literal are encoded in order, left-padded with 0 bits to make a multiple of 4.
- Each group of 4 digits has an indicated prepended in the packet. A 1 indicates this is not the last group, while a 0 indicates it is the last group.

Packets with other types represent operator packets. An operator packet will have one or more subpackets.
There are two different length types, as indicated by the value in bit 7 (i.e. the bit right after the header)
- If this length type ID is 0, then the next 15 bits represent the total length in bits of its sub-packets.
- If this length type ID is 1, then the next 11 bits represent the number of sub-packets immediately contained in this packet.

The sub-packets then start at bit 8

Given the input, we are to parse through and create the heirarchy, and then output the sum of the version numbers for all of the packets.

## Part 2 Requirements ##

### Input Format ###

The input string is a hexidecimal message containing all of the packets. It is all on one line.

### Output Format ###

#### Part 1 ####

Number - the sum of the version numbers for all of the packets.

#### Part 2 ####


## Test Cases ##

### Given Test Cases ###

There are seven provided examples for part 1:
- [1](../data/test_cases/day16_test1.txt).
- [2](../data/test_cases/day16_test2.txt).
- [3](../data/test_cases/day16_test3.txt).
- [4](../data/test_cases/day16_test4.txt).
- [5](../data/test_cases/day16_test5.txt).
- [6](../data/test_cases/day16_test6.txt).
- [7](../data/test_cases/day16_test7.txt).

### Custom Test Cases ###

I don't anticipate any custom test cases, but I'm not completely sure.

## Approach ##

### Input Parsing ###

I will just return the input as a string.

### Convert to binary ###

Rather than converting the hexidecimal text to binary values, I plan on just converting it to a string of 0 and 1 characters.

I will use a map to go from the hex character to the four binary characters.

### Data Structures ###

Packet base class will contain:
- version number
- type ID
- getters for the above
- virtual method to return sum of version numbers for this packet and all sub-packets

LiteralPacket subclass will inherit from Packet and also contain:
- value

OperatorPacket subclass will inherit from Packet and also contain:
- list of subpackets
- method to add a subpacket

### create_packet function ###

This function will handle parsing the binary into packets. It will be cursive.

Input parameters:
- input - the input string in binary
- start_position - the starting position for where to begin consuming the input string

Return/Output parameter:
- The newly created packet is returned
- bits_consumed - the number of bits consumed in creating this packet.

Functionality:
- Convert *input[start_pos] to input[start_pos+2]* to an integer and store in *version*
- Convert *input[start_pos+3] to input[start_pos+5]* to an integer and store in *type_id*
- Set *bits_consumed* to be 6, to correspond to those bits consumed
- If *type_id* is 4, we have a literal value
    - Initialize *value* to 0, *group_ind* to 0
    - do
        - set *group_ind* to *input[start_pos+bits_consumed]*
        - convert *input[start_pos+bits_consumed+1] to input[start_pos+bits_consumed+4]* to an integer *group_value*
        - multiply *value* by 16 and add *group_value*
        - increment *bits_consumed* by 5
    - while *group_ind* equals 1 (this one was not yet the last group)
    - Create a LiteralPacket *packet* from *version*, *type_id*, and *value*
- Else, we have an operator packet
    - Create an OperatorPacket *packet* from *version* and *type_id*.
    - Store *input[start_pos+bits_consumed]* in *length_type*.
    - Increment *bits_consumed*
    - If *length_type* is 0, this packet uses bit length counting.
        - Convert *input[start_pos+bits_consumed] to input[start_pos+bits_consumed+14]* to an integer and store in *subpacket_bits*
        - Increment *bits_consumed* by 15
        - Set *used_subpacket_bits* to 0
        - While *used_subpacket_bits* < *subpacket_bits*
            - Call create_packet, passing in *start_position + bits_consumed* as its input *start_position* value. 
              Store the result in *subpacket* and the length in *current_subpacket_bits*
            - Increment both *bits_consumed* and *used_subpacket_bits* by *current_subpacket_bits*
            - Add *subpacket* to *packet* as a sub-packet
    - Else, this packet uses sub-packet counting
        - Convert *input[start_pos+bits_consumed] to input[start_pos+bits_consumed+10]* to an integer and store in *subpackets*
        - Increment *bits_consumed* by 11
        - Set *used_subpackets* to 0
        - While *used_subpackets* < *subpackets*
            - Call create_packet, passing in *start_position + bits_consumed* as its input *start_position* value. 
              Store the result in *subpacket* and the length in *current_subpacket_bits*
            - Increment both *bits_consumed* by *current_subpacket_bits*
            - Increment *used_subpackets* by 1
            - Add *subpacket* to *packet* as a sub-packet
- Return *packet*.

### Main Loop ###

- Read in the input from the file into a string *hex*
- Convert the *hex* string to *binary* string
- Call the create_packet function passing in *binary* and starting at position 0. Store the result in *packet*.
- **Output** the result of the sum of the version numbers being called on *packet*.

## Things I learned ##

So much fun designing the algorithm to handle the parsing of the packets.

