#pragma once

#include "OpenRTPlotter.h"
#include "evaluator.h"
#include "gnuplot-iostream.h"
#include "networkstate.h"
#include "parser/driver.h"
#include "reaction.h"
#include "reactionnetwork.h"
#include <chrono>
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

//! Frontend class for plotting and exporting the results from evaluation.
class EvaluatorFrontend {
public:
	bool plot = false;
	bool rtPlot = false;
	bool print = false;
	bool printStd = false;
	std::string csvFilename;
	driver *drv;
	Evaluator *evaluator = nullptr;
	/*! This function is responsible for calling all the member functions
	of this class. It evalutes different booleans as to whether or not these
	should be called. It also calls both the EulerEvaluator and
	the MarkovEvaluator */
	void FuncRunner();
	static void Help(ErrorCode errorCode = helpargument);
	void PrintCsvString();
	std::stringstream *csvStream;
	std::vector<std::string> desiredChemicals;
	std::vector<std::string> GeneratePlotString();
	NetworkState initNetworkState;

private:
	/*! Gnu plot was our plotting software of choice
	 It is fed an input stream which it then plots,
	 nothing exciting here*/
	Gnuplot gp;
	std::vector<NetworkState> states;
	std::ofstream csvFileStream;

	std::vector<OpenRTP::Plot> ToPlot;
	OpenRTP::Plotter *mPlot;
	int LastInsertedCount = 1;
	int UpdateRate = 16;

	void PrintCsv();
	void Plot();
	void RTPlotInit();
	/*! This function runs the evaluator. Since both EulerEvaluator
	and MarkovEvaluator inherits from Evaluator the call to perform
	evaluation is as simple as follows
	@snippet{lineno} src/evaluatorfrontend.cpp runeval
	The result is stored in the vector states, which can then
	later be accessed by other functions */
	void RunEvaluator();
	void RunRTEvaluator();
	int EvaluatorFunc();
};
