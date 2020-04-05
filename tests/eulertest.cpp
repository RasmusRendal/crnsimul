#include "CRN/driver.h"
#include "eulerevaluator.h"
#include <gtest/gtest.h>

#define EXPECT_CLOSE(a, b) EXPECT_LT(abs((b) - (a)), 1)

class EulerTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {
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

	while (!evaluator.IsFinished()) {
		evaluator.GetNextNetworkState();
	}
	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["a"], 0);
	EXPECT_CLOSE(nextState["b"], 1);
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

	while (!evaluator.IsFinished())
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
	while (!e.IsFinished())
		e.GetNextNetworkState();

	NetworkState state = e.GetNextNetworkState();

	EXPECT_LT(state["x"], 11);
}

TEST_F(EulerTest, MultipleSame) {
	driver drv;
	driver drv2;
	// This should be the square root function. However, due to the
	// reaction constant not being included, it's actually sqrt(x/2)
	// instead of sqrt(x)
	EXPECT_EQ(drv.parse_string("x:=128; x -> x + z; 2z    -> 0;"), 0);
	EXPECT_EQ(drv2.parse_string("x:=128; x -> x + z; z + z -> 0;"), 0);

	EulerEvaluator e(drv.network);
	EulerEvaluator e2(drv2.network);
	while (!e.IsFinished())
		e.GetNextNetworkState();
	while (!e2.IsFinished())
		e2.GetNextNetworkState();

	EXPECT_CLOSE(e.GetNextNetworkState()["z"], e2.GetNextNetworkState()["z"]);
	EXPECT_CLOSE(e2.GetNextNetworkState()["z"], 8);
	EXPECT_CLOSE(e.GetNextNetworkState()["z"], 8);
}

TEST_F(EulerTest, KahanSummationEqual) {
	driver drv;
	double c = 0;
	double a = 0.0000000000005;
	double b = 0.0000000000006;
	double expected = 0.0000000000011;
	EulerEvaluator e(drv.network);
	EXPECT_EQ(expected, e.KahanSummation({a,b}, c));
}

TEST_F(EulerTest, KahanSummationNotEqual) {
	driver drv;
	double c = 0;
	EulerEvaluator e (drv.network);
	double a = 0.0000000000005;
	double b = 0.0000000000006;
	double expected = 0.000000000011;
	EXPECT_NE(expected, e.KahanSummation({a,b},c));
}