#ifndef VLEX_TEMPLATES_TOKEN_2_CPP_H
#define VLEX_TEMPLATES_TOKEN_2_CPP_H 1

namespace templates
{
    constexpr const char* Token2CPP = R"(        }
        return "forgot to add new token to Token::getName";
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
})";
}

#endif // VLEX_TEMPLATES_TOKEN_2_CPP_H