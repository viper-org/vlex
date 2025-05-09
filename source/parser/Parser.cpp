// Copyright 2025 solar-mist

#include "parser/Parser.h"

#include <format>

namespace parser
{
    Parser::Parser(std::vector<lexer::Token> tokens, diagnostic::Diagnostics& diag)
        : mTokens(std::move(tokens))
        , mPosition(0)
        , mDiag(diag)
    {
    }

    void Parser::parse()
    {
        parseTokens();
    }

    std::string Parser::getNamespaceName() const
    {
        return mNamespaceName;
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
            lexer::Token temp("", tokenType, {}, {});
            mDiag.reportCompilerError(
                current().getStartLocation(),
                current().getEndLocation(),
                std::format("Expected '{}{}{}', found '{}{}{}'",
                    fmt::bold, temp.getName(), fmt::defaults,
                    fmt::bold, current().getText(), fmt::defaults)
            );
            std::exit(1);
        }
    }

    void Parser::parseTokens()
    {
        expectToken(lexer::TokenType::TokensKeyword);
        consume();
        
        expectToken(lexer::TokenType::Identifier);
        std::string name(consume().getText());
        mNamespaceName = std::move(name);

        expectToken(lexer::TokenType::LeftBrace);
        consume();

        while (current().getTokenType() != lexer::TokenType::RightBrace)
        {
            switch (current().getTokenType())
            {
                case lexer::TokenType::TokensKeyword:
                    mDiag.reportCompilerError(
                        current().getStartLocation(),
                        current().getEndLocation(),
                        std::format("Expected token, found '{}{}{}'",
                            fmt::bold, current().getText(), fmt::defaults)
                    );
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
            if (text == "_right_brace")
            {
                mSymbols.push_back({ "}", tokenType });
            }
            else
            {
                mSpecials.push_back({ text, tokenType });
            }
        }
        else
        {
            mKeywords.push_back({ text, tokenType });
        }
    }
}