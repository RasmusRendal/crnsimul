#include "CRN/driver.h"
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

	for (int i = 0; i < 100000; i++) {
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
	evaluator.PrintEquations();

	for (int i = 0; i < 100000; i++)
		evaluator.GetNextNetworkState();

	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["a"], 5);
	EXPECT_CLOSE(nextState["b"], 3);
	EXPECT_CLOSE(nextState["c"], 15);
}

TEST_F(EulerTest, NoChange) {
	driver drv;
	int res = drv.parse_string("a:=5;\nb:=6;\n2c+3b->5x;\n4f+z->2a+3b;");
	auto &network = drv.network;
	EXPECT_EQ(res, 0);

	EulerEvaluator e(network);
	NetworkState state = e.GetNextNetworkState();

	EXPECT_CLOSE(state["a"], 5);
	EXPECT_CLOSE(state["b"], 6);
}

TEST_F(EulerTest, SanityCheck) {
	driver drv;
	int res = drv.parse_string("a:=5;\nb:=6;\n2a+3b->5x;\n4f+z->2a+3b;");
	auto &network = drv.network;
	EXPECT_EQ(res, 0);

	EulerEvaluator e(network);
	for (int i = 0; i < 100000; i++)
		e.GetNextNetworkState();

	NetworkState state = e.GetNextNetworkState();

	EXPECT_LT(state["x"], 11);
}
