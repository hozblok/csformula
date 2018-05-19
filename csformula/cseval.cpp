#include "cseval.h"

cseval::cseval(std::string expression):
    kind('e'),
    id(""),
    value(0),
    leftEval(0),
    rightEval(0)
{
    if (expression.empty()) {
        std::cerr << "ERROR: Empty expression! Check string, brackets and values within brackets!\n";
        throw EMPTY_EXPRESSION;
    }

    // remove braces
    while (!isThereSymbolsOutsideBrackets(expression)) {
        expression.erase(expression.cbegin());
        expression.pop_back();
    }

    // find operations: logical or, logical and, relational =, <, >, addition, subtraction, multiplication, division, the construction of the power
    // First, we are looking for addition and subtraction,
    // because division and multiplication have a higher priority
    // and should be performed first.
    std::string operations("|&=><+-*/^");
    std::unordered_map<char, int> foundedOperation = findSymbolsOutsideBrackets(expression, operations);
    for (std::string::const_iterator it = operations.cbegin(); it != operations.cend(); ++it) {
        if (foundedOperation.at(*it) != -1) {
            kind = 'f';
            id = *it;
            if (*it == '-' || *it == '+') {
                // if '-' represents negative number or negative value of variable, not a subtraction operation
                // similarly for '+'
                if (foundedOperation.at(*it) == 0) {
                    // allowed '-x', '--x', '---x*y', '-.01' etc.
                    leftEval = new cseval(std::string("0"));
                    rightEval = new cseval(expression.substr(1));
                    return;
                } else if (operations.find(expression.at(foundedOperation.at(*it) - 1)) != std::string::npos) {
                    // allowed 'x/-1' or 'y^-x' etc.
                    // go on to the next operation
                    continue;
                }
            }
            // split the string into two parts
            // separator: + - * / ^
            leftEval = new cseval(expression.substr(0, foundedOperation.at(*it)));
            rightEval = new cseval(expression.substr(foundedOperation.at(*it) + 1));
            return;
        }
    }

    size_t len = expression.size();
    size_t iLeftBracket = expression.find('(');
    if (iLeftBracket == len - 1) {
        std::cerr << "ERROR: cannot parse expression, wrong brackets, last symbol is '('.\n";
        throw WRONG_BRACKETS;
    }
    if (iLeftBracket == std::string::npos) {
        // there is no '(' => kind === 'v' or 'n'; or id === "pi" //variable or number or pi
        if (expression.find("pi") == 0) {
            kind = 'n';
            id = "pi";
            value = float100et(M_PI_STR);
        } else if (len == 1 && expression.at(0) >= 'a' && expression.at(0) <= 'z') {
            // x or y or z or other available variables
            kind = 'v';
            id = expression;
        } else {
            // this is number
            kind = 'n';
            id = expression;
            try {
                value = float100et(expression.data());
            } catch(...) {
                std::cerr << "ERROR: unknown number value: \'" << expression << "\'\n";
                throw IMPOSIBLE_VALUE_CONVERSION;
            }
        }
    } else if (iLeftBracket < len) { // this is a function:
        std::string name_fun = expression.substr(0, iLeftBracket);
        expression = expression.substr(iLeftBracket + 1);
        expression.pop_back();
        kind = 'f';
        id = name_fun;
        size_t iComma = expression.find(',');
        if (iComma != std::string::npos) {
            leftEval = new cseval(expression.substr(0, iComma));
            rightEval = new cseval(expression.substr(iComma + 1));
        } else {
            leftEval = new cseval(expression);
        }
    }
}

cseval::~cseval()
{
    if (leftEval != 0) {
        delete leftEval;
        leftEval = 0;
    }
    if (rightEval != 0) {
        delete rightEval;
        rightEval = 0;
    }
}

float100et cseval::calculate(const std::map<std::string, float100et> &mapVariablesValues,
                               const std::map<std::string, mathFunctionTwoArgs> &mapFunctionTwoArgsValue,
                               const std::map<std::string, mathFunctionOneArg> &mapFunctionOneArgValue) const
{
    if (kind == 'n') {
        return value;
    } else if (kind == 'v') {
        std::map<std::string, float100et>::const_iterator it;
        for (it = mapVariablesValues.cbegin(); it != mapVariablesValues.cend(); ++it) {
            if (it->first == id) {
                return it->second;
            }
        }
    } else if (kind == 'f') {
        if (leftEval != 0 && rightEval != 0) {
            // function with two arguments
            float100et left("0");
            float100et right("0");
            left = leftEval->calculate(mapVariablesValues, mapFunctionTwoArgsValue, mapFunctionOneArgValue);
            right = rightEval->calculate(mapVariablesValues, mapFunctionTwoArgsValue, mapFunctionOneArgValue);
            std::map<std::string, mathFunctionTwoArgs>::const_iterator itFunction;
            itFunction = mapFunctionTwoArgsValue.find(id);
            if (itFunction != mapFunctionTwoArgsValue.cend()) {
                return itFunction->second(left, right);
            } else {
                std::cerr << "ERROR: Unknown variable or function \'" << id << "\'\n";
                throw UNKNOWN_VARIABLE_OR_FUNCTION;
            }
        } else if (leftEval != 0) {
            // function with one argument
            float100et left("0");
            left = leftEval->calculate(mapVariablesValues, mapFunctionTwoArgsValue, mapFunctionOneArgValue);
            std::map<std::string, mathFunctionOneArg>::const_iterator itFunction;
            itFunction = mapFunctionOneArgValue.find(id);
            if (itFunction != mapFunctionOneArgValue.cend()) {
                return itFunction->second(left);
            } else {
                std::cerr << "ERROR: Unknown variable or function \'" << id << "\'\n";
                throw UNKNOWN_VARIABLE_OR_FUNCTION;
            }
        }
    }
    std::cerr << "ERROR: Unknown error, unknown variable or function \'" << id << "\' kind'" << kind << "'\n";
    throw UNKNOWN_CALCULATE_ERROR;
}

float100et cseval::calculateDerivative(const std::string &variable,
                                         const std::map<std::string, float100et> &mapVariablesValues,
                                         const std::map<std::string, mathFunctionTwoArgs> &mapFunctionTwoArgsValue,
                                         const std::map<std::string, mathFunctionOneArg> &mapFunctionOneArgValue,
                                         const std::map<std::string, mathFunctionTwoArgs> &mapFunctionDerivLeft,
                                         const std::map<std::string, mathFunctionTwoArgs> &mapFunctionDerivRight) const
{
    if (kind == 'n') {
        return ZERO;
    } else if (kind == 'v') {
        if (id == variable) {
            return ONE;
        } else {
            return ZERO;
        }
    } else if (kind == 'f') {
        if (leftEval != 0 && rightEval != 0) {
            // (u+v)'=u'+v'=1*d+1*c
            // (u-v)'=u'-v'=1*d-1*c
            // (u*v)'=vu'+uv'=b*d+a*c
            // (u/v)'=(vu'-uv')/v^2=(1/v)*u'-(u/v^2)v'=(1/b)*d-(a/b^2)*c
            // (u^v)'=(e^(v*ln(u)))'=e^(v*ln(u)) * (v*ln(u))'=v*u^(v-1)*u' + (u^v)*ln(u)*v'=b*a^(b-1)*d + a^b*ln(a)*c
            // a===u, b===v, d===u', c===v'
            float100et a = leftEval->calculate(mapVariablesValues,
                                                 mapFunctionTwoArgsValue,
                                                 mapFunctionOneArgValue);
            float100et d = leftEval->calculateDerivative(variable,
                                                           mapVariablesValues,
                                                           mapFunctionTwoArgsValue,
                                                           mapFunctionOneArgValue,
                                                           mapFunctionDerivLeft,
                                                           mapFunctionDerivRight);
            float100et b = rightEval->calculate(mapVariablesValues,
                                                  mapFunctionTwoArgsValue,
                                                  mapFunctionOneArgValue);
            float100et c = rightEval->calculateDerivative(variable,
                                                            mapVariablesValues,
                                                            mapFunctionTwoArgsValue,
                                                            mapFunctionOneArgValue,
                                                            mapFunctionDerivLeft,
                                                            mapFunctionDerivRight);
            std::map<std::string, mathFunctionTwoArgs>::const_iterator itFunction_1;
            itFunction_1 = mapFunctionDerivLeft.find(id);
            std::map<std::string, mathFunctionTwoArgs>::const_iterator itFunction_2;
            itFunction_2 = mapFunctionDerivRight.find(id);
            if (itFunction_1 != mapFunctionDerivLeft.cend() && itFunction_2 != mapFunctionDerivRight.cend()) {
                return itFunction_1->second(a, b) * d + itFunction_2->second(a, b) * c;
            } else {
                std::cerr << "ERROR: Unknown variable or function for derivative operation \'" << id << "\'\n";
                throw UNKNOWN_VARIABLE_OR_FUNCTION_D;
            }
        } else if (leftEval != 0) {
            // the same, but b === 0 and c === 0
            float100et a = leftEval->calculate(mapVariablesValues,
                                                 mapFunctionTwoArgsValue,
                                                 mapFunctionOneArgValue);
            float100et d = leftEval->calculateDerivative(variable,
                                              mapVariablesValues,
                                              mapFunctionTwoArgsValue,
                                              mapFunctionOneArgValue,
                                              mapFunctionDerivLeft,
                                              mapFunctionDerivRight);
            std::map<std::string, mathFunctionTwoArgs>::const_iterator itFunction;
            itFunction = mapFunctionDerivLeft.find(id);
            if (itFunction != mapFunctionDerivLeft.cend()) {
                return itFunction->second(a, ZERO) * d;
            } else {
                std::cerr << "ERROR: Unknown variable or function for derivative operation \'" << id << "\'\n";
                throw UNKNOWN_VARIABLE_OR_FUNCTION_D;
            }
        }
    }
    std::cerr << "ERROR: Unknown error, unknown variable or function for derivative operation \'" << id << "\' kind'" << kind << "'\n";
    throw UNKNOWN_CALCULATE_D_ERROR;
}

std::unordered_map<char, int> cseval::findSymbolsOutsideBrackets(const std::string &str, const std::string &symbols) const
{
    unsigned int countBraces = 0;

    std::unordered_map<char, int> operationIndex;
    for (std::string::const_iterator it = symbols.cbegin(); it < symbols.cend(); ++it) {
        operationIndex[*it] = -1;
    }
    for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it) {
        if (*it == '(') {
            countBraces++;
        }
        else if (*it == ')') {
            countBraces--;
        }
        // we get only the first match in str, so we need the last condition
        if (countBraces == 0 && symbols.find(*it) != std::string::npos && operationIndex.at(*it) == -1) {
            operationIndex[*it] = it - str.cbegin();
        }
    }
    return operationIndex;
}

bool cseval::isThereSymbolsOutsideBrackets(const std::string &str) const
{
    unsigned int countBraces = 0;
    std::string::const_iterator it = str.cbegin();
    if (*it != '(') {
        return true;
    } else {
        countBraces++;
        it++;
    }

    for (; it != str.cend(); ++it) {
        if (countBraces == 0) {
            if (*it == '(') {
                // '(x+1)(y+1)'? Worng expression. There is no mathematical operation between brackets.
                throw WRONG_BRACKETS;
            }
            return true;
        }
        if (*it == '(') {
            countBraces++;
        } else if (*it == ')') {
            countBraces--;
        }
    }
    return false;
}
