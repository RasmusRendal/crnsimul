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
	void AddEmptyStates();
	void Print();
};
