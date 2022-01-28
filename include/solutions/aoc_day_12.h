#ifndef __AOC_DAY_12__
#define __AOC_DAY_12__

#include "aoc_day.h"

#include <vector>
#include <map>

#define MAX_PATH_LENGTH 100

namespace Day12
{
    class Cave
    {
        protected:
            string m_name;
            vector<Cave *> m_neighbors;
        public:
            Cave(string name);
            virtual ~Cave();
            virtual bool is_big();
            virtual bool is_small();
            virtual bool is_start();
            virtual bool is_end();
            string get_name();
            vector<Cave *> get_neighbors();
            void add_neighbor(Cave * cave);
    };
    
    class StartCave : public Cave
    {
        public:
            StartCave(string name);
            ~StartCave();
            bool is_start();
    };
    
    class EndCave : public Cave
    {
        public:
            EndCave(string name);
            ~EndCave();
            bool is_end();
    };
    
    class BigCave : public Cave
    {
        public:
            BigCave(string name);
            ~BigCave();
            bool is_big();
    };
    
    class LittleCave : public Cave
    {
        public:
            LittleCave(string name);
            ~LittleCave();
            bool is_little();
    };
    
    class Path
    {
        private:
            Cave * m_path[MAX_PATH_LENGTH];
            int m_next_path;
        public:
            Path();
            ~Path();
            bool can_visit(Cave * next);
            void visit(Cave * next);
            bool is_complete();
            Cave * get_current_cave();
            void dump();
    };
}

using namespace Day12;

class AocDay12 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
        void find_paths(Path & current_path, vector<Path> & completed_paths);
    public:
        AocDay12();
        ~AocDay12();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
