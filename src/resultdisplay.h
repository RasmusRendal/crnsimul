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

class ResultDisplay {
public:
	bool plot;
	bool print;
	std::string csvFilename;
	NetworkState initNetworkState;
	void FuncRunner();
    std::vector<NetworkState> states;
private:
	void Plot();
	void PrintCsv();
	void RunEulerEvaluator();
};