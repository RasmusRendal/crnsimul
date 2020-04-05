#include "CRN/driver.h"
#include "eulerevaluator.h"
#include <gtest/gtest.h>
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

TEST_F(EulerTest, SanityTest) {
	driver drv;
	EXPECT_EQ(
			drv.parse_string(
					"a := 5000; b := 3000; a + b -> c; 2a + 2b -> 2c; 3a + 3b -> 3c;"),
			0);
	EulerEvaluator e(drv.network);
	e.step = 0.01;
	for (int i = 0; i < 100; i++) {
		e.GetNextNetworkState();
		if (e.IsFinished())
			break;
	}
	auto state = e.GetNextNetworkState();
	EXPECT_FALSE(isinf(state["a"]));
	EXPECT_FALSE(isinf(state["b"]));
	EXPECT_FALSE(isinf(state["c"]));
	// Comparing two NaN's to each other will result in false on most
	// architectures
	EXPECT_DOUBLE_EQ(state["a"], state["a"]);
	EXPECT_DOUBLE_EQ(state["b"], state["b"]);
	EXPECT_DOUBLE_EQ(state["c"], state["c"]);

	// We should maybe create a limitation that disallows negative
	// concentrations. But you could also argue that we should just
	// use a clean Euler Evaluator
	EXPECT_GT(state["a"], 0);
	EXPECT_GT(state["b"], 0);
	EXPECT_GT(state["c"], 0);
	EXPECT_LT(state["c"], 3000);
	state.Print();
}

TEST_F(EulerTest, KahanTest) {
	auto crn = "a := 500; b := 300;"
						 "1a + 1b -> c;"
						 "2a + 2b -> c;";
	int iters = 100;
	double step = 0.00000000001;
	double state1;
	double state2;
	{
		driver drv;
		ASSERT_EQ(drv.parse_string(crn), 0);
		EulerEvaluator e(drv.network);
		e.step = step;
		for (int i = 0; i < iters; i++) {
			e.GetNextNetworkState();
		}

		auto state = e.GetNextNetworkState();
		state1 = state["c"];
		state.Print();
	}
	std::cout << "Now for the second test" << std::endl;
	{
		driver drv;
		ASSERT_EQ(drv.parse_string(crn), 0);
		EulerEvaluator e(drv.network);
		e.kahan = true;
		e.step = step;
		for (int i = 0; i < iters; i++) {
			e.GetNextNetworkState();
		}

		auto state = e.GetNextNetworkState();
		state2 = state["c"];
		state.Print();
	}
	EXPECT_NE(state1, state2);
}

TEST_F(EulerTest, KahanSummationEqual) {
	// ran with stepsize 0.0000001
	driver drv;
	NetworkState final;
	drv.parse_string("a:=600; b:=750; a + b -> a + b + c; c -> 0;");
	double expected = 1797.3017995500002;
	EulerEvaluator e(drv.network);
	e.step = 0.001;
	for (int i = 0; i < 4; i++) {
		final = e.GetNextNetworkState();
	}
	double actual = final.find("c")->second;
	EXPECT_DOUBLE_EQ(expected, actual);
}
