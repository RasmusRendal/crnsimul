#pragma once

#include "networkstate.h"
#include "reaction.h"
#include <vector>

//! Class to encapsulate a reaction network
/*! A reaction network is, as the name applies, a network of reactions. */
class ReactionNetwork {
public:
	ReactionNetwork(NetworkState initNetworkState,
									std::vector<Reaction> reactionList)
			: initNetworkState(std::move(initNetworkState)),
				reactionList(std::move(reactionList)) {}
	ReactionNetwork() = default;
	NetworkState initNetworkState;
	std::vector<Reaction> reactionList;
	void AddEmptyStates();
	void Print();
};
