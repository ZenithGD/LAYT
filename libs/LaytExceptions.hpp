#pragma once
#include <iostream>
#include <exception>

#include <Instruction.hpp>

const std::string ERR 	= "\e[31m[LAYT_ERR]>\e[39m";
const std::string WARN 	= "\e[93m[LAYT_WARN]>\e[39m";
const std::string INFO 	= "\e[34m[LAYT_INFO]>\e[39m";

// Exception for fields
class field_exception : public std::runtime_error
{
	const Field* _f;
	public:
		field_exception(const std::string& msg, const Field* f);
		std::string field_values() const;
};

// Exception for instructions
class instruction_exception : public std::runtime_error
{
	public:
		instruction_exception(const std::string& msg);
};

class parse_exception : public std::runtime_error 
{
	std::string _expect;
	std::string _found; 
	public:
		parse_exception(const std::string& msg, const std::string& exp, const std::string& found);
		std::string parse_status();
};

void parse_xc_wrapper(parse_exception& xc, bool condition);

class src_parse_exception : public std::runtime_error 
{
	std::string _found; 
	public:
		src_parse_exception(const std::string& msg, const std::string& found);
		std::string src_parse_status();
};

void src_parse_xc_wrapper(src_parse_exception& xc, bool condition);