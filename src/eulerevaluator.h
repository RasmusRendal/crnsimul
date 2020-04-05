#pragma once

#include "evaluator.h"
#include <map>
#include <vector>

// A term of e differential equation for a single specie
using equation_term = std::pair<int, NetworkState>;
// The differential equation for a single specie
using equation = std::vector<equation_term>;
// A set of differential equations representing a CRN
using equation_set = std::map<std::string, equation>;

class EulerEvaluator : public Evaluator {
public:
	EulerEvaluator(const ReactionNetwork &network) : Evaluator(network) {
		InitializeEquationParts();
		for (const auto &specie : network.initNetworkState) {
			errors.insert(std::make_pair(specie.first, 0));
		}
	}
	int iterations = 0;
	void PrintEquations();
	double step = 0.0001;
	double threshold = 0.0001;
	NetworkState GetNextNetworkState() override;
	double KahanSummation(std::initializer_list<double> numbers, double &c);
	bool kahan = true;

private:
	void InitializeEquationParts();
	// The differential equations are stored here
	equation_set equationParts;
	std::map<std::string, double> errors;
};
