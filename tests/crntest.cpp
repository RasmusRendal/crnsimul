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

TEST_F(CrnTest, twoValues) {
	driver drv;
	drv.parse_string("a:=3;a->b;");
	const int x = 4;
	const int y = 5;
	EXPECT_EQ(drv.Concentration["a"], 3);
	EXPECT_EQ(drv.reactions.size(), 1);
	auto reaction1 = drv.reactions[0];
	EXPECT_EQ(reaction1.reactants["a"], 1);
	EXPECT_EQ(reaction1.reactants["b"], 1);
}
