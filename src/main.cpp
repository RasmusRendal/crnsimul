#include "CRN/driver.h"
#include "reaction.h"
#include <iostream>
#include <string>

void PrintConcen(driver drv) {
	std::cout << "Printing CRN" << std::endl;
	for (auto &conc : drv.Concentration)
		std::cout << "Value of " << conc.first << " is " << conc.second << '\n';

	for (Reaction &r : drv.reactions) {
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
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string("-p")) {
			drv.trace_parsing = true;
		} else if (argv[i] == std::string("-s")) {
			drv.trace_scanning = true;
		} else {
			res = drv.parse_file(argv[i]);
			if (res == 0) {
				PrintConcen(drv);
			} else {
				std::cout << "Error while parsing" << std::endl;
			}
		}
	}
	return res;
}
