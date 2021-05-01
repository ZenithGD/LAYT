#include <Instruction.hpp>
#include <vector>
#include <bitset>

#include <LaytExceptions.hpp>

using namespace std;

Instruction::Instruction(uint32_t opcode, uint32_t start, uint32_t end)
	: _opcode(opcode)
{
	Field* op = new Field("#", start, end);
	_layout.push_back(op);
}

Field::Field(const std::string& name, uint32_t start, uint32_t end) 
	: _start_bit(start), _end_bit(end), _name(name)
{
	try {
		if ( _start_bit < _end_bit ) 
			throw field_exception("Start bit must be equal or less than the end bit", this);
		if ( _start_bit >= 32 )
			throw field_exception("The bit indexes can't be greater than 31", this);
	}
	catch ( field_exception& xc ) {
		cout << ERR + " Field constructor exception: Field values: " << xc.field_values() << endl;
		throw xc;
	}
}

uint32_t Field::eval(const uint32_t value) const
{
	try {
		if ( value >= (1U << (_start_bit - _end_bit + 1U))) 
			throw field_exception("Value is too big", this);
	}
	catch ( field_exception& xc) {
		cout << ERR + " Evaluation exception: Value: " << value << ", Max Value: " << (1 << (_start_bit - _end_bit + 1)) << endl;
		throw xc;
	}
	// shift value to match the least signficant bit's position
	return value << _end_bit;
}

string Field::str_eval(const uint32_t value) const
{
	return to_string(value);
}

uint32_t Field::get_start() const
{
	return _start_bit;
}

uint32_t Field::get_end() const
{
	return _end_bit;
}

std::string Field::get_name() const
{
	return string(Field::_name);
}

InmediateField::InmediateField(const std::string& name, uint32_t start, uint32_t end) 
	: Field(name, start, end) {}

std::string InmediateField::str_eval(const uint32_t value) const 
{
	return "#" + Field::str_eval(value);
}

RegisterField::RegisterField(const std::string& name, uint32_t start, uint32_t end) 
	: Field(name, start, end) {}

std::string RegisterField::str_eval(const uint32_t value) const 
{
	return "r" + Field::str_eval(value);
}

AddressField::AddressField(const std::string& name, uint32_t start, uint32_t end) 
	: Field(name, start, end) {}

std::string AddressField::str_eval(const uint32_t value) const 
{
	return "(r" + Field::str_eval(value) + ")";
}

bool Instruction::check_unique(const string& fieldname) 
{
	for ( auto e : _layout ) {
		if ( e->get_name() == fieldname ) return false;
	}
	return true;
}

bool Instruction::addField(Field* field) 
{
	// TODO: check field overlapping
	string fieldname = field->get_name();
	if ( !check_unique(fieldname) ) {
		cout << WARN + " Warning: Duplicate fields can't be added (" + fieldname + ")" << endl;
		return false;
	}
	else {
		_layout.push_back(field);
		return true;
	}
}

uint32_t Instruction::eval(const std::vector<uint32_t> args) const
{
	try {
		if ( args.size() != _layout.size() - 1 )
			throw instruction_exception("too few arguments to evaluate");
	}
	catch ( instruction_exception& xc ) {
		if ( args.size() < _layout.size() - 1) {
			cout << ERR + " Instruction evaluation exception" << endl;
			throw xc;
		}
		else {
			cout << WARN + " Too many arguments to evaluate, some of them will be ignored" << endl;
		}
	}
	uint32_t word = 0U;
	auto it = args.begin();
	for ( auto field : _layout ) {
		if ( field->get_name() == "#" ) {
			word |= field->eval(_opcode);
		}
		else {
			word |= field->eval(*it);
			it++;
		}
	}
	return word;
}

string Instruction::eval_str(const std::vector<uint32_t> args) const
{
	try {
		if ( args.size() != _layout.size() - 1 )
			throw instruction_exception("too few arguments to evaluate");
	}
	catch ( instruction_exception& xc ) {
		if ( args.size() < _layout.size() - 1) {
			cout << ERR + " Instruction evaluation exception" << endl;
			throw xc;
		}
		else {
			cout << WARN + " Too many arguments to evaluate (" << args.size() << "), some of them will be ignored" << endl;
		}
	}
	string word = "";
	int lsiz = _layout.size();
	for ( int i = 1; i < lsiz; i++ ) {
		word += _layout[i]->str_eval(args[i - 1]);
		if ( i < lsiz - 1 ) word += ", ";
	}

	return word;
}

int Instruction::arg_count() const
{
	return _layout.size() - 1;
}