#include "CRN/driver.h"
#include "eulerevaluator.h"
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
	driver drv;
	bool run = false;
	bool printCsv = false;
	bool plot = false;
	double estep = 0.00001;
	double ethreshold = 0.00001;
	std::string filename;
	std::string csvFilename;
	filename = std::string(argv[argc - 1]);
	if (!file_included(filename)) {
		std::cout << "Error: No file for parsing" << std::endl;
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string("-p")) {
			drv.trace_parsing = true;
		} else if (argv[i] == std::string("-s")) {
			drv.trace_scanning = true;
		} else if (argv[i] == std::string("-r")) {
			run = true;
		} else if (argv[i] == std::string("-P")) {
			plot = true;
		} else if (argv[i] == std::string("-O") &&
							 !std::string(argv[i + 1]).empty()) {
			printCsv = true;
			csvFilename = std::string(argv[i + 1]);
			i++;
		} else if (argv[i] == std::string("-S") && std::stod(argv[i + 1]) != 0) {
			estep = std::stod(argv[i + 1]);
			i++;
		} else if (argv[i] == std::string("-T") && std::stod(argv[i + 1]) != 0) {
			ethreshold = std::stod(argv[i + 1]);
			i++;
		} else {
			res = drv.parse_file(argv[i]);
			if (res == 0) {
				if (run) {
					EulerEvaluator e(drv.network);
					e.threshold = ethreshold;
					e.step = estep;
					std::vector<NetworkState> states;
					while (!e.IsFinished()) {
						states.push_back(e.GetNextNetworkState());
					}
					if (plot) {
						Gnuplot gp;
						std::vector<std::string> plotStrings;
						for (auto &species : drv.network.initNetworkState) {
							plotStrings.push_back(species.first);
						}
						gp << "plot";
						int plotStringsSize = static_cast<int>(plotStrings.size());
						for (int i = 0; i < plotStringsSize; i++) {
							std::string title = plotStrings[i];
							gp << " '-' with lines title '" << title << "'";
							if (i != plotStringsSize - 1)
								gp << ", ";
						}
						gp << "\n";
						for (std::string &toPlot : plotStrings) {
							std::cout.precision(17);
							for (auto &state : states) {
								gp << state.time << " " << state[toPlot] << "\n";
							}
							gp << "e";
						}
					}
					if (printCsv) {
						std::ofstream evaluatedCsv;
						evaluatedCsv.open(csvFilename);
						evaluatedCsv << drv.network.initNetworkState.PrintCsvHeader();
						evaluatedCsv << drv.network.initNetworkState.PrintCsvRow();
						for (auto &state : states) {
							evaluatedCsv << state.PrintCsvRow();
						}
						evaluatedCsv.close();
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
