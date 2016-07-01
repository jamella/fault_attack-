#include <iostream>
#include <fstream>
#include "string"
#include "simp/SimpSolver.h"
#include "Parser/attack_parser.h"
#include "Parser/parser.h"
#include "Parser/fault_parser.h"
#include "attack/constructor.h"
#include "CNF/Attack_CNF_handler.h"
#include "solver/SAT.h"
#include "solver/print_solution.h"

using namespace std;

int main()
{
	cout << "Project started" << endl;
	Parser par("gate.txt");
	constructor cons(par);
	ofstream outfile("model.v");
	cons.show_all(outfile);
	cerr << "Finish generating netlist" << endl;

	Attack_parser *atk_psr = new Attack_parser("model.v");
	atk_psr->parse_circuit();

	Fault_parser flt_psr("INV+AND.txt");
	Attack_CNF_handler att_CNF(atk_psr, cons);
	att_CNF.start_attack(flt_psr);
	cerr << "Finish building final CNF" <<endl;

	ofstream out_cnf("model.cnf");
	att_CNF.print_file(out_cnf);

	SAT_solver solver("model.cnf");

	print_solution print_solu(solver, *atk_psr, cons, cerr);

	return 0;
}
