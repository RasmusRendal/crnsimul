#include "networkstate.h"
#include "CRN/driver.h"
#include "eulerevaluator.h"
#include <gtest/gtest.h>
#include <tgmath.h>

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

TEST_F(NetworkStateTest, PrintCSVHeader) {
	std::string expectedHeaderStr = "t,a,b,c,d,e\n";
	NetworkState initNetworkState;
	initNetworkState.insert(std::pair<std::string, double>("a", 0));
	initNetworkState.insert(std::pair<std::string, double>("b", 0));
	initNetworkState.insert(std::pair<std::string, double>("c", 0));
	initNetworkState.insert(std::pair<std::string, double>("d", 0));
	initNetworkState.insert(std::pair<std::string, double>("e", 0));
	EXPECT_EQ(expectedHeaderStr, initNetworkState.PrintCsvHeader());
}

TEST_F(NetworkStateTest, PrintCSVRow) {
	std::string expectedRowStr =
			"0.000000,500.000000,66.000000,350.000000,99999.000000,5.000000\n";
	NetworkState initNetworkState;
	initNetworkState.insert(std::pair<std::string, double>("a", 500));
	initNetworkState.insert(std::pair<std::string, double>("b", 66));
	initNetworkState.insert(std::pair<std::string, double>("f", 350));
	initNetworkState.insert(std::pair<std::string, double>("x", 99999));
	initNetworkState.insert(std::pair<std::string, double>("z", 5));
	EXPECT_EQ(expectedRowStr, initNetworkState.PrintCsvRow());
}

TEST_F(NetworkStateTest, PrintEvaluatedCSVRow) {
	// reaction used: a:=5; b:=3 c:=500 d:=0 a + c -> d
	NetworkState initNetworkState;
	std::string expectedHeader = "t,a,b,c,d\n";
	std::string expectedRow = "0.000100,4.750000,3.000000,499.750000,0.250000\n";
	initNetworkState.insert(std::pair<std::string, double>("a", 5));
	initNetworkState.insert(std::pair<std::string, double>("b", 3));
	initNetworkState.insert(std::pair<std::string, double>("c", 500));
	initNetworkState.insert(std::pair<std::string, double>("d", 0));
	std::vector<NetworkState> states;
	driver drv;
	drv.parse_string("a:=5; b:=3; c:=500; d:=0; a + c -> d;");
	EulerEvaluator e(drv.network);
	states.push_back(e.GetNextNetworkState());
	EXPECT_EQ(expectedRow, states[0].PrintCsvRow());
	EXPECT_EQ(expectedHeader, states[0].PrintCsvHeader());
}

TEST_F(NetworkStateTest, TestSimpleAddition) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 1));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 1));
	EXPECT_EQ((state1 + state2)["a"], 2);
}

TEST_F(NetworkStateTest, TestAssAddition) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 1));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("b", 1));
	EXPECT_EQ((state1 + state2)["a"], 1);
	EXPECT_EQ((state1 + state2)["b"], 1);
}

TEST_F(NetworkStateTest, TestSimpleSubtraction) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 5));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 2));
	EXPECT_EQ((state1 - state2)["a"], 3);
}

TEST_F(NetworkStateTest, TestSubtractionOutOfRange) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 2));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 5));
	ASSERT_THROW(state1 - state2, NetworkStateOutOfRange);
}

TEST_F(NetworkStateTest, TestSubtractionOutOfRangeUndefined) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 2));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("b", 5));
	ASSERT_THROW(state1 - state2, NetworkStateOutOfRange);
}

TEST_F(NetworkStateTest, LessThanTest) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 2));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 5));
	ASSERT_EQ(state1 < state2, true);
}

TEST_F(NetworkStateTest, LessThanTestFalse) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 2));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 5));
	ASSERT_EQ(state2 < state1, false);
}

TEST_F(NetworkStateTest, LessThanTestFalseIfSame) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 2));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 2));
	ASSERT_EQ(state2 < state1, false);
}

TEST_F(NetworkStateTest, MultipleCompTest) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 5));
	state1.insert(std::pair<std::string, double>("b", 5));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 100));
	state2.insert(std::pair<std::string, double>("b", 2));
	ASSERT_EQ(state1 < state2, true);
}

TEST_F(NetworkStateTest, SubtractFinding) {
	NetworkState state1;
	state1.insert(std::pair<std::string, double>("a", 1));
	state1.insert(std::pair<std::string, double>("b", 0));
	NetworkState state2;
	state2.insert(std::pair<std::string, double>("a", 1));
	auto next = state1 - state2;
	ASSERT_EQ(next["a"], 0);
	ASSERT_EQ(std::roundf(next["b"]), 0);
}
