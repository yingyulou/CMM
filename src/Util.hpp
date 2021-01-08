/*
    Util.hpp
    ========
        Utility functions implementation.
*/

#ifndef __CMM_UTIL_HPP
#define __CMM_UTIL_HPP

#include <cstdio>
#include <cstdlib>
#include "Token.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Invalid Char
////////////////////////////////////////////////////////////////////////////////

void InvalidChar(char invalidChar, int lineNo)
{
    printf("Invalid char: %c in line: %d\n", invalidChar, lineNo);

    exit(1);
}


////////////////////////////////////////////////////////////////////////////////
// Invalid Token
////////////////////////////////////////////////////////////////////////////////

void InvalidToken(const Token *invalidTokenPtr)
{
    printf("Invalid token: %s in line %d\n",
        invalidTokenPtr->tokenStr.c_str(),
        invalidTokenPtr->lineNo);

    exit(1);
}


}  // End namespace CMM


#endif  // __CMM_UTIL_HPP
