#pragma once

#include "networkstate.h"
#include "reactionnetwork.h"
#include <vector>

//! Evaluator class, used as a kind of generalization class.
class Evaluator {
public:
	Evaluator(const ReactionNetwork &network)
			: mNetwork(network), mState(network.initNetworkState) {}

	virtual NetworkState GetNextNetworkState() = 0;
	// Is set to true when the change between the two last state
	// is less than threshold
	/*! Return the procted bool finished, which serves as the flag
	for when the threshold has been exceeded
	@returns finished */
	bool IsFinished();

protected:
	bool finished = false;
	ReactionNetwork mNetwork;
	NetworkState mState;
};
