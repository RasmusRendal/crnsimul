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

std::vector<std::string> EvaluatorFrontend::GeneratePlotString() {
	std::vector<std::string> plotStrings;
	if (!desiredChemicals.empty()) {
		for (const auto &specieName : desiredChemicals) {
			auto toPlot = initNetworkState.find(specieName);
			std::cout << toPlot->first << std::endl;
			if (toPlot == initNetworkState.end()) {
				throw std::runtime_error("Tried to plot chemical not in CRN");
			}
			plotStrings.push_back(toPlot->first);
		}
	} else {
		for (auto &species : initNetworkState) {
			plotStrings.push_back(species.first);
		}
	}
	return plotStrings;
}

void EvaluatorFrontend::PrintSTDout() {
	std::cout << initNetworkState.PrintCsvHeader();
	std::cout << initNetworkState.PrintCsvRow();
	for (auto &state : states) {
		std::cout << state.PrintCsvRow();
	}
}

void EvaluatorFrontend::Plot() {
	std::vector<std::string> plotStrings = GeneratePlotString();
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
	if (printStd)
		PrintSTDout();
}

//! @file
// [runeval]
void EvaluatorFrontend::RunEvaluator() {
	while (!evaluator->IsFinished()) {
		states.push_back(evaluator->GetNextNetworkState());
	}
}
// [runeval]

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
			"Usage: crnsimul [OPTIONS] filename\n"
			"Option types:\n"
			"Default evaluator is eulerEvaluator\n"
			"\t-m to to enable markov evaluator\n"
			"\t-p trace parsing\n"
			"\t-s trace scanning\n"
			"\t-P enable plotting in gnuplot\n"
			"\t-O <filename> output result to file, -r required\n"
			"\t-S <step size> set stepsize for eulerevaluator\n"
			"\t-T <threshold size> set the euler evaluator threshold\n"
			"\t-t to set threshold for markovEvaluator\n"
			"\t\tIf -t 0 is provided the markovEvaluator will continue forever\n"
			"\t\tand print the result as it is going to stdout\n"
			"\t\tIt can then be stopped with ctrl+c\n"
			"\t-C to select desired chemicals to plot\n"
			"\t\tThese are seperated with the commaseperator\n";
	"\t--stdout parameter prints the output to stdout\n"
	"\t\tIt is recommended to use this when markovThreshold i set to 0\n";
	"\t-h Display help options";
	std::cout << helperstring << std::endl;
}
