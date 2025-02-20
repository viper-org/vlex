// Copyright 2025 solar-mist

#ifndef VLEX_PARSER_PARSER_H
#define VLEX_PARSER_PARSER_H 1

#include "lexer/Token.h"

#include <string>
#include <vector>

namespace parser
{
    class Parser
    {
    public:
        Parser(std::vector<lexer::Token> tokens);

        void parse();

        std::vector<std::string> getSymbols();
        std::vector<std::string> getKeywords();
        std::vector<std::string> getSpecials();

    private:
        std::vector<lexer::Token> mTokens;
        unsigned int mPosition;

        std::vector<std::string> mSymbols;
        std::vector<std::string> mKeywords;
        std::vector<std::string> mSpecials;

        lexer::Token current() const;
        lexer::Token consume();
        lexer::Token peek(int offset) const;

        void expectToken(lexer::TokenType tokenType);

        void parseTokens();

        void parseSymbol();
        void parseIdentifier();
    };
}

#endif // VLEX_PARSER_PARSER_H