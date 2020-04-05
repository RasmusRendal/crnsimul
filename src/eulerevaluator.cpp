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

NetworkState EulerEvaluator::GetNextNetworkState() {
	iterations++;
	auto oldState = mState.DeepCopy();
	double c1 = 0;
	double c2 = 0;
	for (auto &specie : equationParts) {
		double diff = 0;
		for (equation_term &term : specie.second) {
			double change = term.first;
			for (auto &reactant : term.second) {
				change *= pow(oldState[reactant.first], reactant.second);
			}
			diff = KahanSummation({diff, change}, c1);
		}
		mState[specie.first] = KahanSummation({mState[specie.first], diff * step}, c2);
	}
	mState.time = step * iterations;
	finished = (oldState.Diff(mState) < threshold);
	return mState;
}

double EulerEvaluator::KahanSummation(std::initializer_list<double> numbers, double &c) {
	double sum = 0.0;
	for (auto number : numbers) {
		double y = number - c;
		double t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}
	return sum;
}

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
