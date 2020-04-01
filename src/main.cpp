#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "evaluatorfrontend.h"
#include "gnuplot-iostream.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <fstream>
#include <iostream>
#include <string>

bool file_included(const std::string &filename) {
	return std::ifstream(filename).good();
}

int main(int argc, char *argv[]) {
	int res = 0;
	std::stringstream *csvStream = nullptr;
	driver drv;
	EvaluatorFrontend frontEnd;
	bool evaluateEuler = false;
	double estep = 0.00001;
	double ethreshold = 0.00001;
	std::string filename;
	filename = std::string(argv[argc - 1]);
	if (argv[argc - 1] == std::string("-h") ||
			argv[argc - 1] == std::string("--help")) {
		frontEnd.Help(helpargument);
		return 0;
	}
	if (!file_included(filename)) {
		frontEnd.Help(fileError);
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string("-p")) {
			drv.trace_parsing = true;
		} else if (argv[i] == std::string("-s")) {
			drv.trace_scanning = true;
		} else if (argv[i] == std::string("-P")) {
			frontEnd.plot = true;
		} else if (argv[i] == std::string("-O") &&
							 !std::string(argv[i + 1]).empty()) {
			frontEnd.print = true;
			frontEnd.csvFilename = std::string(argv[i + 1]);
			csvStream = new std::stringstream();
			frontEnd.csvStream = csvStream;
			i++;
		} else if (argv[i] == std::string("-S") && std::stod(argv[i + 1]) != 0) {
			estep = std::stod(argv[i + 1]);
			i++;
		} else if (argv[i] == std::string("-T") && std::stod(argv[i + 1]) != 0) {
			ethreshold = std::stod(argv[i + 1]);
			i++;
		} else if (argv[i] == std::string("-e")) {
			evaluateEuler = true;
		} else {
			res = drv.parse_file(argv[i]);
			if (res == 0) {
				if (evaluateEuler) {
					frontEnd.drv = &drv;
					EulerEvaluator e(drv.network);
					e.step = estep;
					e.threshold = ethreshold;
					frontEnd.eulerEval = &e;
					frontEnd.FuncRunner();
				}
			} else {
				frontEnd.Help(pError);
			}
		}
	}
	return res;
}
