// Copyright 2025 solar-mist

#ifndef VLEX_LEXER_TOKEN_H
#define VLEX_LEXER_TOKEN_H 1

#include "lexer/SourceLocation.h"

#include <string>

namespace lexer
{
    enum class TokenType
    {
        TokensKeyword,
        CommentsKeyword,
        CloseKeyword,

        LeftBrace, RightBrace,

        Identifier,

        Symbol,

        EndOfFile, Newline
    };

    class Token
    {
    public:
        Token(std::string text, TokenType tokenType, SourceLocation startLocation, SourceLocation endLocation);

        std::string_view getText() const;
        std::string_view getName() const;
        TokenType getTokenType() const;

        SourceLocation getStartLocation() const;
        SourceLocation getEndLocation() const;

    private:
        std::string mText;
        TokenType mTokenType;

        SourceLocation mStartLoc;
        SourceLocation mEndLoc;
    };
}

#endif // VLEX_LEXER_TOKEN_H