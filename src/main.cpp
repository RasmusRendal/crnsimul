#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "gnuplot-iostream.h"
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
		} else if (argv[i] == std::string("-S") && std::atoi(argv[++i]) != 0){
			EulerEvaluator::step = argv[i + 1];
			i++;
		} else if (argv[i] == std::string("-T") && std::atoi(argv[++i]) != 0){
			EulerEvaluator::threshold = argv[i + 1];
			i++;
		} else {
			res = drv.parse_file(argv[i]);
			if (res == 0) {
				if (run) {
					EulerEvaluator e(drv.network);
					e.threshold = 0.000001;
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
