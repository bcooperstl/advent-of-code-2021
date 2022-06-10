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
    void FullState::display()
    {
        cout << "w =" << w << " x = " << x << " y = " << y << " z = " << z << endl;
    }
    
    void SimpleState::display()
    {
        cout << "y = " << y << " z = " << z << endl;
    }
    
    void Path::display()
    {
        cout << "Path has depth " << depth << " with options: ";
        for (int i=0; i<depth; i++)
        {
            cout << "[";
            for (int j=1; j<=9; j++)
            {
                if (to_here[i][j])
                {
                    cout << j;
                }
            }
            cout << "]";
        }
        cout << endl;
    }
    
    void Combination::display()
    {
        cout << "Combination has state ";
        state.display();
        for (int i=0; i<paths.size(); i++)
        {
            cout << "  ";
            paths[i].display();
        }
    }
        
        
    void Instruction::display()
    {
        if (type != INPUT)
        {
            if (use_source_char)
            {
                cout << "Operation " << type << " to " << dest << " from " << source_char << endl;
            }
            else
            {
                cout << "Operation " << type << " to " << dest << " with value " << source_val << endl;
            
            }
        }
        else
        {
            cout << "Operation " << type << " to " << dest << endl;
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
        inst.dest = lines[i][1][0]; // get the first character
        if (inst.type != INPUT)
        {
            inst.source_char = lines[i][2][0]; // get the first character, then check it
            if (inst.source_char == 'w' || inst.source_char == 'x' || inst.source_char == 'y' || inst.source_char == 'z')
            {
                inst.use_source_char = true;
                cout << "Operation " << inst.type << " to " << inst.dest << " from " << inst.source_char << endl;
            }
            else
            {
                inst.use_source_char = false;
                inst.source_val = strtol(lines[i][2].c_str(), NULL, 10);
                cout << "Operation " << inst.type << " to " << inst.dest << " with value " << inst.source_val << endl;
            
            }
        }
        else
        {
            cout << "Operation " << inst.type << " to " << inst.dest << endl;
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

void AocDay24::run_instruction(Instruction inst, FullState & state, int next_input)
{
//    cout << " Initial: ";
//    state.display();
//    cout << " Running: ";
//    inst.display();
    
    long result;
    if (inst.type == INPUT)
    {
        result = next_input;
    }
    else
    {
        long a, b;
        switch (inst.dest)
        {
            case 'w':
                a = state.w;
                break;
            case 'x':
                a = state.x;
                break;
            case 'y':
                a = state.y;
                break;
            case 'z':
                a = state.z;
                break;
        }
        if (inst.use_source_char)
        {
            switch (inst.source_char)
            {
                case 'w':
                    b = state.w;
                    break;
                case 'x':
                    b = state.x;
                    break;
                case 'y':
                    b = state.y;
                    break;
                case 'z':
                    b = state.z;
                    break;
            }
        }
        else
        {
            b = inst.source_val;
        }
        
        if (inst.type == ADD)
        {
            result = a + b;
        }
        else if (inst.type == MULTIPLY)
        {
            result = a * b;
        }
        else if (inst.type == DIVIDE)
        {
            if (b == 0)
            {
                cerr << "INVALID DIVISOR OF 0!!!" << endl;
                result = 0;
            }
            else
            {
                result = a / b;
            }
        }
        else if (inst.type == MODULO)
        {
            if (a < 0)
            {
                cerr << "INVLAID NEGATIVE A FOR A MOD B!!!" << endl;
            }
            else if (b <= 0)
            {
                cerr << "INVALID NON-POSITIVE B FOR A MOD B!!!" << endl;
            }
            else
            {
                result = a % b;
            }
        }
        else if (inst.type == EQUAL)
        {
            result = (a == b ? 1 : 0);
        }
    }
    switch (inst.dest)
    {
        case 'w':
            state.w = result;
            break;
        case 'x':
            state.x = result;
            break;
        case 'y':
            state.y = result;
            break;
        case 'z':
            state.z = result;
            break;
    }
            
//    cout << " Final: ";
//    state.display();
}

void AocDay24::work_combinations(vector<Combination> initial, vector<Combination> & results, vector<Instruction> instructions)
{
    vector<Combination>::iterator current_initial_pos = initial.begin();
    while (current_initial_pos != initial.end())
    {
        Combination init_combination = *current_initial_pos;
        FullState states[10];
        Path paths[10];
        bool used[10];
        for (int i=1; i<=9; i++)
        {
            //cout << "Running for option " << i << endl;
            used[i] = false;
            states[i].w = 0;
            states[i].x = 0;
            states[i].y = init_combination.state.y;
            states[i].z = init_combination.state.z;
            for (int inst = 0; inst < instructions.size(); inst++)
            {
                run_instruction(instructions[inst], states[i], i);
            }
        }
        
        // We now have our results. Time to create new combinations from them
        for (int i=1; i<=9; i++)
        {
            if (used[i])
            {
                continue;
            }
            Combination next = init_combination; // this will duplicate the paths and the state
            next.state.y = states[i].y;
            next.state.z = states[i].z;
            
            for (int j=0; j<next.paths.size(); j++)
            {
                // clear out all 1-9 possibilities
                for (int k=1; k<=9; k++)
                {
                    next.paths[j].to_here[next.paths[j].depth][k] = false;
                }
                
                // set the ith one to true - this is the value we used
                next.paths[j].to_here[next.paths[j].depth][i] = true;
                
                // look to see if any others match and mark them as used/set them true
                for (int k=i+1; k<=9; k++)
                {
                    if (states[k].y == next.state.y && states[k].z == next.state.z)
                    {
                        cout << " State match between " << i << " and " << k << " found!" << endl;
                        next.paths[j].to_here[next.paths[j].depth][k] = true;
                        used[k] = true;
                    }
                }
                
                next.paths[j].depth++;
            }
            
            used[i] = true;
            //next.display();
            bool found_existing = false;
            for (int j=0; j<results.size(); j++)
            {
                if (results[j].state.y == next.state.y && results[j].state.z == next.state.z)
                {
                    results[j].paths.insert(results[j].paths.end(), next.paths.begin(), next.paths.end());
                    found_existing = true;
                    //cout << "Adding to existing results member" << endl;
                    //results[j].display();
                    break;
                }
            }
            if (!found_existing)
            {
                //cout << "Adding as new results member" << endl;
                results.push_back(next);
            }
        }
        
        ++current_initial_pos;
    }
}

string AocDay24::part1(string filename, vector<string> extra_args)
{
    vector<Instruction> all;
    vector<vector<Instruction>> split;
    
    parse_input(filename, all);
    split_instructions(all, split);
    
    Path initial_path;
    initial_path.depth = 0;
    
    SimpleState initial_state;
    initial_state.y = 0;
    initial_state.z = 0;
    
    vector<Combination> initial_combinations, final_combinations;
    Combination initial_combination;
    initial_combination.paths.push_back(initial_path);
    initial_combination.state = initial_state;
    initial_combinations.push_back(initial_combination);
    
    for (int i=1; i<=14; i++)
    {
        cout << "Working " << initial_combinations.size() << " combinations at depth " << i << endl;
        final_combinations.clear();
        work_combinations(initial_combinations, final_combinations, split[i-1]);
        initial_combinations = final_combinations;
    }
    
    ostringstream out;
    out << split.size();
    return out.str();
}
