#include "markovevaluator.h"
#include "parser/driver.h"
#include <gtest/gtest.h>

#define EXPECT_CLOSE(a, b) EXPECT_LT(abs((b) - (a)), 1)

class MarkovTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(MarkovTest, twoValues) {
	NetworkState initNetworkState;
	int startValue = 50;
	initNetworkState.insert(std::make_pair("a", startValue));
	std::vector<Reaction> reactions;
	Reaction r;
	r.reactionConstant = 1;
	r.reactants.insert(std::make_pair("a", 1));
	r.products.insert(std::make_pair("b", 1));
	reactions.push_back(r);
	ReactionNetwork network(initNetworkState, reactions);
	MarkovEvaluator evaluator(network);
	evaluator.timeThreshold = 0;
	double oldTime = 0;
	for (int i = 0; i < startValue * 2; i++) {
		auto nextState = evaluator.GetNextNetworkState();
		if (evaluator.IsFinished())
			break;

		EXPECT_GT(nextState.time, oldTime);
		oldTime = nextState.time;
		EXPECT_CLOSE(nextState["a"], startValue - 1 - i);
		EXPECT_CLOSE(nextState["b"], i + 1);
	}

	auto finalState = evaluator.GetNextNetworkState();
	EXPECT_CLOSE(finalState["a"], 0);
	EXPECT_CLOSE(finalState["b"], startValue);
	EXPECT_EQ(evaluator.IsFinished(), true);
}

TEST_F(MarkovTest, threshold) {
	double finalTime;
	driver drv;
	std::vector<NetworkState> states;
	drv.parse_string("a := 10; b := 15; a -> a + b; b -> 0;");
	MarkovEvaluator markov(drv.network);
	markov.timeThreshold = 5.0;
	while (!markov.IsFinished()) {
		states.push_back(markov.GetNextNetworkState());
	}
	finalTime = states.back().time;
	ASSERT_TRUE(finalTime >= 5 && finalTime < 5.1);
}
