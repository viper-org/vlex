// Copyright 2025 solar-mist

#include "lexer/Token.h"

namespace lexer
{
    Token::Token(std::string text, TokenType tokenType, SourceLocation start, SourceLocation end)
        : mText(std::move(text))
        , mTokenType(tokenType)
        , mStartLoc(start)
        , mEndLoc(end)
    {
    }

    std::string_view Token::getText() const
    {
        return mText;
    }

    std::string_view Token::getName() const
    {
        switch (mTokenType)
        {
            case TokenType::TokensKeyword:
                return "tokens";
            case TokenType::LeftBrace:
                return "{";
            case TokenType::RightBrace:
                return "}";
            case TokenType::Identifier:
                return "identifier";
            case TokenType::Symbol:
                return mText;
            case TokenType::EndOfFile:
                return "eof";
            case TokenType::Newline:
                return "newline";
        }
        return "forgot to add new token to Token::getName()";
    }

    TokenType Token::getTokenType() const
    {
        return mTokenType;
    }


    SourceLocation Token::getStartLocation() const
    {
        return mStartLoc;
    }

    SourceLocation Token::getEndLocation() const
    {
        return mEndLoc;
    }
}