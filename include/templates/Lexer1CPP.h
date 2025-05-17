#ifndef VLEX_TEMPLATES_LEXER_1_CPP_H
#define VLEX_TEMPLATES_LEXER_1_CPP_H 1

namespace templates
{
    constexpr const char* Lexer1CPP = R"(
#include <format>
#include <unordered_map>

namespace {}{}lexer
{{
    Lexer::Lexer(std::string_view text, std::string_view fileName)
        : mText(text)
        , mFileName(fileName)
        , mSourceLocation(fileName, 1, 1, 0)
        , mPosition(0)
    {{
    }}

    static const std::unordered_map<std::string_view, TokenType> keywords = {{
)";
}

#endif // VLEX_TEMPLATES_LEXER_1_CPP_H