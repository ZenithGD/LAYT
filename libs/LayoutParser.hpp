#pragma once

#include <iostream>
#include <map>

#include <Instruction.hpp>
#include <set>

class LayoutParser 
{
	private:
		// Reading streams
		std::istream& _layt_in;

		// Import paths set
		std::set<std::string> _imports;

		void parse_fields(Instruction& inst);

	public:
		// Constructor (read from is and add layt_path to import set
		LayoutParser(std::istream& is, const std::string& layt_path);

		// Parse a ".layt" file into the instruction mapping
		void parse_layout(std::map<std::string, Instruction>& isa);

		// Destructor
		~LayoutParser();
};