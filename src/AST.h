/*
    AST.h
    =====
        Struct __AST header.
*/

#pragma once

#include <string>
#include <vector>
#include "TokenType.hpp"
#include "Token.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __AST
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __AST
{
    // Friend
    friend class __SyntaxAnalyzer;
    friend class __SemanticAnalyzer;
    friend class __CodeGenerator;


public:

    // Constructor
    explicit __AST(__TokenType tokenType, const string &tokenStr = "", const vector<__AST *> &subList = {}, int lineNo = 0);


    // Constructor (With tokenPtr)
    explicit __AST(const __Token *tokenPtr);


    // Destructor
    ~__AST();


private:

    // Attribute
    __TokenType __tokenType;
    string __tokenStr;
    vector<__AST *> __subList;
    int __lineNo;
};


}  // End namespace CMM
