#pragma once

#include "CRN/driver.h"
#include "eulerevaluator.h"
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
	pError = 4
};

class EvaluatorFrontend {
public:
	bool plot = false;
	bool print = false;
	std::string csvFilename;
	driver *drv;
	void FuncRunner(double ethreshold, double estep);
	void Help(int errorCode);

private:
	NetworkState initNetworkState;
	void Plot();
	void PrintCsv();
	void RunEulerEvaluator();
	Gnuplot gp;
	std::vector<NetworkState> states;
	double internalEtreshold;
	double internalEstep;
};