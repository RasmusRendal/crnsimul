#include "eulerevaluator.h"
#include "parser/driver.h"
#include <gtest/gtest.h>
#include <limits>
#include <math.h>

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

TEST_F(EulerTest, fractionalRate) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("a", 1));
	std::vector<Reaction> reactions;
	Reaction r;
	r.reactionConstant = 1.5;
	r.reactants.insert(std::make_pair("a", 1));
	r.products.insert(std::make_pair("b", 1));
	reactions.push_back(r);
	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);
	auto state = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(state["b"], 1.5 * evaluator.step);
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

	EXPECT_EQ(state["a"], 5);
	EXPECT_EQ(state["b"], 6);
}

TEST_F(EulerTest, SanityCheck) {
	driver drv;
	int res = drv.parse_string("a:=5;\nb:=6;\n2a+3b->5x;\n4f+z->2a+3b;");
	auto &network = drv.network;
	EXPECT_EQ(res, 0);

	EulerEvaluator e(network);
	e.step = 0.0001;
	while (!e.IsFinished())
		e.GetNextNetworkState();

	NetworkState state = e.GetNextNetworkState();

	EXPECT_LT(state["x"], 11);
}

TEST_F(EulerTest, subtraction) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("x", 8));
	initNetworkState.insert(std::make_pair("y", 6));
	initNetworkState.insert(std::make_pair("z", 0));
	initNetworkState.insert(std::make_pair("c", 0));
	std::vector<Reaction> reactions;
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("z", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("c", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("z", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("z", 1));
		r.reactants.insert(std::make_pair("c", 1));
		reactions.push_back(r);
	}

	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	while (!evaluator.IsFinished())
		evaluator.GetNextNetworkState();

	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["z"], 2);
}

TEST_F(EulerTest, addition) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("x", 4));
	initNetworkState.insert(std::make_pair("y", 7));
	initNetworkState.insert(std::make_pair("z", 0));
	std::vector<Reaction> reactions;
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("z", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("z", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("z", 1));
		reactions.push_back(r);
	}
	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	while (!evaluator.IsFinished())
		evaluator.GetNextNetworkState();

	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["z"], 11);
}

TEST_F(EulerTest, division) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("x", 56));
	initNetworkState.insert(std::make_pair("y", 8));
	initNetworkState.insert(std::make_pair("z", 0));
	std::vector<Reaction> reactions;
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("z", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("y", 1));
		r.reactants.insert(std::make_pair("z", 1));
		r.products.insert(std::make_pair("y", 1));
		reactions.push_back(r);
	}
	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	while (!evaluator.IsFinished())
		evaluator.GetNextNetworkState();

	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["z"], 7);
}

TEST_F(EulerTest, nthRoot) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("n", 3));
	initNetworkState.insert(std::make_pair("a", 125));
	initNetworkState.insert(std::make_pair("x", 0));
	std::vector<Reaction> reactions;
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("a", 1));
		r.products.insert(std::make_pair("a", 1));
		r.products.insert(std::make_pair("x", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("x", 2));
		r.products.insert(std::make_pair("x", 2));
		r.products.insert(std::make_pair("y", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("x", 1));
		r.reactants.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("y", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("y", 1));
		reactions.push_back(r);
	}

	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	while (!evaluator.IsFinished())
		evaluator.GetNextNetworkState();

	auto nextState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(nextState["x"], 5);
}

TEST_F(EulerTest, comparison) {
	NetworkState initNetworkState;
	initNetworkState.insert(std::make_pair("x", 0.8));
	initNetworkState.insert(std::make_pair("y", 0.2));
	initNetworkState.insert(std::make_pair("a", 0));
	initNetworkState.insert(std::make_pair("b", 0));
	std::vector<Reaction> reactions;
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("x", 1));
		r.products.insert(std::make_pair("a", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("y", 1));
		r.products.insert(std::make_pair("b", 1));
		reactions.push_back(r);
	}
	{
		Reaction r;
		r.reactionConstant = 1;
		r.reactants.insert(std::make_pair("a", 1));
		r.reactants.insert(std::make_pair("b", 1));
		reactions.push_back(r);
	}
	ReactionNetwork network(initNetworkState, reactions);
	EulerEvaluator evaluator(network);

	while (!evaluator.IsFinished())
		evaluator.GetNextNetworkState();

	auto nextState = evaluator.GetNextNetworkState();

	EXPECT_GT(abs(nextState["a"]), 0);
}

TEST_F(EulerTest, MultipleSame) {
	driver drv;
	driver drv2;
	EXPECT_EQ(drv.parse_string("x:=144; x ->(2) x + z; 2z -> 0;"), 0);
	EXPECT_EQ(drv2.parse_string("x:=144; x ->(2) x + z; z + z -> 0;"), 0);
	EulerEvaluator e(drv.network);
	EulerEvaluator e2(drv2.network);
	while (!e.IsFinished())
		e.GetNextNetworkState();
	while (!e2.IsFinished())
		e2.GetNextNetworkState();

	EXPECT_EQ(e.GetNextNetworkState()["z"], e2.GetNextNetworkState()["z"]);
	EXPECT_CLOSE(e2.GetNextNetworkState()["z"], 12);
	EXPECT_CLOSE(e.GetNextNetworkState()["z"], 12);
}

TEST_F(EulerTest, OverflowException) {
	driver drv;
	ASSERT_EQ(drv.parse_string("a := 1000000000; a ->(100) b;"), 0);
	drv.network.initNetworkState["b"] = std::numeric_limits<double>::max();
	EulerEvaluator e(drv.network);
	e.step = INFINITY;
	EXPECT_THROW(e.GetNextNetworkState(), DoubleOverflowException);
}
