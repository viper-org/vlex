#ifndef VLEX_TEMPLATES_KEYWORD_PARSE_CPP_H
#define VLEX_TEMPLATES_KEYWORD_PARSE_CPP_H 1

namespace templates
{
    constexpr const char* KeywordParseCPP = R"(        if (isIdentifierStart(current()))
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
            
            return Token(std::move(text), TokenType::Error, start, mSourceLocation);
        })";
}

#endif // VLEX_TEMPLATES_KEYWORD_PARSE_CPP_H