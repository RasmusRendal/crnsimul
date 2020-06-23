#include "eulerevaluator.h"
#include "evaluatorfrontend.h"
#include <gtest/gtest.h>
#include <iostream>

class FrontendTest : public ::testing::Test {
	void SetUp() override {}
};

/*
 * TODO: Fix later
TEST_F(FrontendTest, PrintEvaluatedCSV) {
	std::stringstream *csvStream = nullptr;
	csvStream = new std::stringstream();
	std::stringstream test;
	test << "t,a,b,c,d\n";
	test << "0.000000,5.000000,3.000000,500.000000,0.000000\n";
	test << "0.000100,4.750000,3.000000,499.750000,0.250000\n";
	// reaction used: a:=5; b:=3 c:=500 d:=0 a + c -> d
	driver drv;
	drv.parse_string("a:=5; b:=3; c:=500; d:=0; a + c -> d;");
	EvaluatorFrontend frontend;
	frontend.csvStream = csvStream;
	frontend.drv = &drv;
	EulerEvaluator e(drv.network);
	e.threshold = 1;
	frontend.evaluator = &e;
	frontend.FuncRunner();
	frontend.PrintCsvString();
	std::string expected = test.str();
	std::string result = frontend.csvStream->str();
	EXPECT_EQ(expected, result);
}
*/

TEST_F(FrontendTest, SpecificPlotting) {
	driver drv;
	drv.parse_string("z:=30; b:=50; aa:=3;");
	EvaluatorFrontend frontend;
	frontend.drv = &drv;
	frontend.initNetworkState = drv.network.initNetworkState;
	std::vector<std::string> expected;
	expected.push_back("z");
	expected.push_back("aa");
	frontend.desiredChemicals.push_back("z");
	frontend.desiredChemicals.push_back("aa");
	std::vector<std::string> actual = frontend.GeneratePlotString();
	EXPECT_EQ(expected, actual);
}

TEST_F(FrontendTest, SpecificPlottingNotEqual) {
	driver drv;
	drv.parse_string("z:=30; b:=50; aa:=3;");
	EvaluatorFrontend frontend;
	frontend.drv = &drv;
	frontend.initNetworkState = drv.network.initNetworkState;
	std::vector<std::string> expected;
	expected.push_back("z");
	expected.push_back("a");
	frontend.desiredChemicals.push_back("z");
	frontend.desiredChemicals.push_back("aa");
	std::vector<std::string> actual = frontend.GeneratePlotString();
	EXPECT_NE(expected, actual);
}

TEST_F(FrontendTest, SpecificPlottingNotInSystem) {
	driver drv;
	drv.parse_string("z:=30; b:=50; aa:=3;");
	EvaluatorFrontend frontend;
	frontend.drv = &drv;
	frontend.initNetworkState = drv.network.initNetworkState;
	std::vector<std::string> expected;
	expected.push_back("z");
	expected.push_back("aa");
	frontend.desiredChemicals.push_back("z");
	frontend.desiredChemicals.push_back("aaaaaaa");
	ASSERT_THROW(frontend.GeneratePlotString(), std::runtime_error);
}
