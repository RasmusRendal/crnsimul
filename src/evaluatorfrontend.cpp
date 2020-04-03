#include "evaluatorfrontend.h"

void EvaluatorFrontend::PrintCsvString() {
	*csvStream << initNetworkState.PrintCsvHeader();
	*csvStream << initNetworkState.PrintCsvRow();
	for (auto &state : states) {
		*csvStream << state.PrintCsvRow();
	}
}

void EvaluatorFrontend::PrintCsv() {
	PrintCsvString();
	csvFileStream.open(csvFilename);
	csvFileStream << csvStream->rdbuf();
	csvFileStream.close();
	std::cout << "Printed to file " << csvFilename << std::endl;
}

void EvaluatorFrontend::Plot() {
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

void EvaluatorFrontend::FuncRunner() {
	initNetworkState = drv->network.initNetworkState;
	RunEvaluator();
	if (print) {
		if (csvStream != nullptr)
			PrintCsv();
		else
			Help(streamNullPtr);
	}
	if (plot)
		Plot();
}

void EvaluatorFrontend::RunEvaluator() {
	while (!evaluator->IsFinished()) {
		states.push_back(evaluator->GetNextNetworkState());
	}
}

void EvaluatorFrontend::Help(ErrorCode errorCode) {
	if (errorCode == fileError) {
		std::cout << "Error: No file for parsing" << std::endl;
	} else if (errorCode == rError) {
		std::cout << "Warning: Perhaps -r flag was forgotten?" << std::endl;
	} else if (errorCode == pError) {
		std::cout << "Error while parsing" << std::endl;
	} else if (errorCode == streamNullPtr)
		std::cout << "filestreampointer was nullptr. Please try again" << std::endl;
	std::string helperstring =
			"Usage: chemilang [OPTIONS] filename"
			"Option types:\n"
			"	-e to to enable eulerevaluator (Required)\n"
			"	-p trace parsing\n"
			"	-s trace scanning\n"
			"	-P enable plotting in gnuplot\n"
			"	-O <filename> output result to file, -r required\n"
			"	-S <step size> set stepsize for eulerevaluator\n"
			"	-T <threshold size> set the euler evaluator threshold\n"
			"   -h Display help options";
	std::cout << helperstring << std::endl;
}
