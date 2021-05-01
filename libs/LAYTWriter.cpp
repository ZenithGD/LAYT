#include <LAYTWriter.hpp>
#include <LaytExceptions.hpp>

using namespace std;

Writer::Writer(const string& header, const string& footer)
	: _header(header), _footer(footer) {}
	
void Writer::open(const string& file_name) 
{
	_os.open(file_name);
	if ( _os.is_open() ) {
		_os << _header;
	}
	else throw writer_open_exception("Can't open", file_name);
}

Writer::~Writer() 
{
	_os << endl;
	_os.close();
}
	
LogiWriter::LogiWriter() 
	: Writer("v2.0 raw\n"), _counter(0) {}
	
void LogiWriter::write(const string& str) 
{
	_os << str;
	if ( ++_counter == 8 ) {
		_os << endl;
		_counter = 0;
	}
	else {
		_os << " ";
	}
}

