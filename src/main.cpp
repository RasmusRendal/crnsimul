#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <iostream>
#include <string>

void PrintConcen(ReactionNetwork network) {
	std::cout << "Printing CRN" << std::endl;
	for (auto &conc : network.initNetworkState)
		std::cout << "Value of " << conc.first << " is " << conc.second << '\n';

	for (Reaction &r : network.reactionList) {
		for (auto &reactant : r.reactants) {
			std::cout << reactant.second << reactant.first << " ";
		}
		std::cout << "-> ";
		for (auto &product : r.products) {
			std::cout << product.second << product.first << " ";
		}
		std::cout << std::endl;
	}
}

int main(int argc, char *argv[]) {
	int res = 0;
	driver drv;
	bool run = false;
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string("-p")) {
			drv.trace_parsing = true;
		} else if (argv[i] == std::string("-s")) {
			drv.trace_scanning = true;

		} else if (argv[i] == std::string("-r")) {
			run = true;
		} else {
			res = drv.parse_file(argv[i]);
			if (res == 0) {
				if (run) {
					EulerEvaluator e(drv.network);
					drv.network.initNetworkState.PrintCsvHeader();
					for (int i = 0; i < 1000; i++) {
						e.GetNextNetworkState().PrintCsvRow();
					}
				} else {
					PrintConcen(drv.network);
				}
			} else {
				std::cout << "Error while parsing" << std::endl;
			}
		}
	}
	return res;
}
