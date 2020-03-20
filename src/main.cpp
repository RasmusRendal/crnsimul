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
	double estep = 0.00001;
	double ethreshold = 0.00001;
	std::string filename;
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
					drv.network.initNetworkState.PrintCsvHeader();
					e.threshold = ethreshold;
					e.step = estep;
					std::vector<NetworkState> states;
					while (!e.IsFinished()) {
						states.push_back(e.GetNextNetworkState());
					}
					Gnuplot gp;
					std::vector<std::string> plotStrings;
					for (auto &species : drv.network.initNetworkState) {
						plotStrings.push_back(species.first);
					}
					gp << "plot";
					for (int i = 0; i < plotStrings.size(); i++) {
						std::string title = plotStrings[i];
						gp << " '-' with lines title '" << title << "'";
						if (i != plotStrings.size() - 1)
							gp << ", ";
					}
					gp << "\n";
					for (std::string &toPlot : plotStrings) {
						int i = 0;
						cout.precision(17);
						for (auto &state : states) {
							gp << i << " " << state[toPlot] << "\n";
							i++;
						}
						// gp.sendBinary1d(plotData);
						gp << "e";
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
