// Copyright 2025 solar-mist

#ifndef VLEX_LEXER_SOURCE_LOCATION_H
#define VLEX_LEXER_SOURCE_LOCATION_H 1

#include <string_view>

namespace lexer
{
    struct SourceLocation
    {
        SourceLocation() = default;
        SourceLocation(std::string_view file, unsigned int line, unsigned int col, unsigned int position);

        std::string_view file;

        unsigned int line;
        unsigned int col;
        unsigned int position;
    };
}

#endif // VLEX_LEXER_SOURCE_LOCATION_H