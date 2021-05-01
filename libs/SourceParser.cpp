#include <vector>
#include <exception>
#include <regex>
#include <SourceParser.hpp>
#include <Instruction.hpp>
#include <LaytExceptions.hpp>
#include <Utils.hpp>

using namespace std;

SourceParser::SourceParser(map<string, Instruction>& isa) 
	: _isa(isa)
{
}

uint32_t SourceParser::parse_param(const string& arg)
{
	regex rNum("#?([0-9]+)");
	regex rReg("r([0-9]+)");
	regex rAddr("\\(r([0-9]+)\\)");
	smatch smN, smR, smAddr;
	bool match_num 	= regex_match(arg, 	smN, 	rNum); 
	bool match_reg	= regex_match(arg, 	smR, 	rReg);
	bool match_addr = regex_match(arg, 	smAddr, rAddr);

	src_parse_exception xc("Token not recognized", arg);
	src_parse_xc_wrapper(xc, !match_num && !match_reg && !match_addr );
	string num;

	if 		( match_num ) num = smN[1].str();
	else if ( match_reg ) num = smR[1].str();
	else 			 	  num = smAddr[1].str(); // match_addr
	return (uint32_t) stoi(num);
}

/* Pre: !in.eof()
 * 
 */
bool SourceParser::parse_inst(ifstream& in, string& name, vector<uint32_t>& param) 
{
	string line;
	if ( !in.eof() ) {
		in >> name;
		getline(in, line);
		auto tokens = tokenize(line, ',', " \t");
		for ( auto it = tokens.begin(); it != tokens.end(); it++ ) 
		{
			uint32_t val = parse_param(*it);
			param.push_back(val);
		}
		return true;
	}
	else {
		return false;
	}
}

vector<pair<string, vector<uint32_t>>> SourceParser::parse_source(const string& src) 
{
	ifstream in(src);
	try {
		if ( !in.is_open() ) throw runtime_error("Can't open source file");
	}
	catch ( runtime_error& xc ) {
		cout << ERR << " File access exception: '" << src << "' not found." << endl;
		throw xc;
	}
	bool next;
	vector<pair<string,vector<uint32_t>>> res;
	cout << INFO << " [2 of 2] Parsing source..." << endl;
	do {
		vector<uint32_t> param;
		string name;
		next = parse_inst(in, name, param);
		if ( next ) {
			res.push_back(make_pair(name,param));
		}
	} while ( next );
	cout << INFO << " Done. " << endl;
	return res;
}