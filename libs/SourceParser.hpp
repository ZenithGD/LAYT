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

		static uint32_t parse_param(const std::string& arg);

		// parse next instruction in source
		bool parse_inst(std::ifstream& in, std::string& name, std::vector<uint32_t>& param);
	public:
		// Constructor ( initialize isa )
		SourceParser(std::map<std::string, Instruction>& isa);

		// Return a vector of argument vectors for evaluating instructions.
		std::vector<std::pair<std::string, std::vector<uint32_t>>> parse_source(const std::string& src);
};