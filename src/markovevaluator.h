#pragma once

#include "evaluator.h"
#include <map>
#include <random>
#include <vector>

//! Brief desc.
/*! long desc. */
class MarkovEvaluator : public Evaluator {
public:
	//! Constructor for markov evaluator.
	/*! The construtor for the markov evaluator 
	takes the CRN to be evaluated as input
	@param network a chemical reaction network */
	MarkovEvaluator(const ReactionNetwork &network);
	//! currentTime is used for keeping track of the result
	double currentTime = 0;
	/*! Inherited function from the evaluator class*/
	NetworkState GetNextNetworkState() override;

private:
	void CalculateProbs();
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
	std::vector<std::tuple<double, int>> reactionProbs;
	Reaction &GetNextReaction();
};
