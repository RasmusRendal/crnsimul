#pragma once

#include "networkstate.h"
#include "reaction.h"
#include <vector>

class ReactionNetwork {
public:
	ReactionNetwork(NetworkState initNetworkState,
									std::vector<Reaction> reactionList)
			: initNetworkState(initNetworkState), reactionList(reactionList) {}
	ReactionNetwork() {}
	NetworkState initNetworkState;
	std::vector<Reaction> reactionList;
	void AddEmptyStates() {
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
};
