#include <LaytExceptions.hpp>
#include <Instruction.hpp>

using namespace std;

field_exception::field_exception(const string& msg, const Field* f)
	: runtime_error(msg), _f(f)
{
}

string field_exception::field_values() const
{
	return string(to_string(_f->get_start()) + ", " + to_string(_f->get_end()));
}

instruction_exception::instruction_exception(const std::string& msg) 
	: runtime_error(msg)
{
}

parse_exception::parse_exception(const string &msg, const string& exp, const string &found)
	: runtime_error(msg), _expect(exp), _found(found)
{
}

string parse_exception::parse_status() const
{
	return "Expected \'" + _expect + "\', found \'" + _found + "\'.";
}

// Wrapper for simple parsing exceptions
void parse_xc_wrapper(parse_exception& xc, bool condition)
{
	try {
		if ( condition )
			throw xc;
	}
	catch ( parse_exception& x ) {
		cout << ERR + " Parsing exception: " << x.parse_status() << endl;
		throw x;
	}
}

src_parse_exception::src_parse_exception(const std::string& msg, const std::string& found) 
	: runtime_error(msg), _found(found)
{	
}

std::string src_parse_exception::src_parse_status() const
{
	return "Found \'" + _found + "\'.";
}

void src_parse_xc_wrapper(src_parse_exception& xc, bool condition) 
{
	try {
		if ( condition )
			throw xc;
	}
	catch ( src_parse_exception& x ) {
		cout << ERR + " Source parsing exception: " << x.src_parse_status() << endl;
		throw x;
	}
}

writer_open_exception::writer_open_exception(const std::string& msg, const std::string& fn)
	: runtime_error(msg), _file_name(fn) {}

std::string writer_open_exception::writer_open_exception_status() const
{
	return "\'" + _file_name + "\' not found.";
}