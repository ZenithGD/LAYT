#pragma once

#include <iostream>
#include <fstream>
#include <map>

#include <Instruction.hpp>

class SourceParser 
{
	private:
		// instruction set for evaluation
		std::map<std::string, Instruction>& _isa;

		// parse next instruction in source
		bool parse_inst(std::ifstream& in, std::string& name, std::vector<unsigned int>& param);
	public:
		// Constructor ( initialize isa )
		SourceParser(std::map<std::string, Instruction>& isa);

		// Return a vector of argument vectors for evaluating instructions.
		std::vector<std::pair<std::string, std::vector<unsigned int>>> parse_source(const std::string& src);
};