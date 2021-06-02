/*
    AST.h
    =====
        Struct AST header.
*/

#pragma once

#include <string>
#include <vector>
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
// struct AST
////////////////////////////////////////////////////////////////////////////////

struct AST
{
    // Attribute
    TOKEN_TYPE tokenType;
    string tokenStr;
    vector<AST *> subList;
    int lineNo;


    // Constructor
    explicit AST(TOKEN_TYPE _tokenType, const string &_tokenStr = "",
        const vector<AST *> &_subList = {}, int _lineNo = 0);

    explicit AST(const Token *tokenPtr);


    // Destructor
    ~AST();
};


}  // End namespace CMM
