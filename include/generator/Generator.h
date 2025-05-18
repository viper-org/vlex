// Copyright 2025 solar-mist

#ifndef VLEX_GENERATOR_GENERATOR_H
#define VLEX_GENERATOR_GENERATOR_H 1

#include "generator/Symbol.h"

#include <filesystem>
#include <string>
#include <vector>

namespace parser
{
    struct TokenDescriptor;
    struct CommentDescriptor;
}

namespace generator
{
    class Generator
    {
    public:
        Generator(std::string namespaceName, std::vector<parser::TokenDescriptor> symbols, std::vector<parser::TokenDescriptor> keywords, std::vector<parser::TokenDescriptor> specials, std::vector<parser::CommentDescriptor> comments);

        void generate(std::filesystem::path outsource, std::filesystem::path outinc);

    private:
        std::string mNamespaceName;
        std::vector<parser::TokenDescriptor> mSymbols;
        std::vector<parser::TokenDescriptor> mKeywords;
        std::vector<parser::TokenDescriptor> mSpecials;
        std::vector<parser::CommentDescriptor> mComments;

        std::vector<std::unique_ptr<Symbol> > mSymbolNodes;

        void generateSourceLocation(std::filesystem::path outsource, std::filesystem::path outinc);
        void generateToken(std::filesystem::path outsource, std::filesystem::path outinc);
        void generateLexer(std::filesystem::path outsource, std::filesystem::path outinc);


        void generateSpecials(std::ofstream& stream);
        void generateSymbols(std::ofstream& stream);


        std::string symbolToName(std::string symbol);
    };
}

#endif // VLEX_GENERATOR_GENERATOR_H