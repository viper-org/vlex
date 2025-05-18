// Copyright 2025 solar-mist

#ifndef VLEX_TEMPLATES_CHAR_LITERAL_PARSE_CPP_H
#define VLEX_TEMPLATES_CHAR_LITERAL_PARSE_CPP_H 1

namespace templates
{
    constexpr const char* CharLiteralParseCPP = R"(        if (current() == '\'')
        {{
            consume();
            char value;
            if (current() == '\\')
            {{
                consume();
                switch(current())
                {{
                    case 'n':
                        value = '\n';
                        break;
                    case '\'':
                        value = '\'';
                        break;
                    case '\"':
                        value = '\"';
                        break;
                    case '\\':
                        value = '\\';
                        break;
                    case '0':
                        value = '\0';
                        break;
                }}
                consume();
            }}
            else
            {{
                value = consume();
            }}

            if (current() != '\'')
            {{
                return Token(std::string(1, value), TokenType::Error, start, mSourceLocation);
            }}
            return Token(std::string(1, value), TokenType::{}, start, mSourceLocation);
        }}
)";
}

#endif // VLEX_TEMPLATES_CHAR_LITERAL_PARSE_CPP_H