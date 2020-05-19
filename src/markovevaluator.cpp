#include "markovevaluator.h"
#include <random>
#include <tuple>

MarkovEvaluator::MarkovEvaluator(const ReactionNetwork &network)
		: Evaluator(network) {
	CalculateFiringRate();
}

void MarkovEvaluator::CalculateFiringRate() {
	firingRates = std::vector<std::tuple<double, int>>();
	int index = 0;
	accFiringRate = 0;
	for (const auto &reaction : mNetwork.reactionList) {
		double firingRate = 1;
		for (const auto &reactant : reaction.reactants) {
			const auto &specie = reactant.first;
			const auto &stoichiometry = reactant.second;
			firingRate *= mState[specie];
			if (mState[specie] < stoichiometry) {
				firingRate = 0;
				break;
			}
		}
		if (firingRate > 0.1) {
			firingRates.emplace_back(std::make_pair(firingRate, index));
			accFiringRate += firingRate;
		}
		index++;
	}
	if (accFiringRate < 0.1) {
		finished = true;
	}
	distribution = std::uniform_real_distribution<double>(0, accFiringRate);
}

Reaction &MarkovEvaluator::GetNextReaction() {
	double reactionIndex = distribution(generator);
	for (std::tuple<double, int> &reactionPair : firingRates) {
		reactionIndex -= std::get<0>(reactionPair);
		if (reactionIndex > 0)
			continue;

		return mNetwork.reactionList[std::get<1>(reactionPair)];
	}
	// Staves off compiler warnings
	throw std::runtime_error(
			"Random variable outside range. This should never happen.");
}

//! @file
// [region]
NetworkState MarkovEvaluator::GetNextNetworkState() {
	CalculateFiringRate();
	if (mState.time > timeThreshold && timeThreshold != 0) {
		finished = true;
	}
	if (finished) {
		return mState;
	}
	Reaction &reaction = GetNextReaction();
	NetworkState newState = mState.DeepCopy();
	newState = newState - reaction.reactants;
	newState = newState + reaction.products;

	std::poisson_distribution<int> timeDistribution(accFiringRate);
	currentTime += static_cast<double>(timeDistribution(generator));

	newState.time = currentTime;
	newState.Verify();
	mState = newState;
	return newState;
}
// [region]
