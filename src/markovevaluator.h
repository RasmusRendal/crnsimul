#pragma once

#include "evaluator.h"
#include <map>
#include <random>
#include <vector>

//! Class that implements and solves ODE using markov chains
class MarkovEvaluator : public Evaluator {
public:
	//! Constructor for markov evaluator.
	/*! The construtor for the markov evaluator
	takes the CRN to be evaluated as input
	@param network a chemical reaction network */
	MarkovEvaluator(const ReactionNetwork &network);
	//! currentTime is used for keeping track of the result
	double currentTime = 0;
	/*! Inherited function from the evaluator class
	@snippet{lineno} src/markovevaluator.cpp region
	Needs description*/
	NetworkState GetNextNetworkState() override;
	double timeThreshold;

private:
	void CalculateProbs();
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
	std::vector<std::tuple<double, int>> reactionProbs;
	Reaction &GetNextReaction();
};
