#include <regex>
#include <sstream>
#include <iostream>

#include <LayoutParser.hpp>
#include <LaytExceptions.hpp>

using namespace std;

LayoutParser::LayoutParser(istream& is, const string& layt_path)
	: _layt_in(is)
{
	string path(realpath(layt_path.c_str(), nullptr));
	_imports.emplace(path);
}

void LayoutParser::parse_fields(Instruction& inst) 
{
	string token;
	_layt_in >> token;
	while ( token != "end" && !_layt_in.eof() ) {
		parse_exception field_xc("Can't recognize token", "field | end", token);
		parse_xc_wrapper( field_xc, token != "field" && token != "end");
		
		_layt_in >> token;
		// read field data based on field type
		regex r_field("(\\$|#|@)(.+)");
		smatch sm;
		parse_exception param_xc("Can't recognize field type", "($|#|@)<name>", token);
		parse_xc_wrapper( param_xc, !regex_match(token, sm, r_field));
		string name = sm[2].str();
		_layt_in >> token;

		unsigned int start, end;
		char range_op;
		istringstream is(token);
		is >> start >> range_op >> end;
		// check range operator
		parse_exception range_xc("Not a range", ":", string(1, range_op));
		parse_xc_wrapper( range_xc, range_op != ':' );

		Field f(name, start, end);
		inst.addField(f);

		_layt_in >> token;
	}

	parse_exception end_xc("End keyword not found", "end", "<EOF>");
	parse_xc_wrapper( end_xc, token != "end" && _layt_in.eof());
}

void LayoutParser::parse_layout(std::map<std::string, Instruction>& isa)
{
	cout << INFO << " [1 of 2] Parsing layout..." << endl;
	string token;
	_layt_in >> token;
	while (!_layt_in.eof())
	{
		parse_exception inst_xc("Can't recognize token", "inst | import", token);
		parse_xc_wrapper(inst_xc, token != "inst" && token != "import");
		if ( token == "inst" ) {
			string name;
			char range_op;
			unsigned int opcode;
			unsigned int start, end;

			// read instruction header data
			_layt_in >> name >> opcode >> start >> range_op >> end;
			Instruction inst(opcode, start, end);

			// check range operator
			parse_exception range_xc("Not a range", ":", string(1, range_op));
			parse_xc_wrapper( range_xc, range_op != ':' );

			// parse fields until 'end' token is found
			parse_fields(inst);
			
			// add instruction into isa
			isa.insert(make_pair(name, inst));
		}
		else { // token == "import"
			// TODO: manage imports 
			// For now ignore import directives
		}
		_layt_in >> token; // read next statement
	}
}

LayoutParser::~LayoutParser() 
{
	
}
