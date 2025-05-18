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

    struct CommentDescriptor
    {
        std::string open;
        std::string close;
    };

    class Parser
    {
    public:
        Parser(std::vector<lexer::Token> tokens, diagnostic::Diagnostics& diag);

        void parse();

        std::string getNamespaceName() const;
        std::vector<TokenDescriptor> getSymbols();
        std::vector<TokenDescriptor> getKeywords();
        std::vector<TokenDescriptor> getSpecials();
        std::vector<CommentDescriptor> getComments();

    private:
        std::vector<lexer::Token> mTokens;
        unsigned int mPosition;

        std::string mNamespaceName;
        std::vector<TokenDescriptor> mSymbols;
        std::vector<TokenDescriptor> mKeywords;
        std::vector<TokenDescriptor> mSpecials;
        std::vector<CommentDescriptor> mComments;

        diagnostic::Diagnostics& mDiag;

        lexer::Token current() const;
        lexer::Token consume();
        lexer::Token peek(int offset) const;

        void expectToken(lexer::TokenType tokenType);

        void parseTokens();
        void parseComments();

        void parseSymbol();
        void parseIdentifier();
    };
}

#endif // VLEX_PARSER_PARSER_H