#ifndef VLEX_TEMPLATES_LEXER_2_CPP_H
#define VLEX_TEMPLATES_LEXER_2_CPP_H 1

namespace templates
{
    constexpr const char* Lexer2CPP = R"(

    std::vector<Token> Lexer::lex()
    {
        std::vector<Token> ret;

        while (mPosition < mText.length())
        {
            auto token = getNextToken();
            if (token)
            {
                ret.push_back(token.value());
            }
            consume();
        }
        ret.push_back(Token("", TokenType::EndOfFile, mSourceLocation, mSourceLocation));

        return ret;
    }


    char Lexer::current()
    {
        return mText[mPosition];
    }

    char Lexer::consume()
    {
        char c = current();
        mSourceLocation.col += 1;
        if (mText[mPosition++] == '\n')
        {
            mSourceLocation.col = 1;
            mSourceLocation.line += 1;
        }
        mSourceLocation.position += 1;
        return c;
    }

    char Lexer::peek(int offset)
    {
        return mText[mPosition + offset];
    }

    std::optional<Token> Lexer::getNextToken()
    {
        SourceLocation start = mSourceLocation;
)";
}

#endif // VLEX_TEMPLATES_LEXER_2_CPP_H