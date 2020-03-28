#include "resultdisplay.h"

void ResultDisplay::PrintCsv() {
	std::ofstream evaluatedCsv;
	evaluatedCsv.open(csvFilename);
	evaluatedCsv << initNetworkState.PrintCsvHeader();
	evaluatedCsv << initNetworkState.PrintCsvRow();
	for (auto &state : states) {
		evaluatedCsv << state.PrintCsvRow();
	}
	evaluatedCsv.close();
	std::cout << "Printed to file " << csvFilename << std::endl;
}

void ResultDisplay::Plot() {
	std::vector<std::string> plotStrings;
	for (auto &species : initNetworkState) {
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

void ResultDisplay::FuncRunner() {
	if(print)
		PrintCsv();
	else if(plot)
		Plot();
}

void ResultDisplay::RunEulerEvaluator() {
	/*while (!eval.IsFinished()) {
		states.push_back(eval.GetNextNetworkState());
	} */
} 