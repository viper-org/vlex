// Copyright 2025 solar-mist

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include <unordered_map>

namespace lexer
{
    Lexer::Lexer(std::string_view text, std::string_view fileName)
        : mText(text)
        , mFileName(fileName)
        , mSourceLocation(fileName, 1, 1, 0)
        , mPosition(0)
    {
    }

    static const std::unordered_map<std::string_view, TokenType> keywords = {
        { "tokens", TokenType::TokensKeyword },
        { "comments", TokenType::CommentsKeyword },
        { "close", TokenType::CloseKeyword }
    };


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

        if (isIdentifierStart(current()))
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

        if (std::isspace(current()))
        {
            if (current() == '\n')
                return Token("\n", TokenType::Newline, start, mSourceLocation);
            return std::nullopt;
        }

        switch (current())
        {
            case '{':
                return Token("{", TokenType::LeftBrace, start, mSourceLocation);
            case '}':
                return Token("}", TokenType::RightBrace, start, mSourceLocation);

            default:
                return Token(std::string(1, current()), TokenType::Symbol, start, mSourceLocation);
        }
    }
}