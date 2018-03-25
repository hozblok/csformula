#include "csformula.h"

int main()
{
    std::string expression("x+y");
    csformula formula(expression);
    std::cout << "result of evaluating 'x+y'(2,2): " << formula.get({{"x", float100et("2")},
                                                                     {"y", float100et("2")}}).str() << "\n";

    return 0;
}
