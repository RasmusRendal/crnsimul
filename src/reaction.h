#pragma once

#include <map>

using namespace std;

class Reaction {
public:
	Reaction(map<string, int> reactants, map<string, int> products, int reactionConstant)
			: reactants(reactants), products(products),
				reactionConstant(reactionConstant) {}
	Reaction() { }
	map<string, int> reactants;
	map<string, int> products;
	int reactionConstant;
};
