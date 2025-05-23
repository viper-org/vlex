#ifndef VLEX_TEMPLATES_LEXER_H_H
#define VLEX_TEMPLATES_LEXER_H_H 1

namespace templates
{
    constexpr const char* LexerH = R"(
#include <istream>
#include <optional>
#include <string>
#include <vector>

namespace {}{}lexer
{{
    class Token;

    class Lexer
    {{
    public:
        Lexer(std::string_view text, std::string_view fileName);

        std::vector<Token> lex();

    private:
        std::string_view mText;
        std::string_view mFileName;

        SourceLocation mSourceLocation;
        unsigned int mPosition;


        char current();
        char consume();
        char peek(int offset);

        std::optional<Token> getNextToken();


        static inline bool isIdentifierStart(char c) {{ return std::isalpha(c) || c == '_'; }}
        static inline bool isIdentifier(char c)      {{ return std::isalnum(c) || c == '_'; }}
        static inline bool isDigitSeparator(char c)  {{ return c == '_'; }}
    }};
}}

#endif // VLEX_GENERATED_LEXER_LEXER_H)";
}

#endif // VLEX_TEMPLATES_LEXER_H_H