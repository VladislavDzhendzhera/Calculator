#ifndef CALCULATOR_H
#define CALCULATOR_H

//#include <QtDebug>

#include <math.h>
#include <stdexcept>

class Calculator
{


    static const char FIRST_NUMBER            = '0';
    static const char LAST_NUMBER             = '9';

    static const char DOT                     = '.';

    static const char TERMINATION             = '\0';
    static const char SPACE                   = ' ';
    static const char TAB                     = '\t';

    static const char ADDITION                = '+';
    static const char SUBTRACTION             = '-';
    static const char MULTIPLICATION          = '*';
    static const char DIVISION                = '/';
    static const char POWER                   = '^';

    static const char LEFT_BRACKET            = '(';
    static const char RIGHT_BRACKET           = ')';



    const char *strExpression;
    const char *acceptableSymbols = "0123456789.+-*/^() \t\0";


    double   parseExpression(int leftPos, int rightPos);    // Парсит выражения, содержащие скобки
    double   parsePolynomial(int leftPos, int rightPos);    // Парсит многочлены
    double   parseMonomial(  int leftPos, int rightPos);    // Парсит одночлены
    double   parsePower(     int leftPos, int rightPos);    // Парсит отдельные степени чисел
    double   parseNumber(    int leftPos, int rightPos);    // Парсит отдельные числа

    void     trim(int &leftPos, int &rightPos);             // Убирает пробельные символы и нуль-терминаторы
    int      stringSize();                                  // Проверяет строку и возвращает её размер
    int      findLeftBracket( int leftPos, int rightPos);   // Ищет открывающую скобку. При появлении внутренних скобок проверяет баланс
    int      findRightBracket(int leftPos, int rightPos);   // Ищет закрывающую скобку. При появлении внутренних скобок проверяет баланс

public:
    Calculator() = default;

    double calc(const char* str);
};

#endif // CALCULATOR_H
