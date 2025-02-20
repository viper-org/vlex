cmake_minimum_required(VERSION 3.26)

project(vlex)

set(SOURCES
    "source/main.cpp"

    "source/lexer/SourceLocation.cpp"
    "source/lexer/Token.cpp"
    "source/lexer/Lexer.cpp"

    "source/parser/Parser.cpp"

    "source/generator/Generator.cpp"
)

set(HEADERS
    "include/lexer/SourceLocation.h"
    "include/lexer/Token.h"
    "include/lexer/Lexer.h"

    "include/parser/Parser.h"

    "include/generator/Generator.h"
)

add_executable(vlex ${SOURCES} ${HEADERS})
target_include_directories(vlex PRIVATE include)
target_compile_features(vlex PRIVATE cxx_std_20)

function(vlex_generate target cfg src inc)
    add_custom_command(
        OUTPUT ${inc}/Lexer.h ${src}/Lexer.cpp ${inc}/SourceLocation.h ${src}/SourceLocation.cpp ${inc}/Token.h ${src}/Token.cpp
        PRE_BUILD
        DEPENDS ${cfg}
        COMMAND vlex ${cfg} ${src} ${inc}
    )
    target_sources(${target} PRIVATE ${inc}/Lexer.h ${src}/Lexer.cpp ${inc}/SourceLocation.h ${src}/SourceLocation.cpp ${inc}/Token.h ${src}/Token.cpp)
endfunction()