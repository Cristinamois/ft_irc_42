#include "Utilities.hpp"
#include <strings.h>
#include <iostream>
#include "RFC1459.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>

static bool is_valid_token(const string& s) {
    return !s.empty() && s.find_first_not_of(' ') != std::string::npos;
}

vector<string> split(const string& s, const string& delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (is_valid_token(token)) res.push_back(token);
    }

    token = s.substr(pos_start);
    if (is_valid_token(token)) res.push_back(token);
    return res;
}

parsed_cmd_t parse(const string& s)
{
    parsed_cmd_t ret;
    vector<string> vec;

    bzero(&ret, sizeof(ret));
    vec = split(s, ":");
    ret.has_suffix = (vec.size() == 1) ? false : true;
    if (ret.has_suffix)
        ret.suffix = vec[1];

    vec = split(vec[0], " ");
    ret.cmd = vec[0];
    for (size_t i = 0; i < ret.cmd.size(); ++i)
        ret.cmd[i] = (char)std::toupper(ret.cmd[i]);
    vec.erase(vec.begin());
    
    ret.has_words = (vec.size() == 0) ? false : true;
    ret.words = vec;

    return ret;
}

string format(const string& s, const vector<string>& values)
{
	size_t b, e;
    string str = s;
	
	for (size_t i = 0; i < values.size(); ++i)
	{	
		b = str.find('<'); e = str.find('>');
		if (!values[i].empty())
			str.replace(b, e - b + 1, values[i]);
		else
			str.erase(b, e - b + 2);
	}

	return str + RFC1459_END;
}

string getDateAndTime(bool inc_date, bool inc_time)
{
    std::stringstream ret;
	std::time_t time;
	std::time(&time);
	std::tm * t = std::localtime(&time);

    if (inc_date)
    {
        switch (t->tm_wday)
        {
        case 0:
            ret << "Sunday ";
            break;
        case 1:
            ret << "Monday ";
            break;
        case 2:
            ret << "Tuesday ";
            break;
        case 3:
            ret << "Wednesday ";
            break;
        case 4:
            ret << "Thursday ";
            break;
        case 5:
            ret << "Friday ";
            break;
        case 6:
            ret << "Saturday ";
            break;
        default:
            ret << "UNKDAY ";
            break;
        }

        switch (t->tm_mon)
        {
        case 0:
            ret << "January ";
            break;
        case 1:
            ret << "February ";
            break;
        case 2:
            ret << "March ";
            break;
        case 3:
            ret << "April ";
            break;
        case 4:
            ret << "May ";
            break;
        case 5:
            ret << "June ";
            break;
        case 6:
            ret << "July ";
            break;
        case 7:
            ret << "August ";
            break;
        case 8:
            ret << "September ";
            break;
        case 9:
            ret << "October ";
            break;
        case 10:
            ret << "November ";
            break;
        case 11:
            ret << "December ";
            break;
        default:
            ret << "UNKMONTH ";
            break;
        }

        ret << t->tm_mday << " " << t->tm_year + 1900;
    }

    if (inc_date && inc_time)
        ret << " -- ";

    if (inc_time)
    {
        ret << std::setfill('0') << std::setw(2) << t->tm_hour << ":" << std::setfill('0') << std::setw(2) << t->tm_min << ":" << std::setfill('0') << std::setw(2) << t->tm_sec;
        ret << " +01:00";
    }

    return ret.str();
}

parsed_mode_t parse_mode(string s, bool for_client)
{
    parsed_mode_t ret;
    size_t pos;
    string bad = for_client ? RFC1459_USER_MODES : RFC1459_CHANNEL_MODES;

    ret.valid = false;
    ret.invalid_mode = "?";
    ret.sign = true;
    ret.modes = "";

    if (s.empty())
        return ret;
    
    if (s[0] == '-')
        ret.sign = false;
    
    if (s[0] == '-' || s[0] == '+')
        s.erase(0, 1);
    
    if (s.empty())
        return ret;

    pos = s.find_first_not_of(bad);
    if (pos != string::npos)
    {
        ret.invalid_mode = s[pos];
        return ret;
    }

    ret.valid = true;
    ret.modes = s;
    return ret;
}

