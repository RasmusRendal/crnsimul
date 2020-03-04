#include "CRN/driver.h"
#include <gtest/gtest.h>
#include "reaction.h"

class CrnTest : public ::testing::Test {
protected:
	virtual void SetUp() {}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CrnTest, parseSingle) {
	driver drv;
	drv.parse_string("a:=3;a->b;");

	EXPECT_EQ(drv.Concentration["a"], 3);

	EXPECT_EQ(drv.reactions.size(), 1);
	auto reaction1 = drv.reactions[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);
}

TEST_F(CrnTest, parseTwo) {
	driver drv;

	// Concentrations
	drv.parse_string("a:=3;a->b;b->a;");
	EXPECT_EQ(drv.Concentration["a"], 3);

	// Reactions
	EXPECT_EQ(drv.reactions.size(), 2);

	// Reaction 1
	auto reaction1 = drv.reactions[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);

	// Reaction 2
	auto reaction2 = drv.reactions[1];
	EXPECT_EQ(reaction2.reactants["b"], 1);
	EXPECT_EQ(reaction2.products["a"], 1);
}

TEST_F(CrnTest, parseZero) {
	driver drv;
	drv.parse_string("a:=3; a->b; b->0;");

	EXPECT_EQ(drv.Concentration["a"], 3);

	EXPECT_EQ(drv.reactions.size(), 2);

	auto reaction1 = drv.reactions[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products["b"], 1);

	auto reaction2 = drv.reactions[1];
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

	EXPECT_EQ(drv.Concentration["a"], 3);
	EXPECT_EQ(drv.Concentration["b"], 5);

	EXPECT_EQ(drv.reactions.size(), 3);

	auto reaction1 = drv.reactions[0];
	EXPECT_EQ(reaction1.reactants.size(), 1);
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.products.size(), 1);
	EXPECT_EQ(reaction1.products["b"], 1);

	auto reaction2 = drv.reactions[1];
	EXPECT_EQ(reaction2.reactants.size(), 1);
	EXPECT_EQ(reaction2.reactants["b"], 1);
	EXPECT_EQ(reaction2.products.size(), 0);

	auto reaction3 = drv.reactions[2];
	EXPECT_EQ(reaction3.reactants.size(), 1);
	EXPECT_EQ(reaction3.reactants["c"], 1);
	EXPECT_EQ(reaction3.products.size(), 1);
	EXPECT_EQ(reaction3.products["a"], 77);
}
