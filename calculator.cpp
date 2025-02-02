#include "calculator.h"

double Calculator::calc(const char *str)
{
    strExpression = str;

    double result = parseExpression(0, stringSize());

    return result;
}

bool Calculator::isZero(double var)
{
    return var < EPSILON && var > -EPSILON;
}

// Функция работает для представления вещественных чисел типа double согласно стандарту IEEE 754
bool Calculator::isInteger(double var)
{
       // Для выполнения побитовых операций
       unsigned long long bitsForm = *reinterpret_cast<unsigned long long*>(&var);

       // Расчёт значения экспоненты по битовой маске
       int exponent = ((bitsForm >> MANTISSA_LENGTH) & EXPONENTA_MASK) - EXPONENT_OFFSET;

       if(exponent < 0)
       {
           // Целая часть равна нулю. Если число не ноль, то оно дробное
           return isZero(var);
       }
       else if(exponent > MANTISSA_LENGTH)
       {
           // Отсутствует дробная часть
           return true;
       }

       unsigned long long fractionPartMask = (1LL << (52 - exponent)) - 1;
       return !(bitsForm & fractionPartMask);
}


double Calculator::parseExpression(int leftPos, int rightPos) const
{
    trim(leftPos, rightPos);

    if(leftPos > rightPos)
        throw std::runtime_error("Empty expression!");

    if(strExpression[leftPos] == SUBTRACTION)
    {
        int nextPos = leftPos + 1;
        while(isSpace(nextPos))
        {
            ++nextPos;
        }
        if(strExpression[nextPos]  == LEFT_BRACKET &&
           strExpression[rightPos] == RIGHT_BRACKET)
        {
            bool isExoBracket = true;
            int balance = 0;

            for(int pos = nextPos + 1; pos < rightPos; ++pos)
            {
                char symbol = strExpression[pos];

                switch (symbol)
                {
                case RIGHT_BRACKET:
                    --balance;
                    break;
                case LEFT_BRACKET:
                    ++balance;
                    break;
                default:
                    break;
                }

                if(balance < 0)
                {
                    isExoBracket = false;
                    break;
                }
            }

            if(isExoBracket)
                return -parseExpression(nextPos, rightPos);
        }


    }

    bool isExoBracket = false;

    if(strExpression[leftPos]  == LEFT_BRACKET  &&
       strExpression[rightPos] == RIGHT_BRACKET)
    {
        isExoBracket = true;
        int balance = 0;
        for(int pos = leftPos + 1; pos < rightPos; ++pos)
        {
            char symbol = strExpression[pos];

            switch (symbol)
            {
            case RIGHT_BRACKET:
                --balance;
                break;
            case LEFT_BRACKET:
                ++balance;
                break;
            default:
                break;
            }

            if(balance < 0)
            {
                isExoBracket = false;
                break;
            }
        }
    }


    if(isExoBracket)
    {
        return parseExpression(leftPos + 1, rightPos - 1);
    }
    else
    {
        return parsePolynomial(leftPos, rightPos);
    }
}

double Calculator::parsePolynomial(int leftPos, int rightPos) const
{
    trim(leftPos, rightPos);

    bool meetBracket = false;

    for(int pos = rightPos; pos >= leftPos; --pos)
    {
        char symbol = strExpression[pos];
        switch (symbol)
        {
        case LEFT_BRACKET:
            throw std::runtime_error("Unexpected left bracket!");
        case RIGHT_BRACKET:

            meetBracket = true;
            pos = findLeftBracket(leftPos, pos);
            break;

        case ADDITION:
            if((pos == rightPos) || (pos == leftPos))
            {
                throw std::runtime_error("Polynomial start or finish with symbol \"+\"!");
            }
            else
            {
                if(meetBracket)
                {
                    return parseExpression(leftPos, pos - 1) + parseExpression(pos + 1, rightPos);
                }
                else
                {
                    return parseExpression(leftPos, pos - 1) + parseMonomial(pos + 1, rightPos);
                }

            }
            break;
        case SUBTRACTION:
            if(pos == rightPos)
            {
                throw std::runtime_error("Polynomial finish with symbol \"-\"!");
            }
            else
            {
                if(pos == leftPos)
                {
                    return parseMonomial(pos, rightPos);
                }

                int previousPos = pos - 1;

                while(previousPos != leftPos && isSpace(previousPos))
                {
                    --previousPos;
                }


                if (!(strExpression[previousPos] == MULTIPLICATION ||
                          strExpression[previousPos]  == DIVISION   ||
                          strExpression[previousPos]  == POWER))
                {
                    if(meetBracket)
                    {
                        return parseExpression(leftPos, pos - 1) - parseExpression(pos + 1, rightPos);
                    }
                    else
                    {
                        return parseExpression(leftPos, pos - 1) - parseMonomial(pos + 1, rightPos);
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    return parseMonomial(leftPos, rightPos);
}

double Calculator::parseMonomial(int leftPos, int rightPos) const
{
    trim(leftPos, rightPos);

    bool meetBracket = false;

    for(int pos = rightPos; pos >= leftPos; --pos)
    {
        char symbol = strExpression[pos];
        switch (symbol)
        {
        case LEFT_BRACKET:
            throw std::runtime_error("Unexpected left bracket!");
        case RIGHT_BRACKET:
            meetBracket = true;
            pos = findLeftBracket(leftPos, pos);
            break;
        case MULTIPLICATION:
            if((pos == rightPos) || (pos == leftPos))
            {
                throw std::runtime_error("Monomial start or finish with symbol \"*\"!");
            }
            else
            {
                double rightMultiplier;

                if(meetBracket)
                    rightMultiplier = parseExpression(pos + 1, rightPos);
                else
                    rightMultiplier = parsePower(pos + 1, rightPos);
                
                  
                
                
                
                return parseExpression(leftPos, pos - 1) * rightMultiplier;
                
            }
            break;
        case DIVISION:
            if((pos == rightPos) || (pos == leftPos))
                throw std::runtime_error("Monomial start or finish with symbol \"/\"!");

            else
            {
                double divisor;
                if(meetBracket)
                    divisor = parseExpression(pos + 1, rightPos);
                else
                    divisor = parsePower(pos + 1, rightPos);

                if(isZero(divisor))
                    throw std::runtime_error("Oops, division by zero!");

                return parseExpression(leftPos, pos - 1) / divisor;
            }
            break;
        default:
            break;
        }
    }

    return parsePower(leftPos, rightPos);
}

double Calculator::parsePower(int leftPos, int rightPos) const
{
    trim(leftPos, rightPos);
    bool meetBracket = false;

    for(int pos = leftPos; pos <= rightPos; ++pos)
    {
        char symbol = strExpression[pos];
        switch (symbol)
        {
        case RIGHT_BRACKET:
            throw std::runtime_error("Unexpected right bracket!");
        case LEFT_BRACKET:
            meetBracket = true;
            pos = findRightBracket(pos, rightPos);
            break;

        case POWER:
            if((pos == rightPos) || (pos == leftPos))
            {
                throw std::runtime_error("Power start or finish with symbol \"^\"!");
            }
            else
            {
                double number;

                if(meetBracket)
                    number = parseExpression(leftPos, pos - 1);
                else
                    number = parseNumber(leftPos, pos - 1);

                double power = parseExpression(pos + 1, rightPos);

                if(number < 0 && !isInteger(power))
                    throw std::runtime_error("Negative number to fractional power!");

                if(isZero(power))
                {
                    if(isZero(number))
                        throw std::runtime_error("Zero to zero power!");
                    else
                        return 1;
                }

                if(isZero(number))
                {
                    if(power < 0)
                        throw std::runtime_error("Zero to negative power!");
                    else
                        return 0;
                }

                return std::pow(number, power);
            }

        default:
            break;
        }

    }

    return parseNumber(leftPos, rightPos);
}

void Calculator::trim(int &leftPos, int &rightPos) const
{
    while(isSpace(leftPos))
                    ++leftPos;

    while(isSpace(rightPos))
                    --rightPos;
}

double Calculator::parseNumber(int leftPos, int rightPos) const
{
    int delimitetPos;
    double result = 0;
    bool delimiterWas = false;

    trim(leftPos, rightPos);

    bool negative = (strExpression[leftPos] == SUBTRACTION);

    if(negative)
    {
        ++leftPos;
        trim(leftPos, rightPos);
    }

    if(strExpression[leftPos] == DOT)
        throw std::runtime_error("Number start with delimiter!");

    for(int pos = leftPos; pos <= rightPos; ++pos)
    {
        if(strExpression[pos] >= FIRST_NUMBER && strExpression[pos] <= LAST_NUMBER)
        {
            result *= 10;
            result += static_cast<int>(strExpression[pos] - FIRST_NUMBER);

        }
        else if(strExpression[pos] == DOT)
        {
            if(delimiterWas)
            {
                throw std::runtime_error("Extra delimiter in number!");
            }
            else
            {
                delimitetPos = pos;
                delimiterWas = true;
            }
        }
        else
        {
            char errorMessage[] = "Not acceptable symbol \"_\" in number!";
            errorMessage[23] = strExpression[pos];

            throw std::runtime_error(errorMessage);
        }
    }

    if(delimiterWas)
    {
        result /= std::pow(10, rightPos - delimitetPos);
    }

    if(negative)
        return -result;

    return result;
}

int Calculator::stringSize() const
{
    int pos = 0;
    while(strExpression[pos] != TERMINATION)
    {
        bool accept = false;
        int acceptableSymbolIndex = 0;
        do
        {
            if(strExpression[pos] == ACCEPTABLE_SYMBOLS[acceptableSymbolIndex])
            {
                accept = true;
                break;
            }
            ++acceptableSymbolIndex;

        }while(ACCEPTABLE_SYMBOLS[acceptableSymbolIndex] != TERMINATION);

        if(!accept)
        {
            char errorMessage[] = "Not acceptable symbol \"_\"!";
            errorMessage[23] = strExpression[pos];
            throw std::runtime_error(errorMessage);
        }

        ++pos;
    }

    return pos;

}

int Calculator::findLeftBracket(int leftPos, int rightPos) const
{

    int balance = 0;
    for(int pos = rightPos; pos >= leftPos; --pos)
    {
        char symbol = strExpression[pos];

        switch (symbol)
        {
        case RIGHT_BRACKET:
            ++balance;
            break;
        case LEFT_BRACKET:
            --balance;
            break;
        default:
            break;
        }

        if(balance == 0)
            return pos;
    }

    throw std::runtime_error("Can't find left bracket!");
}

int Calculator::findRightBracket(int leftPos, int rightPos) const
{
    int balance = 0;
    for(int pos = leftPos; pos <= rightPos; ++pos)
    {
        char symbol = strExpression[pos];

        switch (symbol)
        {
        case RIGHT_BRACKET:
            ++balance;
            break;
        case LEFT_BRACKET:
            --balance;
            break;
        default:
            break;
        }

        if(balance == 0)
            return pos;
    }

    throw std::runtime_error("Can't find right bracket!");
}

bool Calculator::isSpace(int pos) const
{
    return ((strExpression[pos] == SPACE)       ||
            (strExpression[pos] == TERMINATION) ||
            (strExpression[pos] == TAB));
}

