#pragma once

#include <vector>
#include <string>
#include <sstream>

inline std::vector<std::string> split(const std::string& s, char delim = ' ')
{
	std::istringstream ss(s);
	std::string item;
	std::vector<std::string> res;

	while (std::getline(ss, item, delim)) {
		res.push_back(item);
	}

	return res;
}