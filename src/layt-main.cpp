#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>

#include <Instruction.hpp>
#include <LayoutParser.hpp>
#include <SourceParser.hpp>

using namespace std;

int main(int argc, char* argv[])
{
	string layt(argv[1]), source_file(argv[2]);

	// Parser object
	LayoutParser parser(cin, layt);
	
	// Instruction set
	map<string, Instruction> isa;

	parser.parse_layout(isa);

	SourceParser src_parser(isa);

	vector<pair<string, vector<unsigned int>>> arg_vector = src_parser.parse_source(source_file);

	// Show the evaluated instruction
	for ( auto args : arg_vector ) {
		Instruction inst = isa.at(args.first);
		uint32_t value = inst.eval(args.second);
		cout << args.first << " ";
		for ( unsigned int u : args.second ) {
			cout << u << " ";
		}
		cout << endl;
		cout << "\e[1;4mbin:\e[0m " << bitset<32>(value)
				<< ", \e[1;4mhex:\e[0m 0x" << setfill('0') << setw(8) << hex << value
				<< dec << endl;
	}
	
	return 0;
}