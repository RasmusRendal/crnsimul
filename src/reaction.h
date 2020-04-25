#pragma once

#include "networkstate.h"
#include <map>

//! Class for encapsulating reactions
class Reaction {
public:
	// where is this used in relation to the other constructor?
	//! Constructor 1
	Reaction(NetworkState reactants, NetworkState products, int reactionConstant)
			: reactants(std::move(reactants)), products(std::move(products)),
				reactionConstant(reactionConstant) {}
	//! Constructor 2
	Reaction(const std::map<std::string, int> &reactants,
					 const std::map<std::string, int> &products, double reactionConstant)
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
	double reactionConstant = 1;
};
