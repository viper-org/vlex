// Copyright 2025 solar-mist

#ifndef VLEX_GENERATOR_GENERATOR_H
#define VLEX_GENERATOR_GENERATOR_H 1

#include "generator/Symbol.h"

#include <filesystem>
#include <string>
#include <vector>

namespace generator
{
    class Generator
    {
    public:
        Generator(std::vector<std::string> symbols, std::vector<std::string> keywords, std::vector<std::string> specials);

        void generate(std::filesystem::path outsource, std::filesystem::path outinc);

    private:
        std::vector<std::string> mSymbols;
        std::vector<std::string> mKeywords;
        std::vector<std::string> mSpecials;

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