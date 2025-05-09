#ifndef VLEX_TEMPLATES_SOURCE_LOCATION_H_H
#define VLEX_TEMPLATES_SOURCE_LOCATION_H_H 1

namespace templates
{
    constexpr const char* SourceLocationH = R"(#ifndef VLEX_GENERATED_LEXER_SOURCE_LOCATION_H
#define VLEX_GENERATED_LEXER_SOURCE_LOCATION_H 1

#include <string_view>

namespace {}::lexer
{{
    struct SourceLocation
    {{
        SourceLocation() = default;
        SourceLocation(std::string_view file, unsigned int line, unsigned int col, unsigned int position);

        std::string_view file;

        unsigned int line;
        unsigned int col;
        unsigned int position;
    }};
}}

#endif // VLEX_GENERATED_LEXER_SOURCE_LOCATION_H)";
}

#endif // VLEX_TEMPLATES_SOURCE_LOCATION_H_H