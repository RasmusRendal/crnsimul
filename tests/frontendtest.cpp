#include <gtest/gtest.h>
#include "evaluatorfrontend.h"

class FrontendTest : public ::testing::Test {
    void SetUp() override {}
};

TEST_F(FrontendTest, PrintCSVHeader) {
    std::string expectedHeaderStr = "t,a,b,c,d,e\n";
    NetworkState initNetworkState;
    initNetworkState.insert(std::pair<std::string, double>("a", 0));
    initNetworkState.insert(std::pair<std::string, double>("b", 0));
    initNetworkState.insert(std::pair<std::string, double>("c", 0));
    initNetworkState.insert(std::pair<std::string, double>("d", 0));
    initNetworkState.insert(std::pair<std::string, double>("e", 0)); 
    EXPECT_EQ(expectedHeaderStr, initNetworkState.PrintCsvHeader());
}

TEST_F(FrontendTest, PrintCSVRow) {
    std::string expectedRowStr = "0.000000,500.000000,66.000000,350.000000,99999.000000,5.000000\n";
    NetworkState initNetworkState;
    initNetworkState.insert(std::pair<std::string, double>("a", 500));
    initNetworkState.insert(std::pair<std::string, double>("b", 66));
    initNetworkState.insert(std::pair<std::string, double>("f", 350));
    initNetworkState.insert(std::pair<std::string, double>("x", 99999));
    initNetworkState.insert(std::pair<std::string, double>("z", 5));
    EXPECT_EQ(expectedRowStr, initNetworkState.PrintCsvRow()); 
}

TEST_F(FrontendTest, PrintEvaluatedCSVRow) {
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