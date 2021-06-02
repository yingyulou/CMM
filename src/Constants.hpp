/*
    Constants.hpp
    =============
        Constants define.
*/

#pragma once

#include <string>
#include <unordered_map>
#include "TokenType.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::unordered_map;


////////////////////////////////////////////////////////////////////////////////
// Description
////////////////////////////////////////////////////////////////////////////////

const string DESCRIPTION_STR = R"(
================================================================================

CMM
===

    CMM (C Minus Minus) Language Compiler And VM.

================================================================================

Usage)";


////////////////////////////////////////////////////////////////////////////////
// Keyword Map
////////////////////////////////////////////////////////////////////////////////

const unordered_map<string, TOKEN_TYPE> KEYWORD_MAP
{
    {"void",   TOKEN_TYPE::VOID},
    {"int",    TOKEN_TYPE::INT},
    {"if",     TOKEN_TYPE::IF},
    {"else",   TOKEN_TYPE::ELSE},
    {"while",  TOKEN_TYPE::WHILE},
    {"return", TOKEN_TYPE::RETURN},
};


}  // End namespace CMM
