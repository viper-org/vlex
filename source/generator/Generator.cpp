// Copyright 2025 solar-mist

#include "generator/Generator.h"

#include "parser/Parser.h"

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
    Generator::Generator(std::vector<parser::TokenDescriptor> symbols, std::vector<parser::TokenDescriptor> keywords, std::vector<parser::TokenDescriptor> specials)
        : mSymbols(std::move(symbols))
        , mKeywords(std::move(keywords))
        , mSpecials(std::move(specials))
    {
        std::sort(mSymbols.begin(), mSymbols.end(), [](auto& a, auto& b) {
            return a.syntax <=> b.syntax == std::strong_ordering::less;
        });
        for (auto& symbol : mSymbols)
        {
            if (symbol.syntax.size() == 1)
            {
                std::string tokenType;
                if (symbol.tokenType)
                {
                    tokenType = *symbol.tokenType;
                }
                else
                {
                    tokenType = symbolToName(symbol.syntax);
                }

                mSymbolNodes.push_back(std::make_unique<Symbol>(symbol.syntax, tokenType, std::vector<Symbol*>(), nullptr));
            }
            else
            {
                auto it = std::find_if(mSymbolNodes.begin(), mSymbolNodes.end(), [&symbol](const auto& node) {
                    return symbol.syntax.substr(0, symbol.syntax.size() - 1) == node->text;
                });

                if (it != mSymbolNodes.end())
                {
                    auto parent = it->get();

                    std::string tokenType;
                    if (symbol.tokenType)
                    {
                        tokenType = *symbol.tokenType;
                    }
                    else
                    {
                        tokenType = symbolToName(symbol.syntax);
                    }

                    mSymbolNodes.push_back(std::make_unique<Symbol>(symbol.syntax, tokenType, std::vector<Symbol*>(), parent));
                    parent->children.push_back(mSymbolNodes.back().get());
                }
                else
                {
                    std::string tokenType;
                    if (symbol.tokenType)
                    {
                        tokenType = *symbol.tokenType;
                    }
                    else
                    {
                        tokenType = symbolToName(symbol.syntax);
                    }
                    mSymbolNodes.push_back(std::make_unique<Symbol>(symbol.syntax, tokenType, std::vector<Symbol*>(), nullptr));
                    std::string sym = symbol.syntax.substr(0, symbol.syntax.size() - 1);
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

                        mSymbolNodes.push_back(std::make_unique<Symbol>(sym, "Error", std::vector<Symbol*>{prev}, nullptr));
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

        std::filesystem::remove(outinc / ".gitignore");
        std::filesystem::remove(outsource / ".gitignore");

        std::ofstream incGitignore = std::ofstream(outinc / ".gitignore", std::ios::app);
        incGitignore << R"(Token.h
Lexer.h
SourceLocation.h
)";
        std::ofstream srcGitignore = std::ofstream(outsource / ".gitignore", std::ios::app);
        srcGitignore << R"(Token.cpp
Lexer.cpp
SourceLocation.cpp
)";
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

        std::vector<std::string> tokenTypes;
        for (auto& keyword : mKeywords)
        {
            std::string tokenType;
            if (keyword.tokenType)
            {
                tokenType = *keyword.tokenType;
            }
            else
            {
                tokenType = keyword.syntax + "Keyword";
                tokenType[0] = toupper(keyword.syntax[0]);
            }
            if (std::find(tokenTypes.begin(), tokenTypes.end(), tokenType) == tokenTypes.end())
            {
                tokenTypes.push_back(tokenType);
            }
        }
        for (auto& symbol : mSymbols)
        {
            std::string tokenType;
            if (symbol.tokenType)
            {
                tokenType = *symbol.tokenType;
            }
            else
            {
                tokenType = symbolToName(symbol.syntax);
            }
            if (std::find(tokenTypes.begin(), tokenTypes.end(), tokenType) == tokenTypes.end())
            {
                tokenTypes.push_back(tokenType);
            }
        }
        for (auto& special : mSpecials)
        {
            std::string tokenType;
            if (special.syntax == "_identifier")
            {
                tokenType = special.tokenType?*special.tokenType:"Identifier";
            }
            else if (special.syntax == "_integer_literal")
            {
                tokenType = special.tokenType?*special.tokenType:"IntegerLiteral";
            }
            else if (special.syntax == "_string_literal")
            {
                tokenType = special.tokenType?*special.tokenType:"StringLiteral";
            }
            else
            {
                std::cerr << "Unknown special identifier " << special.syntax << "\n";
                std::exit(1);
            }

            if (std::find(tokenTypes.begin(), tokenTypes.end(), tokenType) == tokenTypes.end())
            {
                tokenTypes.push_back(tokenType);
            }
        }
        for (auto& tokenType : tokenTypes)
        {
            tokenH << std::format("\t\t{},\n", tokenType);
        }
        tokenH << "\n\t\tEndOfFile,\n\t\tError\n\t};\n";

        tokenH << templates::TokenH;
        tokenH.close();


        std::ofstream tokenCPP = std::ofstream(outsource / "Token.cpp");
        tokenCPP << std::format("#include \"{}\"\n", (outinc / "Token.h").string());
        tokenCPP << templates::Token1CPP;
        for (auto keyword : mKeywords)
        {
            std::string tokenType;
            if (keyword.tokenType)
            {
                tokenType = *keyword.tokenType;
            }
            else
            {
                tokenType = keyword.syntax;
                tokenType[0] = toupper(keyword.syntax[0]);
                tokenType += "Keyword";
            }
            tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"{}\";\n", tokenType, keyword.syntax);
        }
        for (auto& symbol : mSymbols)
        {
            std::string tokenType;
            if (symbol.tokenType)
            {
                tokenType = *symbol.tokenType;
            }
            else
            {
                tokenType = symbolToName(symbol.syntax);
            }
            tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"{}\";\n", tokenType, symbol.syntax);
        }
        for (auto& special : mSpecials)
        {
            if (special.syntax == "_identifier")
            {
                tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"an identifier\";\n", special.tokenType?*special.tokenType:"Identifier");
            }
            else if (special.syntax == "_integer_literal")
            {
                tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"integer literal\";\n", special.tokenType?*special.tokenType:"IntegerLiteral");
            }
            else if (special.syntax == "_string_literal")
            {
                tokenCPP << std::format("\t\t\tcase TokenType::{}:\n\t\t\t\treturn \"string literal\";\n", special.tokenType?*special.tokenType:"StringLiteral");
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
            std::string tokenType;
            if (keyword.tokenType)
            {
                tokenType = *keyword.tokenType;
            }
            else
            {
                tokenType = keyword.syntax;
                tokenType[0] = toupper(keyword.syntax[0]);
                tokenType += "Keyword";
            }
            lexerCPP << std::format("\t\t{{ \"{}\", TokenType::{} }},\n", keyword.syntax, tokenType);
        }
        lexerCPP << "\t};\n";
        lexerCPP << templates::Lexer2CPP;
        generateSpecials(lexerCPP);
        
        // Generate logic for keywords
        if (std::find_if(mSpecials.begin(), mSpecials.end(), [](auto& special){
            return special.syntax == "_identifier";
        }) == mSpecials.end())
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
            if (special.syntax == "_identifier")
            {
                stream << std::format(templates::IdentifierParseCPP, special.tokenType?*special.tokenType:"Identifier");
            }
            else if (special.syntax == "_integer_literal")
            {
                stream << std::format(templates::IntegerLiteralParseCPP, special.tokenType?*special.tokenType:"IntegerLiteral");
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
                            stream << std::format("return Token(\"{}\", TokenType::{}, start, mSourceLocation);", sym->text, sym->tokenType);
                        }
                        stream << std::format("\n\t\t\t{0}}}\n\t\t\t{0}", std::string(idx, '\t'));
                    };
                    for (auto child : node->children)
                    {
                        doOne(child, 1);
                    }
                }
                stream << std::format("return Token(\"{}\", TokenType::{}, start, mSourceLocation);\n", node->text, node->tokenType);
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