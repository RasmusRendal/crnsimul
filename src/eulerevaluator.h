#pragma once

#include "evaluator.h"
#include <map>
#include <vector>

using namespace std;

// A term of e differential equation for a single specie
using equation_term = pair<int, map<string, int>>;
// The differential equation for a single specie
using equation = vector<equation_term>;
// A set of differential equations representing a CRN
using equation_set = map<string, equation>;

class EulerEvaluator : public Evaluator {
public:
	EulerEvaluator(ReactionNetwork network) : Evaluator(network) {
		InitializeEquationParts();
	}
	void PrintEquations();

private:
	void InitializeEquationParts();
	double step = 0.0001;

	NetworkState GetNextNetworkStateInternal() override;
	// The differential equations are stored here
	equation_set equationParts;
};
