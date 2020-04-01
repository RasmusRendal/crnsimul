#include <gtest/gtest.h>
#include "evaluatorfrontend.h"
#include <iostream>

class FrontendTest : public ::testing::Test {
    void SetUp() override {}
};

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
    frontend.eulerEval = &e;
    frontend.FuncRunner();
    frontend.PrintCsvString();
    std::string expected = test.str();
    std::string result = frontend.csvStream->str();
    EXPECT_EQ(expected, result);
}

/*TEST_F(FrontendTest, PrintCSV) {
    driver drv;
    std::stringstream *returnStream = nullptr;
    drv.parse_string("a:=5; b:=3; c:=500; d:=0; a + c -> d;");
    EvaluatorFrontend frontend;
    std::string expectedHeader = "t,a,b,c,d\n";
    std::string expectedRow0 = "0,5,3,500,0\n";
    std::string expectedRow1 = "0.000100,4.750000,3.000000,499.750000,0.250000\n";
    frontend.drv = &drv;
    returnStream = new std::stringstream;
    frontend.csvStream = returnStream;
} */