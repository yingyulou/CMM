/*
    Token.h
    =======
        Struct Token header.
*/

#pragma once

#include <string>
#include "TokenType.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;


////////////////////////////////////////////////////////////////////////////////
// Struct Token
////////////////////////////////////////////////////////////////////////////////

struct Token
{
    // Attribute
    TokenType tokenType;
    string tokenStr;
    int lineNo;
};


}  // End namespace CMM
