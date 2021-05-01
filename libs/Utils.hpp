#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// Tokenize a string by delimiter 'del' and optionally trim the tokens if "trail" is not an empty string
std::vector<std::string> tokenize(const std::string& str, char del, const std::string& trail = "");

std::string trim_trailing(const std::string& str, const std::string& c);
std::string trim_leading(const std::string& str, const std::string& c);

// Trim from str trailing and leading characters found in c
std::string trim(const std::string& str, const std::string& c);

