/*
    Constants.hpp
    =============
        Constants define.
*/

#pragma once

#include <string>
#include <unordered_map>
#include "TokenType.hpp"

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

const unordered_map<string, TokenType> KEYWORD_MAP
{
    {"void",   TokenType::Void},
    {"int",    TokenType::Int},
    {"if",     TokenType::If},
    {"else",   TokenType::Else},
    {"while",  TokenType::While},
    {"return", TokenType::Return},
};


}  // End namespace CMM
