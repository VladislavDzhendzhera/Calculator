#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <math.h>
#include <stdexcept>

class Calculator
{


    static const char   FIRST_NUMBER            = '0';
    static const char   LAST_NUMBER             = '9';

    static const char   DOT                     = '.';

    static const char   TERMINATION             = '\0';
    static const char   SPACE                   = ' ';
    static const char   TAB                     = '\t';

    static const char   ADDITION                = '+';
    static const char   SUBTRACTION             = '-';
    static const char   MULTIPLICATION          = '*';
    static const char   DIVISION                = '/';
    static const char   POWER                   = '^';

    static const char   LEFT_BRACKET            = '(';
    static const char   RIGHT_BRACKET           = ')';

    inline static const char *ACCEPTABLE_SYMBOLS = "0123456789.+-*/^() \t\0";

    inline static const double EPSILON          = 1e-11;

    // Константы для определения дробности числа по его битовому представлению
    static const int     EXPONENTA_LENGTH        = 11;
    static const int     EXPONENT_OFFSET         = 1023;
    static const int     MANTISSA_LENGTH         = 52;

    static constexpr int EXPONENTA_MASK = (1LL << EXPONENTA_LENGTH) - 1;

    const char *strExpression;

    double   parseExpression(int leftPos, int rightPos) const;    // Парсит выражения, которые могут находиться во внешних скобках
    double   parsePolynomial(int leftPos, int rightPos) const;    // Парсит многочлены
    double   parseMonomial(  int leftPos, int rightPos) const;    // Парсит одночлены
    double   parsePower(     int leftPos, int rightPos) const;    // Парсит отдельные степени чисел
    double   parseNumber(    int leftPos, int rightPos) const;    // Парсит отдельные числа

    void     trim(int &leftPos, int &rightPos) const;             // Убирает пробельные символы и нуль-терминаторы
    int      stringSize() const;                                  // Проверяет строку и возвращает её размер
    int      findLeftBracket( int leftPos, int rightPos) const;   // Ищет открывающую скобку. При появлении внутренних скобок проверяет баланс
    int      findRightBracket(int leftPos, int rightPos) const;   // Ищет закрывающую скобку. При появлении внутренних скобок проверяет баланс

    inline bool isSpace(int pos) const;                           // Проверяет, является ли символ пробельным

    static bool isInteger(double var);                            // Проверяет, является ли число целым. Использует битовое представление числа



public:


    double calc(const char* str);   // Вычисляет значение заданного выражения
    static bool isZero(double var); // Проверяет равенство числа нулю. Число считается равным нулю, если отклонение от ноля ниже заданной погрешности

};

#endif // CALCULATOR_H
