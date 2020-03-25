#pragma once

#include "evaluator.h"
#include <map>
#include <vector>

// A term of e differential equation for a single specie
using equation_term = std::pair<int, std::map<std::string, int>>;
// The differential equation for a single specie
using equation = std::vector<equation_term>;
// A set of differential equations representing a CRN
using equation_set = std::map<std::string, equation>;

class EulerEvaluator : public Evaluator {
public:
	EulerEvaluator(ReactionNetwork network) : Evaluator(std::move(network)) {
		InitializeEquationParts();
	}
	int iterations = 0;
	void PrintEquations();
	double step = 0.0001;

private:
	void InitializeEquationParts();

	NetworkState GetNextNetworkStateInternal() override;
	// The differential equations are stored here
	equation_set equationParts;
};
