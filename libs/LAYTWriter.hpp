#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class Writer
{
	protected:
		// file header
		std::string 		_header;
		// file footer
		std::string 		_footer;
		// file stream
		std::ofstream		_os;
	public:
		Writer(const std::string& header = "", const std::string& footer = "");
		
		/* open a file in write mode. If it fails, it throws a exception.
		 * If it succeeds, it opens 'file_name' and writes the header on it.
		 */
		void open(const std::string& file_name);

		// write data into the file
		virtual void write(const std::string& str) = 0;

		~Writer();
};

class LogiWriter : public Writer
{
	private:
		int _counter;
	public:
		LogiWriter();

		void write(const std::string& str) override;
};