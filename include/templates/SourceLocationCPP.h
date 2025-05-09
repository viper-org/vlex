#ifndef VLEX_TEMPLATES_SOURCE_LOCATION_CPP_H
#define VLEX_TEMPLATES_SOURCE_LOCATION_CPP_H 1

namespace templates
{
    constexpr const char* SourceLocationCPP = R"(
namespace {}::lexer
{{
    SourceLocation::SourceLocation(std::string_view file, unsigned int line, unsigned int col, unsigned int position)
        : file(file)
        , line(line)
        , col(col)
        , position(position)
    {{
    }}
}})";
}

#endif // VLEX_TEMPLATES_SOURCE_LOCATION_CPP_H