#ifndef VLEX_TEMPLATES_IDENTIFIER_PARSE_CPP_H
#define VLEX_TEMPLATES_IDENTIFIER_PARSE_CPP_H 1

namespace templates
{
    constexpr const char* IdentifierParseCPP = R"(        if (isIdentifierStart(current()))
        {
            std::string text = std::string(1, current());
            while (isIdentifier(peek(1)))
            {
                consume();
                text += current();
            }

            if (keywords.find(text) != keywords.end())
            {
                return Token(std::move(text), keywords.at(text), start, mSourceLocation);
            }
            
            return Token(std::move(text), TokenType::Identifier, start, mSourceLocation);
        }
)";
}

#endif // VLEX_TEMPLATES_IDENTIFIER_PARSE_CPP_H