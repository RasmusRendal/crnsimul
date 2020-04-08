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

//! Class that implements and approximates a solution to an ODE using the Euler method
/*! Inherits the Evaluator class. This is used to generalize evaluation between 
	the euler method and the Markov evaluator.
	*/
class EulerEvaluator : public Evaluator {
public:
	//! Constructor for EulerEvaluator
	/*! Takes a reaction network as a parameter.
		Also uses the constructor defined in the Evaluator class
		@param network a chemical reaction network */
	EulerEvaluator(const ReactionNetwork &network) : Evaluator(network) {
		InitializeEquationParts();
	}
	//! Keeping track of iterations
	/*! Used for generating t_values in combination with step.
		This is used when plotting the result and exporting the .csv file.  */
	int iterations = 0;
	void PrintEquations();
	//! Stepsize in regards to Euler Method, can be changed when given as an argument.
	double step = 0.0001;
	//! Threshold for when evaluation stops.
	double threshold = 0.0001;
	//! Inherited function from the evaluator class.
	/*! This function implements the Euler method and gets the next state of the network.
		Each state is a map with the specie name and concentration, respectively.
		@snippet{lineno} src/eulerevaluator.cpp flag 
		After each iteration the finished flag is evaluated in regards to the threshold. 
		The mState.time property represents this mentioned t_value*/
	NetworkState GetNextNetworkState() override;
<<<<<<< HEAD
=======
	//! Kahan summation function to reduce numerical error of summation of floats.
	//! @param numbers is a list of double which contain the numbers to be summed.
	//! @param c is a variable to compensate for the summation errors.
	double KahanSummation(std::initializer_list<double> numbers, double &c);
	bool kahan = true;
>>>>>>> 487b395... documentation added

private:
	void InitializeEquationParts();

	// The differential equations are stored here
	equation_set equationParts;
};
