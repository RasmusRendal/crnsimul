#include "networkstate.h"
#include <gtest/gtest.h>

class NetworkStateTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(NetworkStateTest, EmptyStates) {
	NetworkState s1;
	NetworkState s2;
	EXPECT_EQ(s1.Diff(s2), 0);
}

TEST_F(NetworkStateTest, OneState) {
	NetworkState s1;
	NetworkState s2;
	s1.insert(std::make_pair("a", 5));
	EXPECT_EQ(s1.Diff(s2), 5);
}

TEST_F(NetworkStateTest, OneStateInOther) {
	NetworkState s1;
	NetworkState s2;
	s2.insert(std::make_pair("a", 5));
	EXPECT_EQ(s1.Diff(s2), 5);
}

TEST_F(NetworkStateTest, ActualDiff) {
	NetworkState s1;
	NetworkState s2;
	s1.insert(std::make_pair("a", 3));
	s2.insert(std::make_pair("a", 5));
	EXPECT_EQ(s1.Diff(s2), 2);
}

TEST_F(NetworkStateTest, Diff1) {
	NetworkState s1;
	NetworkState s2;
	s1.insert(std::make_pair("a", 3));
	s2.insert(std::make_pair("a", 5));
	s1.insert(std::make_pair("b", 3));
	s2.insert(std::make_pair("b", 3));
	EXPECT_EQ(s1.Diff(s2), 2);
}
