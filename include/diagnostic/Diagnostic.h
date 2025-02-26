// Copyright 2025 solar-mist

#ifndef VLEX_DIAGNOSTIC_DIAGNOSTIC_H
#define VLEX_DIAGNOSTIC_DIAGNOSTIC_H 1

#include <string>
#include <vector>

namespace lexer
{
    class SourceLocation;
}

namespace fmt
{
    constexpr std::string_view bold     = "\x1b[1m";
    constexpr std::string_view red      = "\x1b[31m";
    constexpr std::string_view yellow   = "\x1b[93m";
    constexpr std::string_view defaults = "\x1b[0m";
}

namespace diagnostic
{
    class Diagnostics
    {
    public:
        Diagnostics();

        void setText(std::string_view text);

        [[noreturn]] void fatalError(std::string_view message);

        void reportCompilerError(lexer::SourceLocation start, lexer::SourceLocation end, std::string_view message);

    private:
        std::string_view mText;

        int getLinePosition(int lineNumber);
    };
}

#endif // VLEX_DIAGNOSTIC_DIAGNOSTIC_H