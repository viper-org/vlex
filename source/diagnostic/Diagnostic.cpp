// Copyright 2025 solar-mist


#include "diagnostic/Diagnostic.h"

#include "lexer/SourceLocation.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

namespace diagnostic
{
    Diagnostics::Diagnostics()
    {
    }

    void Diagnostics::setText(std::string_view text)
    {
        mText = text;
    }


    void Diagnostics::fatalError(std::string_view message)
    {
        std::cerr << std::format("{}viper: {}fatal error: {}{}\n", fmt::bold, fmt::red, fmt::defaults, message);

        std::exit(EXIT_FAILURE);
    }

    void Diagnostics::reportCompilerError(lexer::SourceLocation start, lexer::SourceLocation end, std::string_view message)
    {
        int lineStart = getLinePosition(start.line-1);
        int lineEnd = getLinePosition(end.line)-1;

        end.position += 1;
        std::string before = std::string(mText.substr(lineStart, start.position - lineStart));
        std::string_view error = mText.substr(start.position, end.position - start.position);
        std::string_view after = mText.substr(end.position, lineEnd - end.position);
        std::string spacesBefore = std::string(std::to_string(start.line).length(), ' ');
        std::string spacesAfter = std::string(before.length(), ' ');

        std::for_each(before.begin(), before.end(), [](char& c){if(c == '\t')c=' ';});

        std::ofstream log("vlex.log");
        log << std::format("{}{}:{}:{} {}error: {}{}\n", fmt::bold, start.file, start.line, start.col, fmt::red, fmt::defaults, message);
        log << std::format("    {} | {}{}{}{}{}{}\n", start.line, before, fmt::bold, fmt::red, error, fmt::defaults, after);
        log << std::format("    {} | {}{}{}^{}{}\n", spacesBefore, spacesAfter, fmt::bold, fmt::red, std::string(error.length()-1, '~'), fmt::defaults);
    }


    int Diagnostics::getLinePosition(int lineNumber)
    {
        int line = 0;
        for (int i = 0; i < lineNumber; ++i)
        {
            while(mText[line] != '\n')
            {
                ++line;
            }
            ++line;
        }
        return line;
    }
}