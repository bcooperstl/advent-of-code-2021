#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include <vector>
#include <string>

using namespace std;

class FileUtils {
    private:
        vector<string> split_line_to_strings(string input, char * delimiters, int num_delimiters, char quote_char, char comment_char);
    public:
        bool read_as_list_of_strings(string filename, vector<string> & lines);
        bool read_as_list_of_longs(string filename, vector<long> & values);
        bool read_as_list_of_split_strings(string filename, vector<vector<string>> & split_strings, char * delimiters, int num_delimiters, char quote_char, char comment_char);
        bool read_as_list_of_split_longs(string filename, vector<vector<long>> & split_longs, char * delimiters, int num_delimiters, char quote_char, char comment);
        bool read_as_list_of_split_strings(string filename, vector<vector<string>> & split_strings, char delimiter, char quote_char, char comment_char);
        bool read_as_list_of_split_longs(string filename, vector<vector<long>> & split_longs, char delimiter, char quote_char, char comment);
};

#endif
