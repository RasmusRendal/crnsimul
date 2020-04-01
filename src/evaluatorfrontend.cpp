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
	RunEulerEvaluator();
	if (print) {
		if (csvStream != nullptr)
			PrintCsv();
		else
			Help(streamNullPtr);
	}
	if (plot)
		Plot();
}

void EvaluatorFrontend::RunEulerEvaluator() {
	while (!eulerEval->IsFinished()) {
		states.push_back(eulerEval->GetNextNetworkState());
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
			"argument list\n"
			"	-e to to enable eulerevaluator\n"
			"	-p trace parsing\n"
			"	-s trace scanning\n"
			"	-P enable plotting in gnuplot\n"
			"	-O <filename> output result to file, -r required\n"
			"	-S <step size> set stepsize for eulerevaluator\n"
			"	-T <threshold size> set the euler evaluator threshold\n"
			"When using this compiler an input file is required.\n"
			"This should be passed as the last argument to the compiler.\n"
			"At this moment -e is required.\n"
			"If this argument is not passed no evaluation will happen\n"
			"Example input: ./chemilang -e -P <filename>";
	std::cout << helperstring << std::endl;
}
