#ifndef CNF_CNF_HANDLDER_H
#define CNF_CNF_HANDLDER_H
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include "Parser/netlist_parser.h"
class CNF_handler
{

friend netlist_parser_ABC;

public:
	CNF_handler() = default;
	CNF_handler(const netlist_parser_ABC* info);

	virtual ~CNF_handler() = default;

protected:
	virtual std::vector<std::string> connect_nets(const std::string& net_name1, const std::string& net_name2);
	virtual std::vector<std::string> connect_nets(const std::string& net_name, const unsigned& offset);		
	virtual std::vector<std::string> connect_nets(const unsigned& net_index1, const unsigned& net_index2);	

	virtual std::vector<std::vector<std::string>> duplicate_circuit();
	virtual std::vector<std::vector<std::string>> duplicate_circuit(const unsigned& offset);

	virtual std::string assign(const std::string& net_name, const bool& value);
	virtual std::string assign(const unsigned& net_index, const bool& value);
	virtual std::vector<std::string> assign(const std::vector<std::string>& net_name_list, const std::vector<bool>& value_list);

private:
	netlist_parser_ABC* target;
	unsigned net_amount;
};


//============================================================================
//copy control
CNF_handler::CNF_handler(const netlist_parser_ABC* info):target(info),net_amount(target->varIndexDict.size()){}

//============================================================================
// interface


// connect
std::vector<std::string> CNF_handler::connect_nets(const std::string& net_name1, const std::string& net_name2)
{
	unsigned net_index1 = target->varIndexDict(net_name1);
	unsigned net_index2 = target->varIndexDict(net_name2);
	return connect_nets(net_index1, net_index2);
}

std::vector<std::string> CNF_handler::connect_nets(const std::string& net_name, const unsigned& offset)	
{
	unsigned net_index1 = target->varIndexDict(net_name);
	unsigned net_index2 = net_index1 + offset;
	return connect_nets(net_index1, net_index2);
}	

std::vector<std::string> CNF_handler::connect_nets(const unsigned& net_index1, const unsigned& net_index2)
{
	std::vector<std::string> result;
	result.push_back(std::to_string(net_index1) + " -" + std::to_string(net_index2) + " 0");
	result.push_back("-" + std::to_string(net_index1) + " " + std::to_string(net_index2) + " 0");
	return result;
}	

// duplicate
std::vector<std::vector<std::string>> CNF_handler::duplicate_circuit()
{
	return duplicate_circuit(net_amount);
}
std::vector<std::vector<std::string>> CNF_handler::duplicate_circuit(const unsigned& offset)
{
	std::vector<std::vector<std::string>> result;
	for(auto gate: target->CNF)
	{
		std::vector<std::string> temp_gate;
		for(auto line: gate)
		{
			std::string temp_line;
			stringstream s(line);
			int num;
			while(s >> num)
			{
				if(num > 0) temp_line += std::to_string(num + offset) + " ";
				else if(num < 0) temp_line += "-" + std::to_string(abs(num) + offset) + " ";
			}
			temp_line += "0\n";
			temp_gate.push_back(temp_line);
		}
		result.push_back(temp_gate);
	}
	return result;
}

// assign
std::string CNF_handler::assign(const std::string& net_name, const bool& value)
{
	auto net_index = target->varIndexDict[net_name];
	return assign(net_index, value);
}

std::string CNF_handler::assign(const unsigned& net_index, const bool& value)
{
	if(value == true) return std::to_string(net_index) + " 0\n";
	if(value == false) return "-" + std::to_string(net_index) + " 0\n";
}

std::vector<std::string> assign(const std::vector<std::string>& net_name_list, const std::vector<bool>& value_list)
{
	for(auto net_iter = net_name_list.cbegin(), auto val_iter = value_list.cbegin(); net_iter != net_name_list.cend(); ++net_iter, ++value_list)
	{
		assign(*net_iter, *value_list);
	}
}

#endif