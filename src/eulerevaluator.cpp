#include "eulerevaluator.h"
#include <iostream>
#include <map>
#include <utility>

void AddIfNotExists(map<string, int> &map, string toAdd) {
	if (map.find(toAdd) == map.end()) {
		map.insert(make_pair(toAdd, 0));
	}
}

void EulerEvaluator::InitializeEquationParts() {
	for (auto &reaction : mNetwork.reactionList) {
		map<string, int> &changeRate = reaction.reactants;

		// For each reaction occurence, this map contains the net change
		// for each species
		std::map<string, int> changePerReaction;

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

			equationParts[specie.first].push_back(
					make_pair(specie.second * reaction.reactionConstant, changeRate));
		}
	}
}

NetworkState EulerEvaluator::GetNextNetworkStateInternal() {
	auto oldState = mState.DeepCopy();
	for (auto &specie : equationParts) {
		double diff = 0;
		for (equation_term &term : specie.second) {
			double change = term.first;
			for (auto &reactant : term.second) {
				change *= oldState[reactant.first];
			}
			diff += change;
		}
		mState[specie.first] += diff * step;
	}
	return mState;
}

void EulerEvaluator::PrintEquations() {
	for (auto equation : equationParts) {
		std::cout << equation.first << " = ";
		for (auto &term : equation.second) {
			std::cout << term.first << "*";
			for (auto &specie : term.second) {
				std::cout << specie.second << specie.first;
			}
			std::cout << " + ";
		}
		std::cout << std::endl;
	}
}
