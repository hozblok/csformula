#ifndef CSFORMULA_H
#define CSFORMULA_H
#include "cseval.h"

// {csformula} - wrapper for a {cseval} class that will parse a string
// {csformula} contains case insensitive string parser.
// Each variable can only be set as a single Latin character in a string
//
// to calculate the value of the derivative of a function, we use the following simple transformations:
// (u + v)' = u' + v' = 1 * d + 1 * c
// (u - v)' = u' - v' = 1 * d - 1 * c
// (u * v)' = v * u' + u * v'= b * d + a * c
// (u / v)' = (v * u' - u * v') / v^2 = (1 / v) * u' - (u / v^2) * v' = (1 / b) * d - (a / b^2) * c
// (u ^ v)' = (e^(v * ln(u)))' = e^(v * ln(u)) * (v * ln(u))'=v * u^(v - 1) * u' + (u^v) * ln(u) * v' =
// = b * a^(b - 1) * d + a^b * ln(a) * c
class csformula
{
private:
    // expression to evaluate, for example "(x+1)*(y-0.004)*(sin(x))^2"
    std::string expression;
    // pointer to a recursive, smart formula string parser
    cseval * eval;
    // simple checker the correct order of parentheses
    bool validateBrackets(const std::string &str);

    //+ general static methods that have a one-to-one correspondence
    // with the operations specified in the expression string
    // "+" - addition
    static float100et _add(float100et a, float100et b) { return a + b; }
    // "-" - subtraction
    static float100et _sub(float100et a, float100et b) { return a - b; }
    // "/" - division
    static float100et _truediv(float100et a, float100et b) {
        if (b == ZERO) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return a / b;
    }
    // division for the computation of the derivative (left path)
    static float100et _truediv1(float100et, float100et b) {
        if (b == ZERO) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return 1 / b;
    }
    // division for the computation of the derivative (right path)
    static float100et _truediv2(float100et a, float100et b) {
        if (b == ZERO) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return ZERO - a / (b * b);
    }
    // "*" - multiplication
    static float100et _mul(float100et a, float100et b) { return a * b; }
    // multiplication for the computation of the derivative (left path)
    static float100et _mul1(float100et, float100et b) { return b; }
    // multiplication for the computation of the derivative (right path)
    static float100et _mul2(float100et a, float100et) { return a; }
    // "^" - exponentiation
    static float100et _pow(float100et a, float100et b) { return pow(a, b); }
    // exponentiation for the computation of the derivative (left path)
    static float100et _pow1(float100et a, float100et b) { return (b * _pow(a, b - ONE)); }
    // exponentiation for the computation of the derivative (right path)
    //TODO test log()
    static float100et _pow2(float100et a, float100et b) { return (_log(a) * _pow(a, b)); }
    //- general static methods

    //+ trigonometric functions, exp, log, sqrt and methods for the computation of the derivative:
    // "sin"
    static float100et _sin(float100et a) { return sin(a); }
    // "sin" for the derivative
    static float100et _sin_d(float100et a, float100et) { return cos(a); }
    // "asin"
    static float100et _asin(float100et a) { return asin(a); }
    // "asin" for the derivative
    static float100et _asin_d(float100et a, float100et) {
        if (a * a == ONE) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return ONE / _sqrt(ONE - a * a);
    }
    // "cos"
    static float100et _cos(float100et a) { return cos(a); }
    // "cos" for the derivative
    static float100et _cos_d(float100et a, float100et) { return ZERO - _sin(a); }
    // "acos"
    static float100et _acos(float100et a) { return acos(a); }
    static float100et _acos_d(float100et a, float100et) {
        if (a * a == ONE) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return ZERO - (ONE / _sqrt(ONE - a * a));
    }
    // "tan"
    static float100et _tan(float100et a) { return tan(a); }
    // "tan" for the derivative
    static float100et _tan_d(float100et a, float100et) {
        if (_cos(a) == ZERO) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return ONE / (_cos(a) * _cos(a));
    }
    // "atan"
    static float100et _atan(float100et a) { return atan(a); }
    // "atan" for the derivative
    static float100et _atan_d(float100et a, float100et) { return ONE / (ONE + a * a); }
    // "exp"
    static float100et _exp(float100et a) { return exp(a); }
    // "exp" for the derivative
    static float100et _exp_d(float100et a, float100et) { return exp(a); }
    // "log" - (!) Natural logarithm
    static float100et _log(float100et a) { return log(a); }
    // "log" - (!) Natural logarithm for the derivative
    static float100et _log_d(float100et a, float100et) {
        if (a == ZERO) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return ONE / a;
    }
    // "sqrt" - square root
    static float100et _sqrt(float100et a) { return sqrt(a); }
    // "sqrt" for the derivative
    static float100et _sqrt_d(float100et a, float100et) {
        if (sqrt(a) == ZERO) {
          std::cerr << "ERROR: division by zero error!";
          throw DIVISION_BY_ZERO;
        }
        return float100et("0.5") / sqrt(a);
    }
    //- trigonometric functions, exp, log, sqrt

    //+ auxiliary methods for the computation of the derivative:
    static float100et _zero(float100et, float100et) { return ZERO; }
    static float100et _one(float100et, float100et) { return ONE; }
    static float100et _m_one(float100et, float100et) { return ZERO-ONE; }
    //- auxiliary methods

    // dictionaries contain the appropriate names of operations and static methods for evaluating, e.g.
    // "+" -> _add
    // "sin" -> _sin
    static const std::map<std::string, mathFunctionTwoArgs> functionsTwoArgs;
    static const std::map<std::string, mathFunctionOneArg> functionsOneArg;

    // dictionaries contain references to derivatives of basic functions and their names:
    static const std::map<std::string, mathFunctionTwoArgs> functionsTwoArgsDLeft;
    static const std::map<std::string, mathFunctionTwoArgs> functionsTwoArgsDRight;
    static const std::map<std::string, mathFunctionOneArg> functionsOneArgD;

public:
    csformula(const std::string &texpression);
    ~csformula();
    void setExpression(const std::string &texpression);
    std::string getExpression() const {return expression;}

    // get the calculated value of the formula in accordance with the dictonary {mapVariableValues}
    // contains values of the variables
    // NOTE: Variables can be only letters of the Latin alphabet ('a','b',...,'z')
    float100et get(const std::map<std::string, float100et> &mapVariableValues) const;
    // get the calculated value of partial derivative with respect to a variable {variable} and
    // the {mapVariableValues} dictonary contains values of the variables
    float100et getD(const std::string variable, const std::map<std::string, float100et> &mapVariableValues) const;
};

#endif // CSFORMULA_H
