#include <gtest/gtest.h>
#include "CRN/driver.hh"

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
	drv.parse_string("a:=0;a->b;");
	const int x = 4;
	const int y = 5;
	EXPECT_EQ(9, x + y);
}
