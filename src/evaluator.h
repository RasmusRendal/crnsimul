#pragma once

class Evaluator {
public:
	Evaluator(ReactionNetwork network) : network(network) {}
	State GetNextState();
	int delta(int n);
	std::vector<State> stateHistory;

protected:
	virtual State GetNextStateInternal(ReactionNetwork network) = 0;
	ReactionNetwork mNetwork;
};
