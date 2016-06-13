#ifndef CNF_ATTACK_CNF_HANDLER_H
#define CNF_ATTACK_CNF_HANDLER_H
#include "CNF/CNF_handler.h"
#include "attack/constructor.h"
class Attack_CNF_handler: public virtual CNF_handler
{
public:
	Attack_CNF_handler() = default;
	Attack_CNF_handler(const netlist_parser_ABC* info, const constructor& circuit);

	void progess_inport_CB();
private:
	vector<std::vector<std::string>> process_CB(std::vector<std::string> CBs);
}
#define 