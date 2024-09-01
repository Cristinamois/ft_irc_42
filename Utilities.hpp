#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct parsed_cmd_s
{
    string cmd;
    bool has_words;
    vector<string> words;
    bool has_suffix;
    string suffix;
} parsed_cmd_t;

typedef struct parsed_mode_s
{
    bool valid;
    string invalid_mode;
    bool sign;
    string modes;
} parsed_mode_t;

vector<string> split(const string& s, const string& delimiter);
parsed_cmd_t parse(const string& s);
string format(const string& s, const vector<std::string>& values);
string getDateAndTime(bool inc_date, bool inc_time);
parsed_mode_t parse_mode(string s, bool for_client);
