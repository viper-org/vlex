
    class Token
    {
    public:
        Token(std::string text, TokenType tokenType, SourceLocation startLocation, SourceLocation endLocation);

        std::string_view getText() const;
        std::string_view getName() const;
        TokenType getTokenType() const;

        SourceLocation getStartLocation() const;
        SourceLocation getEndLocation() const;

    private:
        std::string mText;
        TokenType mTokenType;

        SourceLocation mStartLoc;
        SourceLocation mEndLoc;
    };
}

#endif // VLEX_GENERATED_LEXER_TOKEN_H