#pragma once

class Reaction {
public:
	Reaction(vector<int> reactants, vector<int> products, int reactionConstant)
			: reactants(reactants), products(products),
				reactionConstant(reactionConstant) {}
	vector<int> reactants;
	vector<int> products;
	int reactionConstant;
};
