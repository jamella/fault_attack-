#ifndef CNF_ATTACK_CNF_HANDLER_H
#define CNF_ATTACK_CNF_HANDLER_H

#include "CNF/CNF_handler.h"
#include "attack/constructor.h"
class Attack_CNF_handler: public virtual CNF_handler
{
public:
	Attack_CNF_handler() = default;
	Attack_CNF_handler(const netlist_parser_ABC* circuit_info, const constructor constructor_info):CNF_handler(circuit_info), circuit(constructor_info){}

	void process_inport_CB();
//private:
	const constructor circuit;
	std::vector<std::string> CB_constrains;
	std::vector<std::string> process_CB(std::vector<std::string> CBs);
};




void Attack_CNF_handler::process_inport_CB()
{
	unsigned gate_index = 0;
	for(auto for_each_gate: circuit.inports_CB_list)  // each module in netlist
	{
		for(auto for_each_fan: for_each_gate)		// each fanin in netlist
		{
			auto temp = process_CB(for_each_fan);
			CB_constrains += temp;	// generate constrain for certain fan inport
		}
	}
	CNF.push_back(CB_constrains);
}


vector<std::string> Attack_CNF_handler::process_CB(std::vector<std::string> CBs)
{
	auto dict = target->varIndexDict;
	std::vector<std::string> result;
	std::string head_cnf;
	for(auto net: CBs)
	{
		head_cnf += std::to_string(dict.at(net)) + " "; 
		std::cerr << "net: " << net << " = " << dict.at(net) << std::endl;
	}
	head_cnf += "0\n";
	result.push_back(head_cnf);
	std::cerr << "head_cnf = " << head_cnf << std::endl;
	for(auto target_net: CBs)
	{
		for(auto net: CBs)
		{
			if(net == target_net) continue;
			else
			{
				std::string temp = "-" + std::to_string(dict.at(target_net)) + " " + "-" + std::to_string(dict.at(net)) + " 0\n";
				result.push_back(temp);
			}
		}
	}

	return result;

}


#endif