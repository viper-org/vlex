// Copyright 2025 solar-mist

#include "generator/Generator.h"

#include "templates/IdentifierParseCPP.h"
#include "templates/IntegerLiteralParseCPP.h"
#include "templates/KeywordParseCPP.h"
#include "templates/Lexer1CPP.h"
#include "templates/Lexer2CPP.h"
#include "templates/LexerH.h"
#include "templates/SourceLocationCPP.h"
#include "templates/SourceLocationH.h"
#include "templates/Token1CPP.h"
#include "templates/Token2CPP.h"
#include "templates/TokenH.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>

namespace generator
{
    Generator::Generator(std::vector<std::string> symbols, std::vector<std::string> keywords, std::vector<std::string> specials)
        : mSymbols(std::move(symbols))
        , mKeywords(std::move(keywords))
        , mSpecials(std::move(specials))
    {
        std::sort(mSymbols.begin(), mSymbols.end());
        for (auto& symbol : mSymbols)
        {
            if (symbol.size() == 1)
            {
                mSymbolNodes.push_back(std::make_unique<Symbol>(symbol, std::vector<Symbol*>(), nullptr));
            }
            else
            {
                auto it = std::find_if(mSymbolNodes.begin(), mSymbolNodes.end(), [&symbol](const auto& node) {
                    return symbol.substr(0, symbol.size() - 1) == node->text;
                });

                if (it != mSymbolNodes.end())
                {
                    auto parent = it->get();

                    mSymbolNodes.push_back(std::make_unique<Symbol>(symbol, std::vector<Symbol*>(), parent));
                    parent->children.push_back(mSymbolNodes.back().get());
                }
                else
                {
                    mSymbolNodes.push_back(std::make_unique<Symbol>(symbol, std::vector<Symbol*>(), nullptr));
                    std::string sym = symbol.substr(0, symbol.size() - 1);
                    Symbol* prev = mSymbolNodes.back().get();

                    while (!sym.empty())
                    {
                        auto it = std::find_if(mSymbolNodes.begin(), mSymbolNodes.end(), [&sym](const auto& node) {
                            return sym == node->text;
                        });
                        if (it != mSymbolNodes.end())
                        {
                            auto parent = it->get();
                            parent->children.push_back(prev);
                            prev->parent = parent;
                            break;
                        }

                        mSymbolNodes.push_back(std::make_unique<Symbol>(sym, std::vector<Symbol*>{prev}, nullptr));
                        mSymbolNodes.back()->error = true;
                        prev->parent = mSymbolNodes.back().get();
                        prev = prev->parent;
                        sym.erase(sym.end()-1);
                    }
                }
            }
        }
    }

    void Generator::generate(std::filesystem::path outsource, std::filesystem::path outinc)
    {
        if (!std::filesystem::exists(outsource))
        {
            std::filesystem::create_directories(outsource);
        }
        if (!std::filesystem::exists(outinc))
        {
            std::filesystem::create_directories(outinc);
        }

        generateSourceLocation(outsource, outinc);
        generateToken(outsource, outinc);
        generateLexer(outsource, outinc);
    }

    void Generator::generateSourceLocation(std::filesystem::path outsource, std::filesystem::path outinc)
    {
        std::ofstream sourceLocationH = std::ofstream(outinc / "SourceLocation.h");
        sourceLocationH << templates::SourceLocationH;
        sourceLocationH.close();

        std::ofstream sourceLocationCPP = std::ofstream(outsource / "SourceLocation.cpp");
        sourceLocationCPP << std::format("#include \"{}\"\n", (outinc / "SourceLocation.h").string());
        sourceLocationCPP << templates::SourceLocationCPP;
        sourceLocationCPP.close();
    }

    void Generator::generateToken(std::filesystem::path outsource, std::filesystem::path outinc)
    {
        std::ofstream tokenH = std::ofstream(outinc / "Token.h");
        tokenH << std::format(R"(#ifndef VLEX_GENERATED_LEXER_TOKEN_H
#define VLEX_GENERATED_LEXER_TOKEN_H

#include "{}"

#include <string>

namespace lexer
{{
    enum class TokenType
    {{
)", (outinc / "SourceLocation.h").string());

        for (auto keyword : mKeywords)
        {
            keyword[0] = toupper(keyword[0]);
            keyword += "Keyword";
            tokenH << std::format("\t\t{},\n", keyword);
        }
        if (!mKeywords.empty()) tokenH << "\n";
        for (auto& symbol : mSymbols)
        {
            tokenH << std::format("\t\t{},\n", symbolToName(symbol));
        }
        for (auto& special : mSpecials)
        {
            if (special == "_identifier")
            {
                tokenH << "\t\tIdentifier,\n";
            }
            else if (special == "_integer_literal")
            {
                tokenH << "\t\tIntegerLiteral,\n";
            }
            else if (special == "_string_literal")
            {
                tokenH << "\t\tStringLiteral,\n";
            }
            else
            {
                std::cerr << "Unknown special identifier " << special << "\n";
                std::exit(1);
            }
        }
        tokenH << "\n\t\tEndOfFile,\n\t\tError\n\t};\n";

        tokenH << templates::TokenH;
        tokenH.close();


        std::ofstream tokenCPP = std::ofstream(outsource / "Token.cpp");
        tokenCPP << std::format("#include \"{}\"\n", (outinc / "Token.h").string());
        tokenCPP << templates::Token1CPP;
        for (auto keyword : mKeywords)
        {
            std::string tokenType = keyword;
            tokenType[0] = toupper(keyword[0]);
            tokenType += "Keyword";
            tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"{}\";\n", tokenType, keyword);
        }
        for (auto& symbol : mSymbols)
        {
            tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"{}\";\n", symbolToName(symbol), symbol);
        }
        for (auto& special : mSpecials)
        {
            if (special == "_identifier")
            {
                tokenCPP << "\t\t\tcase TokenType::Identifier:\n\t\t\t\treturn \"an identifier\";\n";
            }
            else if (special == "_integer_literal")
            {
                tokenCPP << "\t\t\tcase TokenType::IntegerLiteral:\n\t\t\t\treturn \"integer literal\";\n";
            }
            else if (special == "_string_literal")
            {
                tokenCPP << "\t\t\tcase TokenType::StringLiteral:\n\t\t\t\treturn \"string literal\";\n";
            }
        }
        tokenCPP << "\t\t\tcase TokenType::EndOfFile:\n\t\t\t\treturn \"eof\";\n";
        tokenCPP << "\t\t\tcase TokenType::Error:\n\t\t\t\treturn \"Error\";\n";

        tokenCPP << templates::Token2CPP;
        tokenCPP.close();
    }

    void Generator::generateLexer(std::filesystem::path outsource, std::filesystem::path outinc)
    {
        std::ofstream lexerH = std::ofstream(outinc / "Lexer.h");
        lexerH << std::format(R"(#ifndef VLEX_GENERATED_LEXER_LEXER_H
#define VLEX_GENERATED_LEXER_LEXER_H 1

#include "{}"
)", (outinc / "SourceLocation.h").string());
        lexerH << templates::LexerH;
        lexerH.close();


        std::ofstream lexerCPP = std::ofstream(outsource / "Lexer.cpp");
        lexerCPP << std::format(R"(#include "{}"
#include "{}"
)", (outinc / "Lexer.h").string(), (outinc / "Token.h").string());
        lexerCPP << templates::Lexer1CPP;

        for (auto& keyword : mKeywords)
        {
            std::string tokenType = keyword;
            tokenType[0] = toupper(keyword[0]);
            tokenType += "Keyword";
            lexerCPP << std::format("\t\t{{ \"{}\", TokenType::{} }},\n", keyword, tokenType);
        }
        lexerCPP << "\t};\n";
        lexerCPP << templates::Lexer2CPP;
        generateSpecials(lexerCPP);
        
        // Generate logic for keywords
        if (std::find(mSpecials.begin(), mSpecials.end(), "_identifier") == mSpecials.end())
        {
            lexerCPP << templates::KeywordParseCPP;
        }

        generateSymbols(lexerCPP);
        lexerCPP << "\n\t}\n}";
    }


    void Generator::generateSpecials(std::ofstream& stream)
    {
        for (auto& special : mSpecials)
        {
            if (special == "_identifier")
            {
                stream << templates::IdentifierParseCPP;
            }
            else if (special == "_integer_literal")
            {
                stream << templates::IntegerLiteralParseCPP;
            }
            // TODO: String Literals
        }
    }

    void Generator::generateSymbols(std::ofstream& stream)
    {
        stream << "\n\t\tswitch (current())\n\t\t{\n";

        for (auto& node : mSymbolNodes)
        {
            if (!node->parent)
            {
                stream << std::format("\t\t\tcase '{}':\n\t\t\t\t", node->text[0]);
                if (!node->children.empty())
                {
                    std::function<void(Symbol*, int)> doOne;
                    doOne = [this, &stream, &doOne](Symbol* sym, int idx){
                        stream << std::format("if (peek({0}) == '{1}')\n\t\t\t{2}{{\n\t\t\t\t{2}", idx, sym->text[idx], std::string(idx, '\t'));
                        for (auto child : sym->children)
                        {
                            doOne(child, idx+1);
                        }
                        if (!sym->error)
                        {
                            for (int i = 0; i < idx; ++i)
                            {
                                stream << std::format("consume();\n\t\t\t\t{}", std::string(idx, '\t'));
                            }
                            stream << std::format("return Token(\"{}\", TokenType::{}, start, mSourceLocation);", sym->text, sym->error?"Error":symbolToName(sym->text));
                        }
                        stream << std::format("\n\t\t\t{0}}}\n\t\t\t{0}", std::string(idx, '\t'));
                    };
                    for (auto child : node->children)
                    {
                        doOne(child, 1);
                    }
                }
                stream << std::format("return Token(\"{}\", TokenType::{}, start, mSourceLocation);\n", node->text, node->error?"Error":symbolToName(node->text));
            }
        }

        stream << "\n\t\t\tdefault:\n\t\t\t\treturn Token(std::string(1, current()), TokenType::Error, start, mSourceLocation);\n\t\t}";
    }


    std::string Generator::symbolToName(std::string symbol)
    {
        static std::unordered_map<std::string, std::string> symbols = {
            {"+", "Plus"},
            {"++", "DoublePlus"},
            {"-", "Minus"},
            {"--", "DoubleMinus"},
            {"*", "Star"},
            {"/", "Slash"},
            {"=", "Equal"},
            {"==", "DoubleEqual"},
            {"!=", "BangEqual"},
            {"<", "LessThan"},
            {">", "GreaterThan"},
            {"<=", "LessEqual"},
            {">=", "GreaterEqual"},
            {"+=", "PlusEqual"},
            {"-=", "MinusEqual"},
            {"*=", "StarEqual"},
            {"/=", "SlashEqual"},
            {"|=", "PipeEqual"},
            {"&=", "AmpersandEqual"},
            {"->", "RightArrow"},
            {"(", "LeftParen"},
            {")", "RightParen"},
            {"{", "LeftBrace"},
            {"}", "RightBrace"},
            {"[", "LeftBracket"},
            {"]", "RightBracket"},
            {";", "Semicolon"},
            {":", "Colon"},
            {"::", "DoubleColon"},
            {",", "Comma"},
            {".", "Dot"},
            {"&", "Ampersand"},
            {"&&", "DoubleAmpersand"},
            {"@", "Asperand"},
            {"!", "Bang"},
            {"^", "Caret"},
            {"|", "Pipe"},
            {"||", "DoublePipe"},
            {"~", "Tilde"},
            {"$", "Dollar"},
            {"%", "Percent"},
            {"^=", "CaretEqual"},
            {"?", "QuestionMark"},
            {"#", "Hash"},
        };
        return symbols[symbol];
    }
}