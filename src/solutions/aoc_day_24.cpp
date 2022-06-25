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
        cout << " z = " << z << endl;
    }
    
    bool SimpleState::operator == (const SimpleState & other)
    {
        return (z == other.z);
    }
    
    CompState::CompState()
    {
        m_next_input = 0;
        for (int i=0; i<4; i++)
        {
            m_variables[i] = 0;
        }
    }        
    
    CompState::CompState(long next_input)
    {
        m_next_input = next_input;
    }
    
    CompState::CompState(SimpleState simple, long next_input)
    {
        m_next_input = next_input;
        m_variables[W] = 0;
        m_variables[X] = 0;
        m_variables[Y] = 0;
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
        ret.z = m_variables[Z];
        return ret;
    }
    
    void CompState::reset(SimpleState simple, long next_input)
    {
        m_next_input = next_input;
        m_variables[W] = 0;
        m_variables[X] = 0;
        m_variables[Y] = 0;
        m_variables[Z] = simple.z;
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
    
    
    PathStep::PathStep(int depth, SimpleState state)
    {
        m_depth = depth;
        m_state = state;
        for (int i=0; i<9; i++)
        {
            m_next_steps[i] = NULL;
        }
        m_best_to_here = 0;
    }
    
    PathStep::~PathStep()
    {
    }
    
    // expects which to be from 1-9
    void PathStep::set_next(int which, PathStep * next)
    {
        m_next_steps[which-1] = next;
    }
    
    SimpleState PathStep::get_state()
    {
        return m_state;
    }
    
    int PathStep::get_depth()
    {
        return m_depth;
    }
    
    PathStep * PathStep::get_next(int which)
    {
        return m_next_steps[which];
    }
    
    long PathStep::get_best_to_here()
    {
        return m_best_to_here;
    }
    
    void PathStep::set_best_to_here(long best_to_here)
    {
        m_best_to_here = best_to_here;
    }
    
    void PathStep::display()
    {
        cout << "Path has depth " << m_depth << " with state: ";
        m_state.display();
    }
    
    PathCache::PathCache()
    {
    }
    
    PathCache::~PathCache()
    {
    }
    
    void PathCache::put(PathStep * step)
    {
        SimpleState state = step->get_state();
        map<int, PathStep *>::iterator pos = m_paths.find(state.z);
        if (pos == m_paths.end())
        {
            m_paths[state.z] = step;
        }
        else
        {
            cerr << "Attempting double put in PathCache" << endl;
        }
    }
    
    PathStep * PathCache::get(SimpleState state)
    {
        map<int, PathStep *>::iterator pos = m_paths.find(state.z);
        if (pos == m_paths.end())
        {
            return NULL;
        }
        return pos->second;
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
            if (use_source_var)
            {
                cout << "Operation " << type << " to " << get_letter(dest) << " from " << get_letter(source_var) << endl;
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
                inst.use_source_var = true;
                inst.source_var = inst.get_value(ch);
                cout << "Operation " << inst.type << " to " << inst.get_letter(inst.dest) << " from " << inst.get_letter(inst.source_var) << endl;
            }
            else
            {
                inst.use_source_var = false;
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

void AocDay24::work_section(vector<PathStep *> & from, vector<PathStep *> & to, vector<Instruction> instructions)
{
    int depth = from[0]->get_depth();
    int total_comps = from.size() * 9;
    if (from.size() > 0)
    {
         cout << "Working " << from.size() << " paths from depth " << depth << " resulting in " << total_comps << " computers" << endl;
    }

    PathCache cache;
    
#define INCREMENT 100000
    CompState * comps = new CompState[INCREMENT * 9];
    for (int from_i=0; from_i<from.size(); from_i+=INCREMENT)
    {
        int from_low = from_i;
        int from_high = from_i + INCREMENT - 1;
        int total_used_comps = INCREMENT * 9;
        if (from_high > from.size())
        {
            from_high = from.size() - 1;
            total_used_comps = (from_high - from_low + 1) * 9;
        }
        cout << " Working " << total_used_comps << " comps on states " << from_low << "-" << from_high << endl;
        
        
        for (int i=from_low; i<=from_high; i++)
        {
            for (int j=0; j<9; j++)
            {
                //cout << " Resetting comps[" << 9*(i-from_low)+j << "]" << " to " << from[i]->get_state().z << " and " << j+1 << endl;
                comps[9*(i-from_low)+j].reset(from[i]->get_state(), j+1); // need j from 0 to 8 for array placement, but 1 to 9 for next value
            }
        }
        
        for (int i=0; i<instructions.size(); i++)
        {
            // i know this looks backwards, but i don't want to re-evaluate the instruction comparison 100,000 times per instruction, so i'll repeat the loop inside it
            if (instructions[i].type == INPUT)
            {
                //cout << "Input" << endl;
                for (int j=0; j<total_used_comps; j++)
                {
                    comps[j].do_input(instructions[i].dest);
                }
            }
            else if (instructions[i].type == ADD)
            {
                if (instructions[i].use_source_var)
                {
                    //cout << "Add by variable" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_add_variable(instructions[i].dest, instructions[i].source_var);
                    }
                }
                else
                {
                    //cout << "Add by constant" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_add_constant(instructions[i].dest, instructions[i].source_val);
                    }
                }                
            }            
            else if (instructions[i].type == MULTIPLY)
            {
                if (instructions[i].use_source_var)
                {
                    //cout << "Multiply by variable" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_multiply_variable(instructions[i].dest, instructions[i].source_var);
                    }
                }
                else
                {
                    //cout << "Multiply by constant" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_multiply_constant(instructions[i].dest, instructions[i].source_val);
                    }
                }                
            }            
            else if (instructions[i].type == DIVIDE)
            {
                if (instructions[i].use_source_var)
                {
                    //cout << "Divide by variable" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_divide_variable(instructions[i].dest, instructions[i].source_var);
                    }
                }
                else
                {
                    //cout << "Add by constant" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_divide_constant(instructions[i].dest, instructions[i].source_val);
                    }
                }                
            }            
            else if (instructions[i].type == MODULO)
            {
                if (instructions[i].use_source_var)
                {
                    //cout << "Modulo by variable" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_modulo_variable(instructions[i].dest, instructions[i].source_var);
                    }
                }
                else
                {
                    //cout << "Modulo by constant" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_modulo_constant(instructions[i].dest, instructions[i].source_val);
                    }
                }                
            }            
            else if (instructions[i].type == EQUAL)
            {
                if (instructions[i].use_source_var)
                {
                    //cout << "Equals by variable" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_equals_variable(instructions[i].dest, instructions[i].source_var);
                    }
                }
                else
                {
                    //cout << "Equals by constant" << endl;
                    for (int j=0; j<total_used_comps; j++)
                    {
                        comps[j].do_equals_constant(instructions[i].dest, instructions[i].source_val);
                    }
                }                
            }
        }            
            
        // now accumlulate the results;
        for (int i=0; i<total_used_comps; i++)
        {
            SimpleState state = comps[i].get_simple_state();
            
            if (depth != 13 || (depth == 13 && state.z == 0))
            {
                
                PathStep * next = cache.get(state);
                
                if (next == NULL)
                {
                    next = new PathStep(depth + 1, state);
                    //cout << "Created PathStep ";
                    //next->display();
                    to.push_back(next);
                    cache.put(next);
                }
                from[from_low + (i/9)]->set_next((i%9) + 1, next); // the set_next function expects the first paramter to be from 1-9
                
                long current_path = (from[from_low + (i/9)]->get_best_to_here() * 10l) + ((long)((i%9) + 1));
                //cout << "Current path is " << current_path << endl;
                if (current_path > next->get_best_to_here())
                {
                    //cout << " updated best_to_here from " << next->get_best_to_here() << " to " << current_path << endl;
                    next->set_best_to_here(current_path);
                }
            }
        }
    }
    delete [] comps;
}
                
string AocDay24::part1(string filename, vector<string> extra_args)
{
    vector<Instruction> all;
    vector<vector<Instruction>> split;
    
    parse_input(filename, all);
    split_instructions(all, split);
    
    SimpleState initial_state;
    initial_state.z = 0;
    
    vector<PathStep *> options[15];
    PathStep * initial = new PathStep(0, initial_state);
    options[0].push_back(initial);
    
    for (int i=0; i<14; i++)
    {
        work_section(options[i], options[i+1], split[i]);
    }
        
    cout << "Options[14] has " << options[14].size() << " elements" << endl;
    ostringstream out;
    out << options[14][0]->get_best_to_here();
    for (int i=0; i<=14; i++)
    {
        for (int j=0; j<options[i].size(); j++)
        {
            delete options[i][j];
        }
    }
    
    return out.str();
}
