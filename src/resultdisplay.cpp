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

void ResultDisplay::Help(int errorCode) {
	if (errorCode == fileError) {
		std::cout << "Error: No file for parsing" << std::endl;
	} else if (errorCode == rError) {
		std::cout << "Warning: Perhaps -r flag was forgotten?" << std::endl;
	} else if (errorCode == pError) {
		std::cout << "Error while parsing" << std::endl;
	}
	std::string helperstring =
			"argument list\n"
			"	-r to run evaluator (euler)\n"
			"	-p trace parsing\n"
			"	-s trace scanning\n"
			"	-P enable plotting in gnuplot\n"
			"	-O <filename> output result to file, -r required\n"
			"	-S <step size> set stepsize for eulerevaluator\n"
			"	-T <threshold size> set the euler evaluator threshold\n"
			"When using this compiler an input file is required.\n"
			"This should be passed as the last argument to the compiler.\n"
			"It is also a requirement to pass the -r argument.\n"
			"If this argument is not passed no evaluation will happen\n"
			"Example input: ./chemilang -r -P <filename>";
	std::cout << helperstring << std::endl;
}