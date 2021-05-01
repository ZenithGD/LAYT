#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>

#include <Instruction.hpp>
#include <LayoutParser.hpp>
#include <SourceParser.hpp>
#include <LAYTWriter.hpp>
#include <LaytExceptions.hpp>

using namespace std;

int main(int argc, char* argv[])
{
	string layt(argv[1]), source_file(argv[2]);

	// Parser object
	LayoutParser parser(cin, layt);
	
	// Instruction set
	map<string, Instruction> isa;

	// Parse the layout file
	parser.parse_layout(isa);

	// Source parser object
	SourceParser src_parser(isa);

	LogiWriter lw;
	try
	{
		lw.open(argv[3]);
	}
	catch(const writer_open_exception& e)
	{
		cout << ERR << " " << e.what() << ": " << e.writer_open_exception_status() << '\n';
		throw e;
	}

	// Argument vector (instruction name, arguments)
	vector<pair<string, vector<uint32_t>>> arg_vector = src_parser.parse_source(source_file);

	// Evaluate every instruction in source
	for ( auto args : arg_vector ) {
		Instruction inst = isa.at(args.first);
		uint32_t value = inst.eval(args.second);
		cout << args.first << " " << inst.eval_str(args.second) << endl;
		ostringstream os;
		os << hex << value;
		lw.write(os.str());
		cout << "\e[1;4mbin:\e[0m " << bitset<32>(value)
				<< ", \e[1;4mhex:\e[0m 0x" << setfill('0') << setw(8) << hex << value
				<< dec << endl;
	}
	
	
	return 0;
}