#include "evaluator.h"

State Evaluator::GetNextState() {
	State s = GetNextStateInternal(mNetwork);
}
