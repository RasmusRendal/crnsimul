#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <iostream>
#include <string>

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
					e.threshold = 0.000001;
					while (!e.IsFinished()) {
						e.GetNextNetworkState().PrintCsvRow();
					}
				} else {
					drv.network.Print();
				}
			} else {
				std::cout << "Error while parsing" << std::endl;
			}
		}
	}
	return res;
}
