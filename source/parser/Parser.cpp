// Copyright 2025 solar-mist

#include "parser/Parser.h"

namespace parser
{
    Parser::Parser(std::vector<lexer::Token> tokens)
        : mTokens(std::move(tokens))
        , mPosition(0)
    {
    }

    void Parser::parse()
    {
        parseTokens();
    }
    
    std::vector<TokenDescriptor> Parser::getSymbols()
    {
        return mSymbols;
    }
    
    std::vector<TokenDescriptor> Parser::getKeywords()
    {
        return mKeywords;
    }
    
    std::vector<TokenDescriptor> Parser::getSpecials()
    {
        return mSpecials;
    }


    lexer::Token Parser::current() const
    {
        return mTokens[mPosition];
    }

    lexer::Token Parser::consume()
    {
        return mTokens[mPosition++];
    }

    lexer::Token Parser::peek(int offset) const
    {
        return mTokens[mPosition + offset];
    }

    void Parser::expectToken(lexer::TokenType tokenType)
    {
        if (current().getTokenType() != tokenType)
        {
            std::exit(1); // TODO: Print error
        }
    }

    void Parser::parseTokens()
    {
        expectToken(lexer::TokenType::TokensKeyword);
        consume();

        expectToken(lexer::TokenType::LeftBrace);
        consume();

        while (current().getTokenType() != lexer::TokenType::RightBrace)
        {
            switch (current().getTokenType())
            {
                case lexer::TokenType::TokensKeyword:
                    std::exit(1); // TODO: Error

                case lexer::TokenType::LeftBrace:
                    parseSymbol();
                    break;
                case lexer::TokenType::RightBrace:
                    parseSymbol();
                    break;
                case lexer::TokenType::Identifier:
                    parseIdentifier();
                    break;
                case lexer::TokenType::Symbol:
                    parseSymbol();
                    break;

                case lexer::TokenType::EndOfFile:
                case lexer::TokenType::Newline:
                    break;
            }

            if (current().getTokenType() != lexer::TokenType::RightBrace)
            {
                expectToken(lexer::TokenType::Newline);
                consume();
            }
        }
        consume();
    }

    void Parser::parseSymbol()
    {
        std::string text;
        while (current().getTokenType() == lexer::TokenType::Symbol || current().getTokenType() == lexer::TokenType::LeftBrace || current().getTokenType() == lexer::TokenType::RightBrace)
        {
            text += std::string(consume().getText());
        }

        std::optional<std::string> tokenType;
        if (current().getTokenType() == lexer::TokenType::Identifier)
        {
            tokenType = consume().getText();
        }
        mSymbols.push_back({ text, tokenType });
    }

    void Parser::parseIdentifier()
    {
        std::string text = std::string(consume().getText());

        std::optional<std::string> tokenType;
        if (current().getTokenType() == lexer::TokenType::Identifier)
        {
            tokenType = consume().getText();
        }

        if (text[0] == '_') // Special case
        {
            mSpecials.push_back({ text, tokenType });
        }
        else
        {
            mKeywords.push_back({ text, tokenType });
        }
    }
}