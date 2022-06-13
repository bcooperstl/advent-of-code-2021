#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include "aoc_day_24.h"
#include "file_utils.h"

using namespace std;
using namespace Day24;

namespace Day24
{
    void SimpleState::display()
    {
        cout << "y = " << y << " z = " << z << endl;
    }
    
    CompState::CompState(long next_input)
    {
        m_next_input = next_input;
        for (int i=0; i<4; i++)
        {
            m_variables[i] = 0;
        }
    }
    
    CompState::CompState(SimpleState simple, long next_input)
    {
        m_next_input = next_input;
        m_variables[W] = 0;
        m_variables[X] = 0;
        m_variables[Y] = simple.y;
        m_variables[Z] = simple.z;
    }
    
    CompState::~CompState()
    {
    }
    
    long CompState::get(int which)
    {
        return m_variables[which];
    }
    
    SimpleState CompState::get_simple_state()
    {
        SimpleState ret;
        ret.y = m_variables[Y];
        ret.z = m_variables[Z];
        return ret;
    }
    
    void CompState::set(int which, long value)
    {
        m_variables[which] = value;
    }
    
    void CompState::display()
    {
        cout << "w = "  << m_variables[W] 
             << " x = " << m_variables[X] 
             << " y = " << m_variables[Y] 
             << " z = " << m_variables[Z] << endl;
    }
    
    void CompState::do_input(int a)
    {
        m_variables[a] = m_next_input;
    }
    
    void CompState::do_add_variable(int a, int b)
    {
        m_variables[a] = m_variables[a] + m_variables[b];
    }
    
    void CompState::do_add_constant(int a, long value)
    {
        m_variables[a] = m_variables[a] + value;
    }
    
    void CompState::do_multiply_variable(int a, int b)
    {
        m_variables[a] = m_variables[a] * m_variables[b];
    }
    
    void CompState::do_multiply_constant(int a, long value)
    {
        m_variables[a] = m_variables[a] * value;
    }
    
    void CompState::do_divide_variable(int a, int b)
    {
        if (m_variables[b] == 0)
        {
            cerr << "INVALID DIVISOR OF 0!!!" << endl;
        }
        m_variables[a] = m_variables[a] / m_variables[b];
    }
    
    void CompState::do_divide_constant(int a, long value)
    {
        if (value == 0)
        {
            cerr << "INVALID DIVISOR OF 0!!!" << endl;
        }
        m_variables[a] = m_variables[a] / value;
    }
    
    void CompState::do_modulo_variable(int a, int b)
    {
        if (m_variables[a] < 0)
        {
            cerr << "INVLAID NEGATIVE A FOR A MOD B!!!" << endl;
        }
        else if (m_variables[b] <= 0)
        {
            cerr << "INVALID NON-POSITIVE B FOR A MOD B!!!" << endl;
        }
        m_variables[a] = m_variables[a] % m_variables[b];
    }
    
    void CompState::do_modulo_constant(int a, long value)
    {
        if (m_variables[a] < 0)
        {
            cerr << "INVLAID NEGATIVE A FOR A MOD B!!!" << endl;
        }
        else if (value <= 0)
        {
            cerr << "INVALID NON-POSITIVE B FOR A MOD B!!!" << endl;
        }
        m_variables[a] = m_variables[a] % value;
    }
    
    void CompState::do_equals_variable(int a, int b)
    {
        m_variables[a] = (m_variables[a] == m_variables[b] ? 1 : 0);
    }
    
    void CompState::do_equals_constant(int a, long value)
    {
        m_variables[a] = (m_variables[a] == value ? 1 : 0);
    }
    
    void PathStep::display()
    {
        cout << "Path has depth " << depth << " with state: ";
        state.display();
    }
    
    char Instruction::get_letter(int which)
    {
        switch (which)
        {
            case W:
                return 'w';
            case X:
                return 'x';
            case Y:
                return 'y';
            case Z:
                return 'z';
        }
        return '?';
    }
    
    int Instruction::get_value(char ch)
    {
        switch (ch)
        {
            case 'w':
                return W;
            case 'x':
                return X;
            case 'y':
                return Y;
            case 'z':
                return Z;
        }
        return -1;
    }
    
    void Instruction::display()
    {
        if (type != INPUT)
        {
            if (use_source_char)
            {
                cout << "Operation " << type << " to " << get_letter(dest) << " from " << get_letter(source_char) << endl;
            }
            else
            {
                cout << "Operation " << type << " to " << get_letter(dest) << " with value " << source_val << endl;
            
            }
        }
        else
        {
            cout << "Operation " << type << " to " << get_letter(dest) << endl;
        }
    }
};

AocDay24::AocDay24():AocDay(24)
{
}

AocDay24::~AocDay24()
{
}

void AocDay24::parse_input(string filename, vector<Instruction> & instructions)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    
    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    for (int i=0; i<lines.size(); i++)
    {
        Instruction inst;
        inst.type = lines[i][0];
        inst.dest = inst.get_value(lines[i][1][0]); // get the first character
        if (inst.type != INPUT)
        {
            char ch = lines[i][2][0]; // get the first character, then check it
            if (ch == 'w' || ch == 'x' || ch == 'y' || ch == 'z')
            {
                inst.use_source_char = true;
                inst.source_char = inst.get_value(ch);
                cout << "Operation " << inst.type << " to " << inst.get_letter(inst.dest) << " from " << inst.get_letter(inst.source_char) << endl;
            }
            else
            {
                inst.use_source_char = false;
                inst.source_val = strtol(lines[i][2].c_str(), NULL, 10);
                cout << "Operation " << inst.type << " to " << inst.get_letter(inst.dest) << " with value " << inst.source_val << endl;
            
            }
        }
        else
        {
            cout << "Operation " << inst.type << " to " << inst.get_letter(inst.dest) << endl;
        }
        instructions.push_back(inst);
    }
    return;
}

void AocDay24::split_instructions(vector<Instruction>&  all, vector<vector<Instruction>> & split)
{
    vector<Instruction> current;
    for (int i=0; i<all.size(); i++)
    {
        if (all[i].type == INPUT && i > 0)
        {
            split.push_back(current);
            current.clear();
        }
        current.push_back(all[i]);
    }
    split.push_back(current); // get the last one
}

string AocDay24::part1(string filename, vector<string> extra_args)
{
    vector<Instruction> all;
    vector<vector<Instruction>> split;
    
    parse_input(filename, all);
    split_instructions(all, split);
    
    SimpleState initial_state;
    initial_state.y = 0;
    initial_state.z = 0;
    
    ostringstream out;
    out << split.size();
    return out.str();
}
