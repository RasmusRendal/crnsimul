#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "gnuplot-iostream.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <fstream>
#include <iostream>
#include <string>

enum ErrorCode {
	helpargument = 0,
	rError = 1,
	fileError = 2,
	otherError = 3,
	pError = 4
};

bool file_included(const std::string &filename) {
	return std::ifstream(filename).good();
}

void help(int errorCode) {
	if (errorCode == fileError) {
		std::cout << "Error: No file for parsing" << std::endl;
	} else if (errorCode == rError) {
		std::cout << "Warning: Perhaps -r flag was forgotten?" << std::endl;
	} else if (errorCode == pError) {
		std::cout << "Error while parsing" << std::endl;
	}
	string helperstring =
			"argument list\n"
			"-r to run evaluator (euler)\n"
			"-p trace parsing\n"
			"-s trace scanning\n"
			"-P enable plotting in gnuplot\n"
			"-O <filename> output result to file, -r required\n"
			"-S <step size> set stepsize for eulerevaluator\n"
			"-T <threshold size> set the euler evaluator threshold\n"
			"When using this compiler an input file is required.\n"
			"This should be passed as the last argument to the compiler.\n"
			"It is also a requirement to pass the -r argument.\n"
			"If this argument is not passed no evaluation will happen\n";
	std::cout << helperstring << std::endl;
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
	if (argv[argc - 1] == std::string("-h") ||
			argv[argc - 1] == std::string("--help")) {
		help(helpargument);
		return 0;
	}
	if (!file_included(filename)) {
		help(fileError);
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
		} else if (argv[i] == std::string("-O") && std::string(argv[i + 1]) != "") {
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
							cout.precision(17);
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
					help(rError);
				}
			} else {
				help(pError);
			}
		}
	}
	return res;
}
