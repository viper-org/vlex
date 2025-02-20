#ifndef VLEX_TEMPLATES_INTEGER_LITERAL_PARSE_CPP_H
#define VLEX_TEMPLATES_INTEGER_LITERAL_PARSE_CPP_H 1

namespace templates
{
    constexpr const char* IntegerLiteralParseCPP = R"(        if (std::isdigit(current()))
        {
            std::string text = std::string(1, current());
            if (current() == '0')
            {
                if (peek(1) == 'x') // hex
                {
                    consume();
                    text += current();

                    while (std::isxdigit(peek(1)))
                    {
                        consume();
                        text += current();
                        if (isDigitSeparator(peek(1)))
                        {
                            consume();
                        }
                    }
                }
                else if (peek(1) == 'b') // binary
                {
                    consume();
                    text += current();

                    while (peek(1) == '0' || peek(1) == '1')
                    {
                        consume();
                        text += current();
                        if (isDigitSeparator(peek(1)))
                        {
                            consume();
                        }
                    }
                }
                else // octal
                {
                    while (peek(1) >= '0' && peek(1) <= '7')
                    {
                        consume();
                        text += current();
                        if (isDigitSeparator(peek(1)))
                        {
                            consume();
                        }
                    }
                }
            }
            else // decimal
            {
                if (isDigitSeparator(peek(1)))
                {
                    consume();
                }

                while (std::isdigit(peek(1)))
                {
                    consume();
                    text += current();
                    if (isDigitSeparator(peek(1)))
                    {
                        consume();
                    }
                }
            }
            return Token(std::move(text), TokenType::IntegerLiteral, start, mSourceLocation);
        }
)";
}

#endif // VLEX_TEMPLATES_INTEGER_LITERAL_PARSE_CPP_H