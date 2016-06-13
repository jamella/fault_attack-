#ifndef PARSER_FAULT_PARSER_H
#define PARSER_FAULT_PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
class Fault_parser
{
public:
	Fault_parser();
	Fault_parser(std::string path_info);

private:
	void parse_line(const std::string&);
	const std::string path;
	std::ifstream infile;

	std::vector<std::string> list;
};

Fault_parser::Fault_parser(std::string path_info):path(path_info)
{
	infile.open(path);
	{
		if(!infile.is_open())
		{
			std::cerr << "file is not open correctly!" << std::endl;
		}
		else
		{
			std::string line;
			while(getline(infile, line))
			{
				list.push_back(line);
				parse_FF_line(line);
				getline(infile, line);
				parse_S1_line(line);
				getline(infile, line);
				parse_S0_line(line);
			}
		}
	}
}

void Fault_parser::parse_line(const std::string& line)
{

}

#endif