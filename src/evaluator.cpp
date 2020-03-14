#include "evaluator.h"

NetworkState Evaluator::GetNextNetworkState() {
	NetworkState s = GetNextNetworkStateInternal();
	finished = (oldState.Diff(s) < threshold);
	oldState = s.DeepCopy();
	return s;
}
