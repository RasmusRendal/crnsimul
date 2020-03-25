#pragma once

#include <map>

class Reaction {
public:
	Reaction(std::map<std::string, int> reactants,
					 std::map<std::string, int> products, int reactionConstant)
			: reactants(std::move(reactants)), products(std::move(products)),
				reactionConstant(reactionConstant) {}
	Reaction() = default;
	std::map<std::string, int> reactants;
	std::map<std::string, int> products;
	int reactionConstant = 1;
};
