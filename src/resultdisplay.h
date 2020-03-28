#pragma once

#include "networkstate.h"
#include <map>
#include "eulerevaluator.h"
#include "evaluator.h"
#include "gnuplot-iostream.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "reaction.h"
#include "reactionnetwork.h"

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
	NetworkState initNetworkState;
	void FuncRunner();
    void Help(int errorCode);
    std::vector<NetworkState> states;
private:
	void Plot();
	void PrintCsv();
	void RunEulerEvaluator();
    Gnuplot gp;
};