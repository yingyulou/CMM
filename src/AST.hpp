/*
    AST.hpp
    =======
        Struct AST implementation.
*/

#pragma once

#include <string>
#include <vector>
#include "AST.h"
#include "TokenType.h"
#include "Token.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

AST::AST(TokenType _tokenType, const string &_tokenStr,
    const vector<AST *> &_subList, int _lineNo):
    tokenType(_tokenType),
    tokenStr (_tokenStr),
    subList  (_subList),
    lineNo   (_lineNo) {}


AST::AST(const Token *tokenPtr):
    tokenType(tokenPtr->tokenType),
    tokenStr (tokenPtr->tokenStr),
    lineNo   (tokenPtr->lineNo) {}


////////////////////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////////////////////

AST::~AST()
{
    for (auto subPtr: subList)
    {
        delete subPtr;
    }
}


}  // End namespace CMM
