#pragma once

class ReactionNetwork {
public:
	ReactionNetwork(NetworkState initNetworkState,
									std::vector<Reaction> reactionList)
			: initNetworkState(initNetworkState), reactionList(reactionList) {}
	NetworkState initNetworkState;
	std::vector<Reaction> reactionList;
};
