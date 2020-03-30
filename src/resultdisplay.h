#pragma once

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

class ResultDisplay {
public:
	bool plot = false;
	bool print = false;
	std::string csvFilename;
	NetworkState *initNetworkState;
	EulerEvaluator *eval;
	void FuncRunner();
	void Help(int errorCode);

private:
	void Plot();
	void PrintCsv();
	void RunEulerEvaluator();
	Gnuplot gp;
	std::vector<NetworkState> states;
};