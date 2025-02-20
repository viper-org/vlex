        }
        return "forgot to add new token to Token::getName()";
    }

    TokenType Token::getTokenType() const
    {
        return mTokenType;
    }


    SourceLocation Token::getStartLocation() const
    {
        return mStartLoc;
    }

    SourceLocation Token::getEndLocation() const
    {
        return mEndLoc;
    }
}