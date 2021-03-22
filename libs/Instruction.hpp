#pragma once

#include <iostream>
#include <vector>

class Field
{
	private:
		unsigned int _start_bit;
		unsigned int _end_bit;

		std::string _name;

		// TODO: store field type:
		//		Inheritance -> Register field, address field or constant field
		//		enum value for field type <- restricts modularity

	public:
		// Constructor (start bit, end bit)
		// Will throw an exception if start < end
		Field(const std::string& name, unsigned int start, unsigned int end);
		// Return 32-bit integer with evaluated bits
		// value should be able to be encoded with (_start_bit - _end_bit) bits,
		// will throw an exception otherwise
		uint32_t eval(const unsigned int value) const;

		// Getters
		unsigned int get_start() const;
		unsigned int get_end() const;
		std::string  get_name() const;

};

class Instruction 
{
	private:
		// Instruction layout
		// <field_name, <start_bit, end_bit>> 
		std::vector<Field> 	_layout;

		// Instruction opcode
		unsigned int _opcode;
		
		bool check_unique(const std::string& fieldname);

	public:

		// Constructor (creates instruction and allocates (start:end) bits for opcode)
		Instruction(unsigned int opcode, unsigned int start, unsigned int end);
		
		// Adds a field into the layout if possible
		bool addField(const Field& field);

		// Evaluate an instruction
		// Pre: args.size() >= _layout.size()
		// Post: if precondition doesn't hold, it will throw an instruction exception,
		//		 otherwise it will return a 32-bit unsigned integer with the evaluated word
		uint32_t eval(const std::vector<unsigned int> args) const;

		// Argument count
		int arg_count() const;
};
