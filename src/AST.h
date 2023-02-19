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
<<<<<<< HEAD
// Class AST
=======
// Class __AST
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __AST
{
    // Friend
    friend class __SyntaxAnalyzer;
    friend class __SemanticAnalyzer;
    friend class __CodeGenerator;


public:

    // Constructor
<<<<<<< HEAD
    explicit AST(TokenType tokenType, const string &tokenStr = "", const vector<AST *> &subList = {}, int lineNo = 0);
=======
    explicit __AST(__TokenType tokenType, const string &tokenStr = "", const vector<__AST *> &subList = {}, int lineNo = 0);
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a


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
