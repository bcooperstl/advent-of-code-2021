#ifndef __AOC_DAY_13__
#define __AOC_DAY_13__

#include "aoc_day.h"

#include <vector>
#include <set>

#define FOLD_HORIZONTAL 'y'
#define FOLD_VERTICAL 'x'

namespace Day13
{
    struct Dot
    {
        int x;
        int y;
        bool operator <(const Dot & other) const;
    };
    
    struct FoldInstruction
    {
        char direction;
        int position;
    };
}

using namespace Day13;

class AocDay13 : public AocDay
{
    private:
        void parse_input(string filename, set<Dot> & dots, vector<FoldInstruction> & folds);
        void display_sheet(set<Dot> & dots);
        set<Dot> perform_fold(set<Dot> & input_dots, FoldInstruction fold);
    public:
        AocDay13();
        ~AocDay13();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
