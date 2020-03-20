#include "reactionnetwork.h"

void ReactionNetwork::AddEmptyStates() {
	for (auto &reaction : reactionList) {
		for (auto &reactant : reaction.reactants) {
			if (initNetworkState.find(reactant.first) == initNetworkState.end()) {
				initNetworkState[reactant.first] = 0;
			}
		}

		for (auto &product : reaction.products) {
			if (initNetworkState.find(product.first) == initNetworkState.end()) {
				initNetworkState[product.first] = 0;
			}
		}
	}
}

void ReactionNetwork::Print() {
	std::cout << "Printing CRN" << std::endl;
	for (auto &conc : initNetworkState)
		std::cout << "Value of " << conc.first << " is " << conc.second << '\n';

	for (Reaction &r : reactionList) {
		for (auto &reactant : r.reactants) {
			std::cout << reactant.second << reactant.first << " ";
		}
		if (r.reactionConstant != 1) {
			std::cout << "->";
			std::cout << "(" << r.reactionConstant << ") ";
		} else {
			std::cout << "-> ";
		}

		for (auto &product : r.products) {
			std::cout << product.second << product.first << " ";
		}
		std::cout << std::endl;
	}
}
