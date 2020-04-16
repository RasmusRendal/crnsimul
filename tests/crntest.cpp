#include "parser/driver.h"
#include "reaction.h"
#include <gtest/gtest.h>

class CrnTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CrnTest, parseSingle) {
	driver drv;
	drv.parse_string("a:=3;a->b;");

	auto &network = drv.network;

	EXPECT_EQ(network.initNetworkState["a"], 3);

	EXPECT_EQ(network.reactionList.size(), 1);
	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);
}

TEST_F(CrnTest, parseTwo) {
	driver drv;

	// Concentrations
	drv.parse_string("a:=3;a->b;b->a;");
	auto &network = drv.network;
	EXPECT_EQ(network.initNetworkState["a"], 3);

	// Reactions
	EXPECT_EQ(network.reactionList.size(), 2);

	// Reaction 1
	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);

	// Reaction 2
	auto reaction2 = network.reactionList[1];
	EXPECT_EQ(reaction2.reactants["b"], 1);
	EXPECT_EQ(reaction2.products["a"], 1);
}

TEST_F(CrnTest, parseZero) {
	driver drv;
	drv.parse_string("a:=3; a->b; b->0;");
	auto &network = drv.network;

	EXPECT_EQ(network.initNetworkState["a"], 3);

	EXPECT_EQ(network.reactionList.size(), 2);

	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);

	auto reaction2 = network.reactionList[1];
	EXPECT_EQ(reaction2.reactants["b"], 1);
	EXPECT_EQ(reaction2.products.size(), 0);
}

TEST_F(CrnTest, doesNotParseINvalid) {
	driver drv;
	ASSERT_ANY_THROW(drv.parse_string("a:=3; a->b; b->7;"));
}

TEST_F(CrnTest, goNuts) {
	driver drv;
	drv.parse_string("a:=3; b:=5; a->b; b->0; c->77a;");
	auto &network = drv.network;

	EXPECT_EQ(network.initNetworkState["a"], 3);
	EXPECT_EQ(network.initNetworkState["b"], 5);

	EXPECT_EQ(network.reactionList.size(), 3);

	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants.size(), 1);
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products.size(), 1);
	EXPECT_EQ(reaction1.products["b"], 1);

	auto reaction2 = network.reactionList[1];
	EXPECT_EQ(reaction2.reactants.size(), 1);
	EXPECT_EQ(reaction2.reactants["b"], 1);
	EXPECT_EQ(reaction2.products.size(), 0);

	auto reaction3 = network.reactionList[2];
	EXPECT_EQ(reaction3.reactants.size(), 1);
	EXPECT_EQ(reaction3.reactants["c"], 1);
	EXPECT_EQ(reaction3.products.size(), 1);
	EXPECT_EQ(reaction3.products["a"], 77);
}

TEST_F(CrnTest, multipleSpecies) {
	driver drv;
	int res = drv.parse_string("a:=5; b:=6; 2a+3b->5x; 4f+z->2a+3b;");
	auto &network = drv.network;
	EXPECT_EQ(res, 0);

	// EXPECT_EQ(network.initNetworkState.size(), 2);
	EXPECT_EQ(network.initNetworkState["a"], 5);
	EXPECT_EQ(network.initNetworkState["b"], 6);

	EXPECT_EQ(network.reactionList.size(), 2);
	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants.size(), 2);
	EXPECT_EQ(reaction1.products.size(), 1);

	auto reaction2 = network.reactionList[1];
	EXPECT_EQ(reaction2.reactants.size(), 2);
	EXPECT_EQ(reaction2.products.size(), 2);
}

TEST_F(CrnTest, withNewlines) {
	driver drv;
	int res = drv.parse_string("a:=5;\nb:=6;\n2a+3b->5x;\n4f+z->2a+3b;");
	auto &network = drv.network;
	EXPECT_EQ(res, 0);

	// EXPECT_EQ(network.initNetworkState.size(), 2);
	EXPECT_EQ(network.initNetworkState["a"], 5);
	EXPECT_EQ(network.initNetworkState["b"], 6);

	EXPECT_EQ(network.reactionList.size(), 2);
	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants.size(), 2);
	EXPECT_EQ(reaction1.products.size(), 1);

	auto reaction2 = network.reactionList[1];
	EXPECT_EQ(reaction2.reactants.size(), 2);
	EXPECT_EQ(reaction2.products.size(), 2);
}

TEST_F(CrnTest, parseReactConst) {
	driver drv;
	drv.parse_string("a:=3;a->(2)b;");

	auto &network = drv.network;

	EXPECT_EQ(network.initNetworkState["a"], 3);

	EXPECT_EQ(network.reactionList.size(), 1);
	auto reaction1 = network.reactionList[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);
	EXPECT_EQ(reaction1.reactionConstant, 2);
}

TEST_F(CrnTest, testComment) {
	driver drv;
	int res = drv.parse_string("#lol\n"
														 "a := 5;\n"
														 "b := 3;\n"
														 "#lolagain \n"
														 "a + b -> a + b + c;\n"
														 "c -> 0;\n"
														 "#loln");
	EXPECT_EQ(res, 0);
	auto &network = drv.network;

	EXPECT_EQ(network.initNetworkState.size(), 3);

	EXPECT_EQ(network.initNetworkState["a"], 5);
	EXPECT_EQ(network.initNetworkState["b"], 3);
	EXPECT_EQ(network.initNetworkState["c"], 0);

	EXPECT_EQ(network.reactionList.size(), 2);
	{
		auto reaction = &network.reactionList[0];
		auto reactants = reaction->reactants;
		auto products = reaction->products;
		EXPECT_EQ(reactants.size(), 2);
		EXPECT_EQ(reactants["a"], 1);
		EXPECT_EQ(reactants["b"], 1);
		EXPECT_EQ(products.size(), 3);
		EXPECT_EQ(products["a"], 1);
		EXPECT_EQ(products["b"], 1);
		EXPECT_EQ(products["c"], 1);
	}
	{
		auto reaction = &network.reactionList[1];
		auto reactants = reaction->reactants;
		auto products = reaction->products;
		EXPECT_EQ(reactants.size(), 1);
		EXPECT_EQ(reactants["c"], 1);
		EXPECT_EQ(products.size(), 0);
	}
}
