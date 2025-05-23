#ifndef VLEX_TEMPLATES_TOKEN_1_CPP_H
#define VLEX_TEMPLATES_TOKEN_1_CPP_H 1

namespace templates
{
    constexpr const char* Token1CPP = R"(
namespace {}{}lexer
{{
    Token::Token(std::string text, TokenType tokenType, SourceLocation start, SourceLocation end)
        : mText(std::move(text))
        , mTokenType(tokenType)
        , mStartLoc(start)
        , mEndLoc(end)
    {{
    }}

    std::string_view Token::getText() const
    {{
        return mText;
    }}

    std::string_view Token::getName() const
    {{
        switch (mTokenType)
        {{
)";
}

#endif // VLEX_TEMPLATES_TOKEN_1_CPP_H