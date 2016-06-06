#ifndef MUX_MUX_H
#define MUX_MUX_H
#include <map>
#include <string>
#include <iostream>
#include "Parser/parser.h"
#include "utils/tools.h"

class Gate
{
public:
	Gate() = default;
	Gate(const unsigned& fanin_num, const unsigned& Gate_index):index(Gate_index),fanin(fanin_num){}	

	std::vector<std::string> build_gate(const Parser&);	
	
	std::vector<std::string> build_unit_mux(const std::vector<std::string> input_info, const std::vector<std::string> CB_info, const std::string output_info);
private:

	void collect_input(const Parser& parser);
	std::vector<std::string> build_body();
	std::vector<std::string> build_inport(const unsigned& inport_index);
	std::vector<std::string> name_CB(const unsigned& index, const std::string& type, const unsigned& component_index, const unsigned& input_size);
	std::string name_output(const unsigned& index, const std::string& type, const unsigned& inport_index);

	unsigned index;
	unsigned fanin;
	std::string output;
	std::vector<std::string> input;
	std::vector<std::string> CB_inport;
	std::vector<std::string> CB_body;
	std::vector<std::string> Gate_model;
};


//=============================================================
//implementation

std::vector<std::string> Gate::build_gate(const Parser& parser)
{
	collect_input(parser);
	for(unsigned i = 0; i != fanin; ++i)
	{
		Gate_model+=build_inport(i);
	}
	Gate_model+=build_body();
	return Gate_model;
}	

void Gate::collect_input(const Parser& parser)
{
	input = parser.PI;
	for(unsigned i = 0; i < parser.gate_counter; ++i)
	{
		if(i != index)
		{
			input.push_back("gate" + std::to_string(i) + "_out");
		}
	}
}

std::vector<std::string> Gate::build_inport(const unsigned& inport_index)
{
	std::vector<std::string> result;
	auto inport_CB = name_CB(index, "fanin", inport_index, input.size());
	auto inport_output = name_output(index, "fanin", inport_index);
	result = build_unit_mux(input, inport_CB, inport_output);
	return result;
}

std::vector<std::string> Gate::build_body()
{
	std::vector<std::string> result;
	std::vector<std::string> body_CB;
	std::vector<std::string> body_input;
	std::string body_output = "gate" + std::to_string(index) + "_in";
	auto input_size = std::pow(2, fanin);

	for(unsigned i = 0; i != fanin; ++i)
	{
		body_CB.push_back(name_output(index, "fanin", i));
	}

	for(int i = 0; i != input_size; ++i)
	{
		body_input.push_back("gate" + std::to_string(index) + "_body_CB_" + std::to_string(i));
	}

	result = build_unit_mux(body_input, body_CB, body_output);
	return result;
}

std::vector<std::string> Gate::name_CB(const unsigned& index, const std::string& type, const unsigned& component_index, const unsigned& input_size)
{
	vector<std::string> result;
	for(unsigned i = 0; i < input_size; ++i)
	{
		result.push_back("gate" + std::to_string(index) + "_" + type + std::to_string(component_index) + "_CB_" + std::to_string(i));
	}
	return result;
}

std::string Gate::name_output(const unsigned& index, const std::string& type, const unsigned& inport_index) 
{
	return "gate" + std::to_string(index) + "_" + type + "_" + std::to_string(inport_index); 
}

std::vector<std::string> Gate::build_unit_mux(const std::vector<std::string> input_info, const std::vector<std::string> CB_info, const std::string output_info)
{
	std::vector<std::string> result;
	return result;
}
#endif