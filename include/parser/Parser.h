// Copyright 2025 solar-mist

#ifndef VLEX_PARSER_PARSER_H
#define VLEX_PARSER_PARSER_H 1

#include "lexer/Token.h"

#include "diagnostic/Diagnostic.h"

#include <optional>
#include <string>
#include <vector>

namespace parser
{
    struct TokenDescriptor
    {
        std::string syntax;

        std::optional<std::string> tokenType;
    };

    class Parser
    {
    public:
        Parser(std::vector<lexer::Token> tokens, diagnostic::Diagnostics& diag);

        void parse();

        std::vector<TokenDescriptor> getSymbols();
        std::vector<TokenDescriptor> getKeywords();
        std::vector<TokenDescriptor> getSpecials();

    private:
        std::vector<lexer::Token> mTokens;
        unsigned int mPosition;

        std::vector<TokenDescriptor> mSymbols;
        std::vector<TokenDescriptor> mKeywords;
        std::vector<TokenDescriptor> mSpecials;

        diagnostic::Diagnostics& mDiag;

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