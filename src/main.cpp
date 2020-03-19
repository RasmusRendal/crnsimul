#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <iostream>
#include <string>
#include <fstream>

bool file_included(const std::string &filename) {
    return std::ifstream(filename).good();
}

int main(int argc, char *argv[]) {
	int res = 0;
	driver drv;
	bool run = false;
	ReactionNetwork network;
	EulerEvaluator e(network);
	bool file = false;
	std::string filename;

	filename = std::string(argv[argc - 1]);
    if (file_included(filename) == true) {
		file = true;
	}
	if(file == false) {
	    std::cout << "Error: No file included for parsing." << std::endl;
	 	return 0; 
	}
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string("-p")) {
			drv.trace_parsing = true;
		} else if (argv[i] == std::string("-s")) {
			drv.trace_scanning = true;
		} else if (argv[i] == std::string("-r")) {
			run = true;
		} else if (argv[i] == std::string("-S") && std::atoi(argv[i + 1]) != 0){
			e.step = std::atoi(argv[i + 1]);
			std::cout << "e.step value is " << e.step << std::endl;
			i++;
		} else if (argv[i] == std::string("-T") && std::atoi(argv[i + 1]) != 0){
			e.threshold = std::atoi(argv[i + 1]);
			std::cout << "e.threshold value is " << e.threshold << std::endl;
			i++;
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
