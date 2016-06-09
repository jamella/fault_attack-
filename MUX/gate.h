#ifndef MUX_GATE_H
#define MUX_GATE_H
#include <map>
#include <string>
#include <iostream>
#include "Parser/parser.h"
#include "utils/tools.h"
#include "MUX/MUX.h"

class Gate
{
	friend class constructor;
public:
	Gate() = default;
	Gate(const unsigned& fanin_num, const unsigned& Gate_index):index(Gate_index),fanin(fanin_num){}	

	void show_info();
	void show_model();
	std::vector<std::string> build_gate(const Parser&);	

private:
	void collect_input(const Parser& parser);
	std::vector<std::string> build_body();
	std::vector<std::string> build_inport(const unsigned& inport_index);
	std::vector<std::string> name_CB(const unsigned& index, const std::string& type, const unsigned& component_index, const unsigned& input_size);
	std::string name_output(const unsigned& index, const std::string& type, const unsigned& inport_index);

	unsigned index;
	unsigned fanin;

	std::vector<std::string> input;
	std::vector<std::string> CB;
	std::vector<std::string> wire;
	std::string output;
	std::vector<std::vector<std::string>> inport_CB_list;
	std::vector<std::string> body_CB;
	std::vector<std::string> Gate_model;

	std::vector<std::string> body_input;
};


//=============================================================
//interface

std::vector<std::string> Gate::build_gate(const Parser& parser)
{
	collect_input(parser);
	for(unsigned i = 0; i != fanin; ++i)
	{
		Gate_model+=build_inport(i);
	}
	Gate_model+=build_body();

	for(auto i: inport_CB_list)
	{
		CB += i;
	}
	CB += body_CB;


	return Gate_model;
}	

void Gate::show_info()
{
	std::cerr << "========================================" << std::endl;
	std::cerr << "Gate index: " << index << std::endl;
	std::cerr << "Gate fanin_num:" << fanin << std::endl;
	std::cerr << std::endl;
	std::cerr << "input info:" << fanin << std::endl;
	for(auto i: input)
	{
		std::cerr << i << std::endl;
	}
	std::cerr << std::endl;
	std::cerr << "port CB info:" << std::endl;
	for(auto i: inport_CB_list)
	{
		static auto c = 0;
		std::cerr << "Port index = " << c << std::endl;
		for(auto j: i)
		{
			std::cerr << "\t" << j << std::endl;

		}
		++c;
	}
	std::cerr << std::endl;
	std::cerr << "body CB info:" << std::endl;
	{
		for(auto i: body_CB)
		{
			std::cerr << i << std::endl;
		}
	}
	std::cerr << std::endl;
	std::cerr << "body CB input:" << std::endl;
	{
		for(auto i: body_input)
		{
			std::cerr << i << std::endl;
		}
	}
	std::cerr << std::endl;
	std::cerr << "output:" << std::endl;
	std::cerr << output << std::endl;
	std::cerr << "========================================" << std::endl;
}

void Gate::show_model()
{
	for(auto item: Gate_model)
	{
		std::cerr << item << std::endl;
	}
}

//=============================================================
//implementation

void Gate::collect_input(const Parser& parser)
{
	input = parser.PI;
	for(unsigned i = 0; i < parser.gate_counter; ++i)
	{
		if(i != index)
		{
			input.push_back("gate" + std::to_string(i) + "_in");
		}
	}
}

std::vector<std::string> Gate::build_inport(const unsigned& inport_index)
{
	auto inport_CB = name_CB(index, "fanin", inport_index, input.size());
	auto inport_output = name_output(index, "fanin", inport_index);

	inport_CB_list.push_back(inport_CB);
	Cool_Mux result(input, inport_CB, inport_output);
	wire += result.wire;
	wire.push_back(inport_output);
	return result.to_vector(index, inport_index);
}

std::vector<std::string> Gate::build_body()
{
	std::string body_output = "gate" + std::to_string(index) + "_out";
	auto input_size = std::pow(2, fanin);

	for(unsigned i = 0; i != fanin; ++i)
	{
		body_CB.push_back(name_output(index, "fanin", i));
	}

	for(int i = 0; i != input_size; ++i)
	{
		body_input.push_back("gate" + std::to_string(index) + "_body_CB_" + std::to_string(i));
	}
	output = body_output;
	Standard_Mux result(body_input, body_CB, body_output);
	return result.to_vector(index);
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
	return "gate" + std::to_string(index) + "_" + type + std::to_string(inport_index); 
}
/*
std::vector<std::vector<std::string>> Gate::collect_port_wire()
{
	std::vector<std::vector<std::string>> result;
	for(unsigned fan_index = 0; fan_index != fanin; ++fan_index)
	{
		std::vector<std::string> single_port_wire;
		for(unsigned input_index = 0; input_index != input.size(); ++input_index)
		{
			single_port_wire.push_back("gate" + std::to_string(index) + "_fanin" + std::to_string(fan_index) + "_CB_" + std::to_string(input_index);
		}
		single_port_wire.push_back("gate" + std::to_string(index) + "_fanin" + std::to_string(fan_index));
		result.push_back(single_port_wire);
	}
	return result;
}
*/


#endif