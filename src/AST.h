/*
    AST.h
    =====
        Struct AST header.
*/

#pragma once

#include <string>
#include <vector>
#include "TokenType.hpp"
#include "Token.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;


////////////////////////////////////////////////////////////////////////////////
// Class AST
////////////////////////////////////////////////////////////////////////////////

class AST
{
public:

    // Constructor
    explicit AST(TokenType tokenType, const string &tokenStr = "",
        const vector<AST *> &subList = {}, int lineNo = 0);


    // Constructor (With tokenPtr)
    explicit AST(const Token *tokenPtr);


    // Getter: __tokenType
    TokenType tokenType() const;


    // Getter: __tokenStr
    string &tokenStr();
    const string &tokenStr() const;


    // Getter: __subList
    vector<AST *> &subList();
    const vector<AST *> &subList() const;


    // Getter: __lineNo
    int lineNo() const;


    // Destructor
    ~AST();


private:

    // Attribute
    TokenType __tokenType;
    string __tokenStr;
    vector<AST *> __subList;
    int __lineNo;
};


}  // End namespace CMM
