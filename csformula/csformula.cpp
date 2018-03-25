#include "csformula.h"

const std::map<std::string, mathFunctionTwoArgs> csformula::functionsTwoArgs = {{std::string("+"), _add},
                                                                                {std::string("-"), _sub},
                                                                                {std::string("/"), _truediv},
                                                                                {std::string("*"), _mul},
                                                                                {std::string("^"), _pow}};

const std::map<std::string, mathFunctionOneArg> csformula::functionsOneArg = {{std::string("sin"), _sin},
                                                                              {std::string("asin"), _asin},
                                                                              {std::string("cos"), _cos},
                                                                              {std::string("acos"), _acos},
                                                                              {std::string("tan"), _tan},
                                                                              {std::string("atan"), _atan},
                                                                              {std::string("log"), _log},
                                                                              {std::string("sqrt"), _sqrt},
                                                                              {std::string("exp"), _exp}};

const std::map<std::string, mathFunctionTwoArgs> csformula::functionsTwoArgsDLeft = {{std::string("+"), _one},
                                                                                     {std::string("-"), _one},
                                                                                     {std::string("*"), _mul1},
                                                                                     {std::string("/"), _truediv1},
                                                                                     {std::string("^"), _pow1},
                                                                                     {std::string("sin"), _sin_d},
                                                                                     {std::string("asin"), _asin_d},
                                                                                     {std::string("cos"), _cos_d},
                                                                                     {std::string("acos"), _acos_d},
                                                                                     {std::string("tan"), _tan_d},
                                                                                     {std::string("atan"), _atan_d},
                                                                                     {std::string("log"), _log_d},
                                                                                     {std::string("sqrt"), _sqrt_d},
                                                                                     {std::string("exp"), _exp_d}};

const std::map<std::string, mathFunctionTwoArgs> csformula::functionsTwoArgsDRight = {{std::string("+"), _one},
                                                                                      {std::string("-"), _m_one},
                                                                                      {std::string("*"), _mul2},
                                                                                      {std::string("/"), _truediv2},
                                                                                      {std::string("^"), _pow2}};

csformula::csformula(const std::string &texpression):
    expression(""),
    eval(0)
{
    setExpression(texpression);
}

csformula::~csformula()
{
    delete eval;
    eval = 0;
}

void csformula::setExpression(const std::string &texpression)
{
    expression = texpression;
    if (expression.empty()) {
        std::cerr << "ERROR: Empty expression\n";
        throw EMPTY_EXPRESSION;
    }
    if (!validateBrackets(expression)) {
        std::cerr << "ERROR: wrong brackets in expression\n";
        throw WRONG_BRACKETS;
    }
    boost::algorithm::to_lower(expression);

    // remove spaces, \n, \r, \t, etc.
    boost::algorithm::erase_all(expression, " ");
    boost::algorithm::erase_all(expression, "\n");
    boost::algorithm::erase_all(expression, "\r");
    boost::algorithm::erase_all(expression, "\t");
    boost::algorithm::erase_all(expression, "\v");
    boost::algorithm::erase_all(expression, "\a");

    eval = new cseval(expression);
}

bool csformula::validateBrackets(const std::string &str) {
    int count = 0;
    for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it) {
        if (*it == '(') {
            count++;
        } else if (*it == ')') {
            count--;
        }
        if (count < 0) {
            return false;
        }
    }
    return (count == 0);
}

float100et csformula::get(const std::map<std::string, float100et> &mapVariableValues) const
{
    return eval->calculate(mapVariableValues, functionsTwoArgs, functionsOneArg);
}

float100et csformula::getD(const std::string variable, const std::map<std::string, float100et> &mapVariableValues) const
{
    return eval->calculateDerivative(variable,
                                     mapVariableValues,
                                     functionsTwoArgs,
                                     functionsOneArg,
                                     functionsTwoArgsDLeft,
                                     functionsTwoArgsDRight);
}
