#include "eulerevaluator.h"
#include <cmath>
#include <iostream>
#include <map>
#include <utility>

void AddIfNotExists(std::map<std::string, int> &map, const std::string &toAdd) {
	if (map.find(toAdd) == map.end()) {
		map.insert(std::make_pair(toAdd, 0));
	}
}

void EulerEvaluator::InitializeEquationParts() {
	for (auto &reaction : mNetwork.reactionList) {
		// For each reaction occurence, this map contains the net change
		// for each species
		std::map<std::string, int> changePerReaction;

		for (auto &reactant : reaction.reactants) {
			AddIfNotExists(changePerReaction, reactant.first);
			changePerReaction[reactant.first] -= reactant.second;
		}
		for (auto &product : reaction.products) {
			AddIfNotExists(changePerReaction, product.first);
			changePerReaction[product.first] += product.second;
		}

		for (auto &specie : changePerReaction) {
			if (specie.second == 0)
				continue;

			if (equationParts.find(specie.first) == equationParts.end())
				equationParts.insert(make_pair(specie.first, equation()));

			equationParts[specie.first].push_back(make_pair(
					specie.second * reaction.reactionConstant, reaction.reactants));
		}
	}
}

NetworkState EulerEvaluator::calculateDiff(const NetworkState &delta) const {
	NetworkState res = NetworkState();
	for (const auto &specie : equationParts) {
		for (const equation_term &term : specie.second) {
			double change = term.first;
			for (const auto &reactant : term.second) {
				change *= pow(mState.get(reactant.first) + delta.get(reactant.first),
											reactant.second);
			}
			res[specie.first] += change;
		}
	}
	return res;
}

//! \file
// [flag]
NetworkState EulerEvaluator::GetNextNetworkState() {
	iterations++;
	NetworkState k1 = calculateDiff(NetworkState());
	NetworkState k2 = calculateDiff((k1) * (step / 2));
	NetworkState k3 = calculateDiff((k2) * (step / 2));
	NetworkState k4 = calculateDiff(k3 * step);
	NetworkState oldState = NetworkState(mState);
	auto totalChange = (k1 + k2 * 2 + k3 * 2 + k4) * (step / 6);
	mState = mState + totalChange;
	mState.time = step * iterations;
	finished = (mState.Diff(oldState) < threshold);
	mState.Verify();
	return mState;
}
// [flag]

void EulerEvaluator::PrintEquations() {
	for (const auto &equation : equationParts) {
		std::cout << equation.first << " = ";
		for (auto &term : equation.second) {
			std::cout << term.first << "*";
			for (auto &specie : term.second) {
				if (specie.second != 1)
					std::cout << specie.second;
				std::cout << specie.first;
			}
			std::cout << " + ";
		}
		std::cout << std::endl;
	}
}
