#include "calculator.h"

double Calculator::calc(const char *str)
{
    strExpression = str;

    double result = parseExpression(0, stringSize());

    return result;
}

double Calculator::parseExpression(int leftPos, int rightPos)
{
    trim(leftPos, rightPos);

    if(strExpression[leftPos] == SUBTRACTION)
    {
        int nextPos = leftPos + 1;
        while(strExpression[nextPos] == SPACE ||
              strExpression[nextPos] == TAB)
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

double Calculator::parsePolynomial(int leftPos, int rightPos)
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
                throw std::runtime_error("Polynomial can't start or finish with symbol \"+\"!");
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
                throw std::runtime_error("Polynomial can't finish with symbol \"-\"!");
            }
            else
            {
                if(pos == leftPos)
                {
                    return parseMonomial(pos, rightPos);
                }

                int previousPos = pos - 1;

                while(previousPos != leftPos &&
                     (strExpression[previousPos] == SPACE ||
                      strExpression[previousPos] == TAB))
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

double Calculator::parseMonomial(int leftPos, int rightPos)
{

    trim(leftPos, rightPos);

    bool meetBracket = false;

    for(int pos = rightPos; pos >= leftPos; --pos)
    {
        switch (strExpression[pos])
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
                throw std::runtime_error("Monomial can't start or finish with symbol \"*\"!");
            }
            else
            {
                if(meetBracket)
                {
                    return parseExpression(leftPos, pos - 1) * parseExpression(pos + 1, rightPos);
                }
                else
                {
                    return parseExpression(leftPos, pos - 1) * parsePower(pos + 1, rightPos);
                }
            }
            break;
        case DIVISION:
            if((pos == rightPos) || (pos == leftPos))
            {
                throw std::runtime_error("Monomial can't start or finish with symbol \"/\"!");
            }
            else
            {
                if(meetBracket)
                {
                    return parseExpression(leftPos, pos - 1) / parseExpression(pos + 1, rightPos);
                }
                else
                {
                    return parseExpression(leftPos, pos - 1) / parsePower(pos + 1, rightPos);
                }
            }
            break;
        default:
            break;
        }
    }

    return parsePower(leftPos, rightPos);

}

double Calculator::parsePower(int leftPos, int rightPos)
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
                throw std::runtime_error("Power can't start or finish with symbol \"^\"!");
            }
            else
            {
                if(meetBracket)
                {
                    return std::pow(parseExpression(leftPos, pos - 1), parseExpression(pos + 1, rightPos));
                }
                else
                {
                    return std::pow(parseNumber(leftPos, pos - 1), parseExpression(pos + 1, rightPos));
                }

            }
            break;

        default:
            break;
        }

    }

    return parseNumber(leftPos, rightPos);
}

void Calculator::trim(int &leftPos, int &rightPos)
{
    while((strExpression[leftPos] == SPACE)                   ||
          (strExpression[leftPos] == TERMINATION)             ||
          (strExpression[leftPos] == TAB))
                                    ++leftPos;

    while((strExpression[rightPos] == SPACE)                  ||
          (strExpression[rightPos] == TERMINATION)            ||
          (strExpression[rightPos] == TAB))
                                    --rightPos;
}

double Calculator::parseNumber(int leftPos, int rightPos)
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
                throw std::runtime_error("Extra delimiter!");
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

int Calculator::stringSize()
{
    int pos = 0;
    while(strExpression[pos] != TERMINATION)
    {
        bool accept = false;
        int acceptableSymbolIndex = 0;
        do
        {
            if(strExpression[pos] == acceptableSymbols[acceptableSymbolIndex])
            {
                accept = true;
                break;
            }
            ++acceptableSymbolIndex;

        }while(acceptableSymbols[acceptableSymbolIndex] != TERMINATION);

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

int Calculator::findLeftBracket(int leftPos, int rightPos)
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

int Calculator::findRightBracket(int leftPos, int rightPos)
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

