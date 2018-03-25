#ifndef EVAL_MPF_H
#define EVAL_MPF_H
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 512 bit of pi number
#ifndef M_PI_STR
#define M_PI_STR "3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117"
#endif

// error codes (?)
#define WRONG_BRACKETS 10000
#define IMPOSIBLE_VALUE_CONVERSION 10001
#define EMPTY_EXPRESSION 10002
#define UNKNOWN_VARIABLE_OR_FUNCTION 10003
#define DIVISION_BY_ZERO 10004
#define UNKNOWN_VARIABLE_OR_FUNCTION_D 10005
#define UNKNOWN_CALCULATE_D_ERROR 10006
#define UNKNOWN_CALCULATE_ERROR 10099

// arbitrary-precision arithmetic
typedef boost::multiprecision::cpp_dec_float<100> mp_float_100;
typedef boost::multiprecision::number<mp_float_100, boost::multiprecision::et_off> float100noet;
// by default using float_100_et everywhere
typedef boost::multiprecision::number<mp_float_100, boost::multiprecision::et_on> float100et;
// complex numbers
typedef std::complex<float100et> complexFloat100et;
// function with two arguments
typedef float100et (*mathFunctionTwoArgs)(float100et, float100et);
// function with one atgument
typedef float100et (*mathFunctionOneArg)(float100et);

// usefull constants
const float100et ZERO("0");
const float100et ONE("1");

using boost::math::constants::pi;
using boost::multiprecision::pow;
using boost::multiprecision::sin;
using boost::multiprecision::asin;
using boost::multiprecision::cos;
using boost::multiprecision::acos;
using boost::multiprecision::tan;
using boost::multiprecision::atan;
using boost::multiprecision::exp;
using boost::multiprecision::log;
using boost::multiprecision::sqrt;
using boost::multiprecision::fabs;
using boost::multiprecision::swap;

// Class for evaluating formula specified by the string
class cseval
{
private:
    // kind of formula element:
    // 'n' - number, 'v' - variable, 'f' - function, 'e' - error
    char kind;
    // the function name or variable name for current node. e.g. "+","-","/","x","a"
    // (!) all variable names must be represented by one Latin letter (!)
    std::string id;
    // value which was parset from expression, value makes sense if kind is 'n'
    float100et value;
    // child elements in the formula tree
    cseval *leftEval, *rightEval;
    // try to find each symbol of {symbols} string in the {str} string
    std::unordered_map<char, int> findSymbolsOutsideBrackets(const std::string &str, const std::string &symbols) const;
    bool isThereSymbolsOutsideBrackets(const std::string &str) const;
public:
    cseval(std::string expression);
    ~cseval();
    // evaluation of subformula
    float100et calculate(const std::map<std::string, float100et> &mapVariablesValues,
                         const std::map<std::string, mathFunctionTwoArgs> &mapFunctionTwoArgsValue,
                         const std::map<std::string, mathFunctionOneArg> &mapFunctionOneArgValue) const;
    // evaluation derivative of subformula
    float100et calculateDerivative(const std::string &variable,
                                   const std::map<std::string, float100et> &mapVariablesValues,
                                   const std::map<std::string, mathFunctionTwoArgs> &mapFunctionTwoArgsValue,
                                   const std::map<std::string, mathFunctionOneArg> &mapFunctionOneArgValue,
                                   const std::map<std::string, mathFunctionTwoArgs> &mapFunctionDerivLeft,
                                   const std::map<std::string, mathFunctionTwoArgs> &mapFunctionDerivRight) const;
};

#endif // EVAL_MPF_H
