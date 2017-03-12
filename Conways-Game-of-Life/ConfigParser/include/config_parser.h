#pragma once
#include <map>
#include <vector>

class ConfigParser
{

public:

	std::map<std::string, std::string> parse(const std::string& file) const;

	static bool find(std::map<std::string, std::string>& map, const std::string& key, std::string& out);

	static bool to_ullong(const std::string& input, unsigned long long& out);
	static bool to_llong(const std::string& input, long long& out);
	static bool to_long(const std::string& input, long& out);
	static bool to_uint(const std::string& input, unsigned int& out);
	static bool to_int(const std::string& input, int& out);
	static bool to_float(const std::string& input, float& out);
	static bool to_ldouble(const std::string&input, long double& out);
	static bool to_double(const std::string& input, double& out);
	static bool to_bool(std::string input, bool& out);

private:

	static bool get_value(std::string &string, std::string& out);
	static std::vector<std::string> split(const std::string& string, const char& delimiter);
	static std::string& erase(std::string& string, const char& character);
	static std::string& lower(std::string& string);

};

