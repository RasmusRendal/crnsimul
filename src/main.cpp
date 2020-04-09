#include "CRN/driver.h"
#include "eulerevaluator.h"
#include "evaluatorfrontend.h"
#include "gnuplot-iostream.h"
#include "markovevaluator.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

bool file_included(const std::string &filename) {
	return std::ifstream(filename).good();
}

int main(int argc, char *argv[]) {
	std::stringstream *csvStream = nullptr;
	driver drv;
	EvaluatorFrontend frontEnd;
	bool evaluateEuler = true;
	double estep = 0.01;
	double ethreshold = 0.00001;
	std::string filename;

	if (argv[argc - 1] == std::string("-h") ||
			argv[argc - 1] == std::string("--help")) {
		EvaluatorFrontend::Help(helpargument);
		return 0;
	}

	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string("-p")) {
			drv.trace_parsing = true;
		} else if (argv[i] == std::string("-s")) {
			drv.trace_scanning = true;
		} else if (argv[i] == std::string("-P")) {
			frontEnd.plot = true;
		} else if (argv[i] == std::string("-O")) {
			frontEnd.print = true;
			frontEnd.csvFilename = std::string(argv[++i]);
			csvStream = new std::stringstream();
			frontEnd.csvStream = csvStream;
		} else if (argv[i] == std::string("-S")) {
			estep = std::stod(argv[++i]);
		} else if (argv[i] == std::string("-T")) {
			ethreshold = std::stod(argv[++i]);
		} else if (argv[i] == std::string("-e")) {
			evaluateEuler = true;
		} else if (argv[i] == std::string("-m")) {
			evaluateEuler = false;
		} else if (argv[i] == std::string("-C")) {
			std::stringstream chemArg(argv[++i]);
			while (chemArg.good()) {
				std::string substr;
				getline(chemArg, substr, ',');
				frontEnd.desiredChemicals.push_back(substr);
			}
		} else if (file_included(argv[i])) {
			filename = argv[i];
		} else {
			std::cout << "Unknown argument " << argv[i] << std::endl;
			EvaluatorFrontend::Help();
			return 1;
		}
	}

	if (filename.empty()) {
		std::cout << "No filename supplied" << std::endl;
		return 1;
	}

	if (estep < 0) {
		std::cout << "The step value must be more than zero" << std::endl;
		return 1;
	}

	if (ethreshold < 0) {
		std::cout << "The threshold value must be more than zero" << std::endl;
		return 1;
	}

	if (drv.parse_file(filename) == 0) {
		frontEnd.drv = &drv;
		Evaluator *evaluator;
		if (evaluateEuler) {
			auto eulerEvaluator = new EulerEvaluator(drv.network);
			eulerEvaluator->step = estep;
			eulerEvaluator->threshold = ethreshold;
			evaluator = eulerEvaluator;
		} else {
			evaluator = new MarkovEvaluator(drv.network);
		}
		frontEnd.evaluator = evaluator;
		frontEnd.FuncRunner();
	} else {
		EvaluatorFrontend::Help(pError);
		return 1;
	}

	return 0;
}

/*! \mainpage Chemilang index page
	This is the documentation for chemilang as of 
	the 8th of april 2020.
    This document contains documentation regaring
	the chemilang. \n
	For usage of this compiler please go to 
	https://github.com/RasmusRendal/Chemilang \n 
	You may also consult the readme. \n
	As a final alternative you can use the -h argument*/