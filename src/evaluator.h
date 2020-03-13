#pragma once

#include "networkstate.h"
#include "reactionnetwork.h"
#include <vector>

class Evaluator {
public:
	Evaluator(ReactionNetwork network)
			: mNetwork(network), mState(network.initNetworkState) {}
	NetworkState GetNextNetworkState();
	int delta(int n);
	std::vector<NetworkState> stateHistory;

protected:
	virtual NetworkState GetNextNetworkStateInternal() = 0;
	ReactionNetwork mNetwork;
	NetworkState mState;
};
