
namespace lexer
{
    Token::Token(std::string text, TokenType tokenType, SourceLocation start, SourceLocation end)
        : mText(std::move(text))
        , mTokenType(tokenType)
        , mStartLoc(start)
        , mEndLoc(end)
    {
    }

    std::string_view Token::getText() const
    {
        return mText;
    }

    std::string_view Token::getName() const
    {
        switch (mTokenType)
        {
