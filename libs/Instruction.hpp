#pragma once

#include <iostream>
#include <vector>

class Field
{
	private:
		uint32_t _start_bit;
		uint32_t _end_bit;
		std::string _name;

	public:
		// Constructor (start bit, end bit)
		// Will throw an exception if start < end
		Field(const std::string& name, uint32_t start, uint32_t end);
		// Return 32-bit integer with evaluated bits
		// value should be able to be encoded with (_start_bit - _end_bit) bits,
		// will throw an exception otherwise
		uint32_t eval(const uint32_t value) const;
		// Return a string representing the field value, based on its type.
		virtual std::string str_eval(const uint32_t value) const;

		// Getters
		uint32_t 	get_start() const;
		uint32_t 	get_end() const;
		std::string get_name() const;

};

// A class representing inmediate fields (constant values)
class InmediateField : public Field
{
	public:
		InmediateField(const std::string& name, uint32_t start, uint32_t end);
		
		std::string str_eval(const uint32_t value) const override;
};

// A class representing register fields
class RegisterField : public Field
{
	public:
		RegisterField(const std::string& name, uint32_t start, uint32_t end);
		
		std::string str_eval(const uint32_t value) const override;
};

// A class representing address fields
class AddressField : public Field
{
	public:
		AddressField(const std::string& name, uint32_t start, uint32_t end);
		
		std::string str_eval(const uint32_t value) const override;
};

// TODO: Create Opcode field and opcode -> instruction mapping structure

class Instruction 
{
	private:
		// Instruction layout
		// <field_name, <start_bit, end_bit>> 
		std::vector<Field*> 	_layout;

		// Instruction opcode
		uint32_t _opcode;
		
		// check if field "fieldname" exists in the layout vector already
		bool check_unique(const std::string& fieldname);

	public:

		// Constructor (creates instruction and save (start:end) bits for opcode)
		Instruction(uint32_t opcode, uint32_t start, uint32_t end);
		
		// Adds a field into the layout if possible
		bool addField(Field* field);

		// Evaluate an instruction
		// Pre: -
		// EC: args.size() >= _layout.size()
		// Post: if EC doesn't hold, it will throw an instruction exception,
		//		 otherwise it will return a 32-bit unsigned integer with the evaluated word
		uint32_t eval(const std::vector<uint32_t> args) const;

		// Evaluate an instruction and return string representation.
		// Pre: -
		// EC: args.size() >= _layout.size()
		// Post: if EC doesn't hold, it will throw an instruction exception,
		//		 otherwise it will return a 32-bit unsigned integer with the evaluated word
		std::string eval_str(const std::vector<uint32_t> args) const;

		// Argument count
		int arg_count() const;
};
