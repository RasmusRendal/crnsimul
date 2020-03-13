#include "evaluator.h"

NetworkState Evaluator::GetNextNetworkState() {
	NetworkState s = GetNextNetworkStateInternal();
	return s;
}
