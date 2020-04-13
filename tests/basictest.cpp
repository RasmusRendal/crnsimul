#include <gtest/gtest.h>
class BasicTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(BasicTest, twoValues) {
	const int x = 4;
	const int y = 5;
	EXPECT_EQ(9, x + y);
}
