// Copyright 2025 solar-mist

#include "lexer/Token.h"
#include "lexer/Lexer.h"

#include "parser/Parser.h"

#include "generator/Generator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cerr << "vlex: incorrect input\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::ifstream inputFile = std::ifstream(inputPath);

    std::stringstream buf;
    buf << inputFile.rdbuf();
    std::string text = std::move(buf).str();

    lexer::Lexer lexer(text, inputPath);
    auto tokens = lexer.lex();

    parser::Parser parser(std::move(tokens));
    parser.parse();

    auto generator = generator::Generator(parser.getSymbols(), parser.getKeywords(), parser.getSpecials());
    generator.generate(argv[2], argv[3]);
}