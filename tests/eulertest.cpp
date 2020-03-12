#include "eulerevaluator.h"
#include <gtest/gtest.h>

#define EXPECT_CLOSE(a, b) EXPECT_LT(abs(b - a), 1)

class EulerTest : public ::testing::Test {
protected:
	virtual void SetUp() {}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(EulerTest, twoValues) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("a", 1));
	std::vector<Reaction> reactions;
	Reaction r;
	r.reactionConstant = 1;
	r.reactants.insert(std::make_pair("a", 1));
	r.products.insert(std::make_pair("b", 1));
	reactions.push_back(r);
	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	for (int i = 0; i < 10; i++) {
		evaluator.GetNextNetworkState();
	}
	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_LT(nextState["a"], 0.1);
	EXPECT_GT(nextState["b"], 0.9);
}

TEST_F(EulerTest, multiplication) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("a", 5));
	initNetworkState.insert(std::make_pair("b", 3));
	initNetworkState.insert(std::make_pair("c", 0));
	std::vector<Reaction> reactions;

	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("a", 1));
		r.reactants.insert(std::make_pair("b", 1));
		r.products.insert(std::make_pair("a", 1));
		r.products.insert(std::make_pair("b", 1));
		r.products.insert(std::make_pair("c", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("c", 1));
		reactions.push_back(r);
	}

	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["a"], 5);
	EXPECT_CLOSE(nextState["b"], 3);
	EXPECT_CLOSE(nextState["c"], 15);
}
