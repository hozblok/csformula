#include <iostream>
#include <boost/test/minimal.hpp>
#include "csformula.h"


int test_main (int, char *[ ]){
    // allowed: 'x/-y' -> allowed: 'x/(-y)'
    // allowed: '+x' -> allowed: 'x'
    // allowed: (---x), allowed: (--x), allowed: (13*z/+--+x)

    std::cout << "test 000: ' \\tx \\v+ 1.0\\n +z+(1)'\n";
    std::string expression_000(" \tx \v+ 1.0\n +z+(1)");
    csformula formula_000(expression_000);
    std::map<std::string, float100et> map000 = {{"x", float100et("1")},
                                                {"z", float100et("11.00")}};
//    assert(formula_000.get(map000).str() == "14");
    BOOST_CHECK(formula_000.get(map000).str() == "14");

    std::cout << "test 000_: '(x*x+y*y-1)/(z-0.5)'\n";
    std::string expression_000_("(x*x+y*y-1)/(z-0.5)");
    csformula formula_000_(expression_000_);
    std::map<std::string, float100et> map000_ = {{"x", float100et("1")},
                                                 {"y", float100et("1")},
                                                 {"z", float100et("1")}};
    BOOST_CHECK(formula_000_.get(map000_).str() == "2");

    std::cout << "test 000_1: '((2-1)/(z-0.5))/(z-0.5)'\n";
    std::string expression_000_1("((2-1)/(z-0.5))/(z-0.5)");
    csformula formula_000_1(expression_000_1);
    std::map<std::string, float100et> map000_1 = {{"z", float100et("1")}};
    BOOST_CHECK(formula_000_1.get(map000_1).str() == "4");

    std::cout << "test 001: '1+--+-x+-+-14'\n";
    std::string expression_001("1+--+-x+-+-14");
    csformula formula_001(expression_001);
    std::map<std::string, float100et> map001 = {{"x", float100et("1")},
                                                  {"z", float100et("0")}};
    BOOST_CHECK(formula_001.get(map001).str() == "14");

    //10/5*2 === 4
    //10/5*2+1
    std::cout << "test 002: '10/----5*x+10'\n";
    std::string expression_002("10/----5*x+10");
    csformula formula_002(expression_002);
    std::map<std::string, float100et> map002 = {{"x", float100et("2")}};
    BOOST_CHECK(formula_002.get(map002).str() == "14");


//    std::cout << std::numeric_limits<float_100_et>::digits10 << " "
//                << std::setprecision(std::numeric_limits<float_100_et>::max_digits10) << formula_001.get(map) << "\n";
    float100et aa("1");
    float100et bb("0.00000000000000000000000000000000014000000000000000000009");
    float100et cc = aa + bb;
//    std::cout << cc.str() << "\n";
    BOOST_CHECK(cc.str() == "1.00000000000000000000000000000000014000000000000000000009");
//    std::cout << cseval_001.getValue(9., 10., 10) << "=== 10" << "\n";
//    std::cout << cseval_001.getValue(-1) << "=== 0" << "\n";
//    std::cout << cseval_001.getValue(5e-49, 10., 10) << "=== 1+5e-49" << "\n";

    //10/5*2 === 4
    //10/5*2+1
    std::cout << "test 004: '(14*y)'y'\n";
    std::string expression_004("14*y");
    csformula formula_004(expression_004);
    std::map<std::string, float100et> map004 = {{"x", float100et("1")},
                                                  {"y", float100et("1")}};
    BOOST_CHECK(formula_004.getD("y", map004).str() == "14");

    std::cout << "test 005: '(0.5*x*x+y)'x'\n";
    std::string expression_005("0.5*x*x+y");
    csformula formula_005(expression_005);
    std::map<std::string, float100et> map005 = {{"x", float100et("14")},
                                                  {"y", float100et("100")}};
    BOOST_CHECK(formula_005.getD("x", map005).str() == "14");

    std::cout << "test 006: '(x^2)-2'\n";
    std::string expression_006("x^2-2");
    csformula formula_006(expression_006);
    std::map<std::string, float100et> map006 = {{"x", float100et("4")}};
    BOOST_CHECK(formula_006.get(map006).str() == "14");

    std::cout << "test 007: '(x^2)+10.2-x/y'x'\n";
    std::string expression_007("(x^2)+10.2-x/y");
    csformula formula_007(expression_007);
    std::map<std::string, float100et> map007 = {{"x", float100et("2")},
                                                  {"y", float100et("-0.10")}};
    BOOST_CHECK(formula_007.getD("x", map007).str() == "14");

    std::cout << "test 008: 'sin(x) + 13*x'x'\n";
    std::string expression_008("sin(x) + 13*x");
    csformula formula_008(expression_008);
    std::map<std::string, float100et> map008 = {{"x", float100et("0")}};
    BOOST_CHECK(formula_008.getD("x", map008).str() == "14");

    std::cout << "test 009: '-14 * (x < 0) + 14 * (x > 0) + 0.14 * (x = 0)'";
    csformula formula_("-14 * (x < 0) + 14 * (x > 0) + 0.14 * (x = 0)");
    BOOST_CHECK(formula_.get({{"x", float100et("0")}}).str() == "0.14");
    BOOST_CHECK(formula_.get({{"x", float100et("-10")}}).str() == "-14");
    BOOST_CHECK(formula_.get({{"x", float100et("0.000000000000000000000000000000000000001")}}).str() == "14");

    std::cout << "test 010: '-14 * (x < 0) + 14 * (x > 0) + 0.14 * (x = 0)'";
    csformula formula__("14 * (x & y) + 0.14 * (x | y) + 0.0014 * (x = y)");
    BOOST_CHECK(formula__.get({{"x", float100et("0")},{"y", float100et("0")}}).str() == "0.0014");
    BOOST_CHECK(formula__.get({{"x", float100et("14")},{"y", float100et("0")}}).str() == "0.14");
    BOOST_CHECK(formula__.get({{"x", float100et("0")},{"y", float100et("14")}}).str() == "0.14");
    BOOST_CHECK(formula__.get({{"x", float100et("14")},{"y", float100et("0.14")}}).str() == "14.14");
    BOOST_CHECK(formula__.get({{"x", float100et("14")},{"y", float100et("14")}}).str() == "14.1414");

    return 0;
}
