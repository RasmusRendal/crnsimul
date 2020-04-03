#pragma once

#include "networkstate.h"
#include "reactionnetwork.h"
#include <vector>

class Evaluator {
public:
	Evaluator(const ReactionNetwork &network)
			: mNetwork(network), mState(network.initNetworkState) {}

	virtual NetworkState GetNextNetworkState() = 0;
	// Is set to true when the change between the two last state
	// is less than threshold
	bool IsFinished();

protected:
	bool finished = false;
	ReactionNetwork mNetwork;
	NetworkState mState;
};
