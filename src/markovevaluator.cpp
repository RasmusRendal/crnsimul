#include "markovevaluator.h"
#include <random>
#include <tuple>

MarkovEvaluator::MarkovEvaluator(const ReactionNetwork &network)
		: Evaluator(network) {
	CalculateProbs();
}

void MarkovEvaluator::CalculateProbs() {
	reactionProbs = std::vector<std::tuple<double, int>>();
	int index = 0;
	double probSize = 0;
	for (const auto &reaction : mNetwork.reactionList) {
		double prob = 1;
		for (const auto &reactant : reaction.reactants) {
			prob *= mState[reactant.first];
			if (mState[reactant.first] < reactant.second) {
				prob = 0;
				break;
			}
		}
		if (prob > 0.1) {
			reactionProbs.emplace_back(std::make_pair(prob, index));
			probSize += prob;
		}
		index++;
	}
	if (probSize < 0.1) {
		finished = true;
	}
	distribution = std::uniform_real_distribution<double>(0, probSize);
}

Reaction &MarkovEvaluator::GetNextReaction() {
	double reactionIndex = distribution(generator);
	const Reaction reaction;
	for (std::tuple<double, int> &reactionPair : reactionProbs) {
		reactionIndex -= std::get<0>(reactionPair);
		if (reactionIndex > 0)
			continue;

		std::poisson_distribution<int> timeDistribution(std::get<0>(reactionPair));
		currentTime += static_cast<double>(timeDistribution(generator)) / 10000;
		return mNetwork.reactionList[std::get<1>(reactionPair)];
	}
	throw std::runtime_error("Random variable outside range");
}

//! @file
// [region]
NetworkState MarkovEvaluator::GetNextNetworkState() {
	CalculateProbs();
	if (mState.time > timeThreshold && timeThreshold != 0) {
		finished = true;
	}
	if (finished) {
		return mState;
	}
	Reaction &reaction = GetNextReaction();
	mState = mState - reaction.reactants;
	mState = mState + reaction.products;
	mState.time = currentTime;
	mState.Verify();
	return mState;
}
// [region]
