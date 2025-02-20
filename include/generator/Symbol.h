// Copyright 2025 solar-mist

#ifndef VLEX_GENERATOR_SYMBOL_H
#define VLEX_GENERATOR_SYMBOL_H 1

#include <string>
#include <vector>

namespace generator
{
    struct Symbol
    {
        Symbol(std::string text, std::vector<Symbol*> children, Symbol* parent)
            : text(std::move(text))
            , children(std::move(children))
            , parent(parent)
        {
        }

        std::string text;
        std::vector<Symbol*> children;
        Symbol* parent;
        bool error = false;
    };
}

#endif // VLEX_GENERATOR_SYMBOL_H