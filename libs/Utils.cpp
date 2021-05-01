#include "Utils.hpp"

using namespace std;

vector<string> tokenize(const string& str, char delim, const string& trail) 
{
	istringstream ss(str);
	string token;
	vector<string> sv;
	while (getline(ss, token, delim)) 
	{
		sv.push_back( ( trail == "" ) ? token : trim(token, trail) );
	}
	return sv;
}

string trim_trailing(const string& str, const std::string& c) 
{
	size_t end = str.find_last_not_of(c);
	return (end == string::npos) ? "" : str.substr(0, end + 1);
}

std::string trim_leading(const std::string& str, const std::string& c) 
{
	size_t start = str.find_first_not_of(c);
	return (start == string::npos) ? "" : str.substr(start);
}

std::string trim(const std::string& str, const std::string& c) 
{
	return trim_trailing(trim_leading(str, c), c);
}