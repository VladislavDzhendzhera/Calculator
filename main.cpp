#include <iostream>
#include "calculator.h"

using namespace std;

const unsigned COR_EXPR_COUNT = 15;
const unsigned INCOR_EXPR_COUNT = 14;

double answers[COR_EXPR_COUNT] = {4, -16, 0.0625, 1, -3.2, 12.3, 1.75, 0, 4, -1, -1, 3, 0, -1, 1};
const char* correctExpression[COR_EXPR_COUNT] = {"2+2",
                                                 "4*-4",
                                                 "2^-4",
                                                 "-1^2",
                                                 "-2.1 + 2^ -1+ 2.7*2 - 7",
                                                 "-2.1+2^-2^2+2.7*2-7",
                                                 "2-(2^-2)",
                                                 "- (2)- 4/-	(6-4)",
                                                 "(2)- 4/-	(6-4)",
                                                 "-1",
                                                 "(-1.)",
                                                 "((4.0 - 1.) / (2.0 - 1))",
                                                 "(-1+1)^-(8   -9)	",
                                                 "0-(1)^2",
                                                 "-(1)^2"
                                                 };

const char* incorrectExpression[INCOR_EXPR_COUNT] = {"(",
                                                     "1 / (1-1)",
                                                     "-1 ^ 0.5",
                                                     "((-1+1)^-(8+ 1- 9))",
                                                     "Hello world!",
                                                     "()",
                                                     "	((	(    ) ))  ",
                                                     "(2))+((3)",
                                                     "",
                                                     "10.1.",
                                                     "4+-4",
                                                     "1--1",
                                                     "+1",
                                                     "((+1))"
                                                    };

int main()
{

    Calculator calculator;

    double functionResult;

    cout << "\nTesting correct expression" << endl;
    for(unsigned exprIndex = 0; exprIndex < COR_EXPR_COUNT; ++exprIndex)
    {
        try
        {
            functionResult = calculator.calc(correctExpression[exprIndex]);
        }
        catch(std::runtime_error e)
        {
            cout << "Error on test "                << exprIndex
                 << ". Program catch exception:\""  << e.what()
                 << "\" on expression \""           << correctExpression[exprIndex]
                 << "\"\n";
            return 1;
        }

        if(Calculator::isZero(functionResult - answers[exprIndex]))
        {
            cout << "Test " << exprIndex         << " pass\n"
                 << correctExpression[exprIndex] << " = "
                 << functionResult               << endl;
        }
        else
        {
            cout << "Error on test "            << exprIndex
                 << " on expression \""         << correctExpression[exprIndex]
                 << "\". Function must return " << answers[exprIndex]
                 << " but actualy return "      << functionResult << endl;

            return 1;
        }
    }

    cout << "\nTesting incorrect expression\n";
    for(unsigned exprIndex = 0; exprIndex < INCOR_EXPR_COUNT; ++exprIndex)
    {
        try
        {
            calculator.calc(incorrectExpression[exprIndex]);
        }
        catch(std::runtime_error e)
        {
            cout << "Test " << exprIndex  << " pass\n"
                 << "Catch exception \""  << e.what() << "\"\n";
            continue;
        }

        cout << "Programm can't catch exception on test " << exprIndex
             << " on expression \"" << incorrectExpression[exprIndex]
             << "\"\n";

        return 2;
    }

    cout << "All tests pass\n";

    return 0;
}
