#pragma once

#include "networkstate.h"
#include "reactionnetwork.h"
#include <vector>

class Evaluator {
public:
	Evaluator(ReactionNetwork network)
			: mNetwork(network), mState(network.initNetworkState),
				oldState(network.initNetworkState.DeepCopy()) {}
	NetworkState GetNextNetworkState();
	int delta(int n);
	std::vector<NetworkState> stateHistory;
	double threshold = 0.0001;
	// Is set to true when the change between the two last state
	// is less than threshold
	bool IsFinished() {
		return finished;
	}

protected:
	virtual NetworkState GetNextNetworkStateInternal() = 0;
	ReactionNetwork mNetwork;
	NetworkState mState;

private:
	bool finished = false;
	NetworkState oldState;
};
