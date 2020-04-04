#pragma once

#include "CRN/driver.h"
#include "evaluator.h"
#include "gnuplot-iostream.h"
#include "networkstate.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

enum ErrorCode {
	helpargument = 0,
	rError = 1,
	fileError = 2,
	otherError = 3,
	pError = 4,
	streamNullPtr = 5
};

class EvaluatorFrontend {
public:
	bool plot = false;
	bool print = false;
	std::string csvFilename;
	driver *drv;
	Evaluator *evaluator = nullptr;
	void FuncRunner();
	static void Help(ErrorCode errorCode = helpargument);
	void PrintCsvString();
	std::stringstream *csvStream;
	std::vector<std::string> desiredChemicals;
	std::vector<std::string> GeneratePlotString();
	NetworkState initNetworkState;
private:
	Gnuplot gp;
	std::vector<NetworkState> states;
	std::ofstream csvFileStream;
	void PrintCsv();
	void Plot();
	void RunEvaluator();
};
