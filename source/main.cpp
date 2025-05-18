// Copyright 2025 solar-mist

#include "lexer/Token.h"
#include "lexer/Lexer.h"

#include "parser/Parser.h"

#include "generator/Generator.h"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::ofstream log("vlex.log");
        log << "vlex: incorrect input\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::ifstream inputFile = std::ifstream(inputPath);
    if (!inputFile.is_open())
    {
        std::ofstream log("vlex.log");
        log << std::format("vlex: file {} not found\n", inputPath);
        return 1;
    }

    std::stringstream buf;
    buf << inputFile.rdbuf();
    std::string text = buf.str();

    lexer::Lexer lexer(text, inputPath);
    auto tokens = lexer.lex();

    diagnostic::Diagnostics diag;
    diag.setText(text);

    parser::Parser parser(std::move(tokens), diag);
    parser.parse();

    auto generator = generator::Generator(parser.getNamespaceName(), parser.getSymbols(), parser.getKeywords(), parser.getSpecials(), parser.getComments());
    generator.generate(argv[2], argv[3]);
}