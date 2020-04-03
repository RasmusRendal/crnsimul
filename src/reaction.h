#pragma once

#include "networkstate.h"
#include <map>

class Reaction {
public:
	Reaction(NetworkState reactants, NetworkState products, int reactionConstant)
			: reactants(std::move(reactants)), products(std::move(products)),
				reactionConstant(reactionConstant) {}
	Reaction(const std::map<std::string, int> &reactants,
					 const std::map<std::string, int> &products, int reactionConstant)
			: reactionConstant(reactionConstant) {
		for (auto &specie : reactants) {
			this->reactants[specie.first] = specie.second;
		}
		for (auto &specie : products) {
			this->products[specie.first] = specie.second;
		}
	}
	Reaction() = default;
	NetworkState reactants;
	NetworkState products;
	int reactionConstant = 1;
};
