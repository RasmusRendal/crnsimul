#pragma once

#include "evaluator.h"
#include <map>
#include <random>
#include <vector>

class MarkovEvaluator : public Evaluator {
public:
	MarkovEvaluator(const ReactionNetwork &network);
	double currentTime = 0;
	NetworkState GetNextNetworkState() override;

private:
	void CalculateProbs();
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
	std::vector<std::tuple<double, int>> reactionProbs;
	Reaction &GetNextReaction();
};
