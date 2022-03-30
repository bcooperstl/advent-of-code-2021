#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_18.h"
#include "file_utils.h"

using namespace std;
using namespace Day18;

namespace Day18
{
    Node::Node()
    {
    }
    
    Node::~Node()
    {
    }
    
    Number::Number(long value)
    {
        m_value = value;
    }
    
    Number::~Number()
    {
    }
    
    long Number::get_value()
    {
        return m_value;
    }
    
    void Number::set_value(long value)
    {
        m_value = value;
    }
    
    string Number::to_string()
    {
        ostringstream out;
        out << m_value;
        return out.str();
    }
    
    int Number::get_type()
    {
        return TYPE_NUMBER;
    }
    
    Node * Number::clone()
    {
        Number * clone = new Number(m_value);
        return clone;
    }
    
    long Number::get_magnitude()
    {
        return (long) m_value;
    }
    
    Pair::Pair() 
    {
        m_members[LEFT] = NULL;
        m_members[RIGHT] = NULL;
        m_depth = 0;
    }
    
    Pair::~Pair()
    {
        if (m_members[LEFT] != NULL)
        {
            delete m_members[LEFT];
        }
        if (m_members[RIGHT] != NULL)
        {
            delete m_members[RIGHT];
        }
    }
    
    Node * Pair::get_member(int which)
    {
        return m_members[which];
    }
    
    void Pair::set_member(Node * value, int which)
    {
        m_members[which] = value;
    }
    
    int Pair::get_depth()
    {
        return m_depth;
    }
    
    void Pair::set_depth(int depth)
    {
        m_depth = depth;
    }
    
    string Pair::to_string()
    {
        ostringstream out;
        out << '[';
        if (m_members[LEFT] != NULL)
        {
            out << m_members[LEFT]->to_string();
        }
        out << '|';
        if (m_members[RIGHT] != NULL)
        {
            out << m_members[RIGHT]->to_string();
        }
        out << ']';
        return out.str();
    }
    
    int Pair::get_type()
    {
        return TYPE_PAIR;
    }
    
    Node * Pair::clone()
    {
        Pair * clone = new Pair();
        clone->set_depth(m_depth);
        clone->set_member(m_members[LEFT]->clone(), LEFT);
        clone->set_member(m_members[RIGHT]->clone(), RIGHT);
        
        return clone;
    }

    void Pair::build_number_list(vector<Number *> & numbers)
    {
        if (m_members[LEFT]->get_type() == TYPE_NUMBER)
        {
            numbers.push_back((Number *)m_members[LEFT]);
        }
        else
        {
            ((Pair *)m_members[LEFT])->build_number_list(numbers);
        }

        if (m_members[RIGHT]->get_type() == TYPE_NUMBER)
        {
            numbers.push_back((Number *)m_members[RIGHT]);
        }
        else
        {
            ((Pair *)m_members[RIGHT])->build_number_list(numbers);
        }
    }
    
    Pair * Pair::find_parent(Node * target)
    {
        if (m_members[LEFT] == target || m_members[RIGHT] == target)
        {
            return this;
        }
        
        if (m_members[LEFT]->get_type() == TYPE_PAIR)
        {
            Pair * result = ((Pair *)m_members[LEFT])->find_parent(target);
            if (result != NULL)
            {
                return result;
            }
        }
        
        if (m_members[RIGHT]->get_type() == TYPE_PAIR)
        {
            Pair * result = ((Pair *)m_members[RIGHT])->find_parent(target);
            if (result != NULL)
            {
                return result;
            }
        }
        
        return NULL;
    }

    Pair * Pair::find_first_to_explode()
    {
        if (m_depth == 5)
        {
            return this;
        }
        
        if (m_members[LEFT]->get_type() == TYPE_PAIR)
        {
            Pair * result = ((Pair *)m_members[LEFT])->find_first_to_explode();
            if (result != NULL)
            {
                return result;
            }
        }
        
        if (m_members[RIGHT]->get_type() == TYPE_PAIR)
        {
            Pair * result = ((Pair *)m_members[RIGHT])->find_first_to_explode();
            if (result != NULL)
            {
                return result;
            }
        }
        
        return NULL;
    }

    Number * Pair::find_first_to_split()
    {
        
        if (m_members[LEFT]->get_type() == TYPE_PAIR)
        {
            Number * result = ((Pair *)m_members[LEFT])->find_first_to_split();
            if (result != NULL)
            {
                return result;
            }
        }
        else // is number
        {
            if (((Number *)m_members[LEFT])->get_value() > 9)
            {
                return (Number *)m_members[LEFT];
            }
        }
        
        if (m_members[RIGHT]->get_type() == TYPE_PAIR)
        {
            Number * result = ((Pair *)m_members[RIGHT])->find_first_to_split();
            if (result != NULL)
            {
                return result;
            }
        }
        else // is number
        {
            if (((Number *)m_members[RIGHT])->get_value() > 9)
            {
                return (Number *)m_members[RIGHT];
            }
        }
        
        return NULL;
    }
    
    void Pair::increment_depth()
    {
        m_depth++;
        if (m_members[LEFT]->get_type() == TYPE_PAIR)
        {
            ((Pair *)m_members[LEFT])->increment_depth();
        }
        
        if (m_members[RIGHT]->get_type() == TYPE_PAIR)
        {
            ((Pair *)m_members[RIGHT])->increment_depth();
        }
    }

    long Pair::get_magnitude()
    {
        return (3l * m_members[LEFT]->get_magnitude()) + (2l * m_members[RIGHT]->get_magnitude());; // 3 and 2 are longs
    }
};

AocDay18::AocDay18():AocDay(18)
{
}

AocDay18::~AocDay18()
{
}

vector<Pair *> AocDay18::parse_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    vector<Pair *> pairs;
    
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return pairs;
    }
    
    for (int i=0; i<lines.size(); i++)
    {
        pairs.push_back(convert_line(lines[i]));
    }
        
    return pairs;
}

Pair * AocDay18::convert_line(string input)
{
    Pair * current_pairs[6];
    bool set_left[6];
    for (int i=1; i<6; i++)
    {
        set_left[i] = false;
    }
    int current_depth = 1;
    Pair * base = new Pair();
    base->set_depth(1);
    current_pairs[1] = base;
    int position = 1;
    while (position < input.length())
    {
        if (isxdigit(input[position]))
        {
            Number * number;
            if (isdigit(input[position]))
                number = new Number(input[position]-'0');
            else
                number = new Number(input[position]-'A' + 10);
            if (!set_left[current_depth])
            {
                current_pairs[current_depth]->set_member(number, LEFT);
                set_left[current_depth] = true;
            }
            else
            {
                current_pairs[current_depth]->set_member(number, RIGHT);
            }
        }
        else if (input[position] == '[')
        {
            Pair * pair = new Pair();
            pair->set_depth(current_depth + 1);
            if (!set_left[current_depth])
            {
                current_pairs[current_depth]->set_member(pair, LEFT);
                set_left[current_depth] = true;
            }
            else
            {
                current_pairs[current_depth]->set_member(pair, RIGHT);
            }
            current_depth++;
            current_pairs[current_depth] = pair;
            set_left[current_depth] = false;
        }
        else if (input[position] == ']')
        {
            current_depth--;
        }
        else if (input[position] != ',')
        {
            cerr << "Unexpected character " << input[position] << " at position " << position << endl;
        }
        position++;
    }
    return base;
}

void AocDay18::explode(Pair * base, Pair * target)
{
    vector<Number *> numbers;
    Number * left_number = (Number *) target->get_member(LEFT);
    Number * right_number = (Number *) target->get_member(RIGHT);
    
    base->build_number_list(numbers);
    for (int num=0; num<numbers.size(); num++)
    {
        if (numbers[num] == left_number && num > 0)
        {
            numbers[num-1]->set_value(numbers[num-1]->get_value() + left_number->get_value());
        }
        if (numbers[num] == right_number && num < (numbers.size() - 1))
        {
            numbers[num+1]->set_value(numbers[num+1]->get_value() + right_number->get_value());
        }
    }
    Pair * parent = base->find_parent(target);
    Number * zero = new Number(0);
    if (parent->get_member(LEFT) == target)
    {
        parent->set_member(zero, LEFT);
    }
    else
    {
        parent->set_member(zero, RIGHT);
    }
    delete target;
}

void AocDay18::split(Pair * base, Number * target)
{
    Pair * split_pair = new Pair();
    split_pair->set_member(new Number(target->get_value()/2), LEFT);
    split_pair->set_member(new Number((target->get_value()+1)/2), RIGHT);
    
    Pair * parent = base->find_parent(target);
    split_pair->set_depth(parent->get_depth()+1);
    
    if (parent->get_member(LEFT) == target)
    {
        parent->set_member(split_pair, LEFT);
    }
    else
    {
        parent->set_member(split_pair, RIGHT);
    }
    delete target;
}

Pair * AocDay18::sum(Pair * first, Pair * second)
{
    Pair * sum = new Pair();
    sum->set_depth(1);
    sum->set_member(first->clone(), LEFT);
    sum->set_member(second->clone(), RIGHT);
    ((Pair *)sum->get_member(LEFT))->increment_depth();
    ((Pair *)sum->get_member(RIGHT))->increment_depth();
    //cout << "Addition " << sum->to_string() << endl;
    
    Pair * to_explode = sum->find_first_to_explode();
    Number * to_split = sum->find_first_to_split();
    
    while ((to_explode != NULL) || (to_split != NULL))
    {
        if (to_explode != NULL)
        {
            explode(sum, to_explode);
            //cout << "Explode " << sum->to_string() << endl;
        }
        else
        {
            split(sum, to_split);
            //cout << "Split " << sum->to_string() << endl;
        }
        to_explode = sum->find_first_to_explode();
        to_split = sum->find_first_to_split();
    }
    return sum;
}

string AocDay18::run_test(string filename, string test)
{
    if (test == "parse")
    {
        vector<Pair *> inputs = parse_input(filename);
        
        string ret = inputs[0]->to_string();
        delete inputs[0];
        return ret;
    }
    if (test == "explode")
    {
        vector<Pair *> inputs = parse_input(filename);
        
        Pair * first_to_explode = inputs[0]->find_first_to_explode();
        if (first_to_explode == NULL)
        {
            cerr << "No node found to explode for test" << endl;
        }
        explode(inputs[0], first_to_explode);
        
        string ret = inputs[0]->to_string();
        delete inputs[0];
        return ret;
    }
    if (test == "split")
    {
        vector<Pair *> inputs = parse_input(filename);
        
        Number * first_to_split = inputs[0]->find_first_to_split();
        if (first_to_split == NULL)
        {
            cerr << "No node found to split for test" << endl;
        }
        split(inputs[0], first_to_split);
        
        string ret = inputs[0]->to_string();
        delete inputs[0];
        return ret;
    }
    if (test == "sum")
    {
        vector<Pair *> inputs = parse_input(filename);
        
        Pair * running_sum = (Pair *)inputs[0]->clone();
        
        for (int i=1; i<inputs.size(); i++)
        {
            Pair * current = sum(running_sum, inputs[i]);
            delete running_sum;
            running_sum = current;
        }
        
        string ret = running_sum->to_string();
        delete running_sum;
        for (int i=0; i<inputs.size(); i++)
        {
            delete inputs[i];
        }
        
        return ret;
    }
    if (test == "mag")
    {
        vector<Pair *> inputs = parse_input(filename);
        
        long magnitude = inputs[0]->get_magnitude();
        delete inputs[0];
        ostringstream out;
        out << magnitude;
        return out.str();
    }
    
    return "";
}

string AocDay18::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() > 0)
    {
        return run_test(filename, extra_args[0]);
    }
    
    vector<Pair *> inputs = parse_input(filename);
        
    Pair * running_sum = (Pair *)inputs[0]->clone();
    
    for (int i=1; i<inputs.size(); i++)
    {
        Pair * current = sum(running_sum, inputs[i]);
        delete running_sum;
        running_sum = current;
    }
    
    ostringstream out;
    out << running_sum->get_magnitude();
    
    delete running_sum;
    for (int i=0; i<inputs.size(); i++)
    {
        delete inputs[i];
    }
    
    return out.str();
}

string AocDay18::part2(string filename, vector<string> extra_args)
{
    vector<Pair *> inputs = parse_input(filename);
        
    long max_magnitude = 0;
    
    for (int first=0; first<inputs.size(); first++)
    {
        for (int second=0; second<inputs.size(); second++)
        {
            if (first != second)
            {
                Pair * current = sum(inputs[first], inputs[second]);
                long magnitude = current->get_magnitude();
                if (magnitude > max_magnitude)
                {
                    max_magnitude = magnitude;
                }
                delete current;
            }
        }
    }
    
    ostringstream out;
    out << max_magnitude;
    
    for (int i=0; i<inputs.size(); i++)
    {
        delete inputs[i];
    }
    
    return out.str();
}
