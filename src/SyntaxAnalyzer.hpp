/*
    SyntaxAnalyzer.hpp
    ==================
        Class __SyntaxAnalyzer implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <boost/format.hpp>
#include "SyntaxAnalyzer.h"
#include "Token.h"
#include "AST.h"
#include "TokenType.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::runtime_error;
using boost::format;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__SyntaxAnalyzer::__SyntaxAnalyzer(const vector<__Token> &tokenList):
    __tokenList(tokenList) {}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid Token
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__invalidToken(const __Token *tokenPtr)
{
    throw runtime_error((format("Invalid token: %s in line %d") %
        tokenPtr->__tokenStr                                    %
        tokenPtr->__lineNo
    ).str());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Match Token
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__matchToken(__TokenType tar__TokenType, __Token *&tokenPtr)
{
    if (tokenPtr->__tokenType == tar__TokenType)
    {
        tokenPtr++;
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Program
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Program(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Program ::= Decl { Decl }


        AST:

            __TokenType::__Program
            |
            |---- __Decl
            |
            |---- [__Decl]
            .
            .
            .
    */

    root = new __AST(__TokenType::__Program, "Program", {nullptr});

    __Decl(root->__subList[0], tokenPtr);

    while (tokenPtr->__tokenType != __TokenType::__End)
    {
        root->__subList.push_back(nullptr);

        __Decl(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Decl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Decl(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Decl ::= VarDecl
                   | FuncDecl


        AST:

            __VarDecl | __FuncDecl
    */

    if (tokenPtr->__tokenType != __TokenType::__Int && tokenPtr->__tokenType != __TokenType::__Void)
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr[1].__tokenType != __TokenType::__Id)
    {
        __invalidToken(tokenPtr + 1);
    }

    if (tokenPtr[2].__tokenType == __TokenType::__LeftSquareBracket || tokenPtr[2].__tokenType == __TokenType::__Semicolon)
    {
        __VarDecl(root, tokenPtr);
    }
    else if (tokenPtr[2].__tokenType == __TokenType::__LeftRoundBracket)
    {
        __FuncDecl(root, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr + 2);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: VarDecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__VarDecl(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            VarDecl ::= Type ID [ '[' NUM ']' ] ';'


        AST:

            __TokenType::__VarDecl
            |
            |---- __Type
            |
            |---- __TokenType::__Id
            |
            |---- [__TokenType::__Number]
    */

    root = new __AST(__TokenType::__VarDecl, "VarDecl", {nullptr, nullptr, nullptr});

    __Type(root->__subList[0], tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Id)
    {
        root->__subList[1] = new __AST(tokenPtr);

        __matchToken(__TokenType::__Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr->__tokenType == __TokenType::__LeftSquareBracket)
    {
        __matchToken(__TokenType::__LeftSquareBracket, tokenPtr);

        root->__subList[2] = new __AST(tokenPtr);

        __matchToken(__TokenType::__Number, tokenPtr);
        __matchToken(__TokenType::__RightSquareBracket, tokenPtr);
    }

    __matchToken(__TokenType::__Semicolon, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Type
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Type(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Type ::= int
                   | void


        AST:

            __TokenType::__Int | __TokenType::__Void
    */

    if (tokenPtr->__tokenType == __TokenType::__Int || tokenPtr->__tokenType == __TokenType::__Void)
    {
        root = new __AST(tokenPtr);

        __matchToken(tokenPtr->__tokenType, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: FuncDecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__FuncDecl(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            FuncDecl ::= Type ID '(' [ ParamList ] ')' '{' LocalDecl StmtList '}'


        AST:

            __TokenType::__FuncDecl
            |
            |---- __Type
            |
            |---- __TokenType::__Id
            |
            |---- [__ParamList]
            |
            |---- __LocalDecl
            |
            |---- __StmtList
    */

    root = new __AST(__TokenType::__FuncDecl, "FuncDecl", {nullptr, nullptr, nullptr, nullptr, nullptr});

    __Type(root->__subList[0], tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Id)
    {
        root->__subList[1] = new __AST(tokenPtr);

        __matchToken(__TokenType::__Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    __matchToken(__TokenType::__LeftRoundBracket, tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Int || tokenPtr->__tokenType == __TokenType::__Void)
    {
        __ParamList(root->__subList[2], tokenPtr);
    }

    __matchToken(__TokenType::__RightRoundBracket, tokenPtr);

    __matchToken(__TokenType::__LeftCurlyBracket, tokenPtr);

    __LocalDecl(root->__subList[3], tokenPtr);

    __StmtList(root->__subList[4], tokenPtr);

    __matchToken(__TokenType::__RightCurlyBracket, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: ParamList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__ParamList(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            ParamList ::= Param { ',' Param }


        AST:

            __TokenType::__ParamList
            |
            |---- __Param
            |
            |---- [__Param]
            .
            .
            .
    */

    root = new __AST(__TokenType::__ParamList, "ParamList", {nullptr});

    __Param(root->__subList[0], tokenPtr);

    while (tokenPtr->__tokenType == __TokenType::__Comma)
    {
        __matchToken(__TokenType::__Comma, tokenPtr);

        root->__subList.push_back(nullptr);

        __Param(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Param
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Param(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Param ::= Type ID [ '[' ']' ]


        AST:

            __TokenType::__Param
            |
            |---- __Type
            |
            |---- __TokenType::__Id
    */

    root = new __AST(__TokenType::__Param, "Param", {nullptr, nullptr});

    __Type(root->__subList[0], tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Id)
    {
        root->__subList[1] = new __AST(tokenPtr);

        __matchToken(__TokenType::__Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr->__tokenType == __TokenType::__LeftSquareBracket)
    {
        __matchToken(__TokenType::__LeftSquareBracket, tokenPtr);
        __matchToken(__TokenType::__RightSquareBracket, tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: LocalDecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__LocalDecl(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            LocalDecl ::= { VarDecl }


        AST:

            __TokenType::__LocalDecl
            |
            |---- [__VarDecl]
            .
            .
            .
    */

    root = new __AST(__TokenType::__LocalDecl, "LocalDecl");

    while (tokenPtr->__tokenType == __TokenType::__Int || tokenPtr->__tokenType == __TokenType::__Void)
    {
        root->__subList.push_back(nullptr);

        __VarDecl(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: StmtList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__StmtList(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            StmtList ::= { Stmt }


        AST:

            __TokenType::__StmtList
            |
            |---- [__Stmt]
            .
            .
            .
    */

    root = new __AST(__TokenType::__StmtList, "StmtList");

    while (tokenPtr->__tokenType == __TokenType::__Semicolon     ||
        tokenPtr->__tokenType == __TokenType::__Id               ||
        tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
        tokenPtr->__tokenType == __TokenType::__Number           ||
        tokenPtr->__tokenType == __TokenType::__LeftCurlyBracket ||
        tokenPtr->__tokenType == __TokenType::__If               ||
        tokenPtr->__tokenType == __TokenType::__While            ||
        tokenPtr->__tokenType == __TokenType::__Return)
    {
        root->__subList.push_back(nullptr);

        __Stmt(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Stmt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Stmt(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Stmt ::= ExprStmt
                   | IfStmt
                   | WhileStmt
                   | ReturnStmt


        AST:

            __ExprStmt | __IfStmt | __WhileStmt | __ReturnStmt
    */

    if (tokenPtr->__tokenType == __TokenType::__Semicolon        ||
        tokenPtr->__tokenType == __TokenType::__Id               ||
        tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
        tokenPtr->__tokenType == __TokenType::__Number)
    {
        __ExprStmt(root, tokenPtr);
    }
    else if (tokenPtr->__tokenType == __TokenType::__If)
    {
        __IfStmt(root, tokenPtr);
    }
    else if (tokenPtr->__tokenType == __TokenType::__While)
    {
        __WhileStmt(root, tokenPtr);
    }
    else if (tokenPtr->__tokenType == __TokenType::__Return)
    {
        __ReturnStmt(root, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: ExprStmt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__ExprStmt(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            ExprStmt ::= [ Expr ] ';'


        AST:

            [__Expr]
    */

    if (tokenPtr->__tokenType == __TokenType::__Id               ||
        tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
        tokenPtr->__tokenType == __TokenType::__Number)
    {
        __Expr(root, tokenPtr);
    }

    __matchToken(__TokenType::__Semicolon, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: IfStmt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__IfStmt(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            IfStmt ::= if '(' Expr ')' '{' StmtList '}' [ else '{' StmtList '}' ]


        AST:

            __TokenType::__IfStmt
            |
            |---- __Expr
            |
            |---- __StmtList
            |
            |---- [__StmtList]
    */

    root = new __AST(__TokenType::__IfStmt, "IfStmt", {nullptr, nullptr, nullptr});

    __matchToken(__TokenType::__If, tokenPtr);

    __matchToken(__TokenType::__LeftRoundBracket, tokenPtr);

    __Expr(root->__subList[0], tokenPtr);

    __matchToken(__TokenType::__RightRoundBracket, tokenPtr);

    __matchToken(__TokenType::__LeftCurlyBracket, tokenPtr);

    __StmtList(root->__subList[1], tokenPtr);

    __matchToken(__TokenType::__RightCurlyBracket, tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Else)
    {
        __matchToken(__TokenType::__Else, tokenPtr);

        __StmtList(root->__subList[2], tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: WhileStmt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__WhileStmt(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            WhileStmt ::= while '(' Expr ')' '{' StmtList '}'


        AST:

            __TokenType::__WhileStmt
            |
            |---- __Expr
            |
            |---- __StmtList
    */

    root = new __AST(__TokenType::__WhileStmt, "WhileStmt", {nullptr, nullptr});

    __matchToken(__TokenType::__While, tokenPtr);

    __matchToken(__TokenType::__LeftRoundBracket, tokenPtr);

    __Expr(root->__subList[0], tokenPtr);

    __matchToken(__TokenType::__RightRoundBracket, tokenPtr);

    __matchToken(__TokenType::__LeftCurlyBracket, tokenPtr);

    __StmtList(root->__subList[1], tokenPtr);

    __matchToken(__TokenType::__RightCurlyBracket, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: ReturnStmt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__ReturnStmt(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            ReturnStmt ::= return [ Expr ] ';'


        AST:

            __TokenType::__ReturnStmt
            |
            |---- [__Expr]
    */

    root = new __AST(__TokenType::__ReturnStmt, "ReturnStmt", {nullptr});

    __matchToken(__TokenType::__Return, tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Id               ||
        tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
        tokenPtr->__tokenType == __TokenType::__Number)
    {
        __Expr(root->__subList[0], tokenPtr);
    }

    __matchToken(__TokenType::__Semicolon, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Expr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Expr(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Expr ::= Var '=' Expr
                   | SimpleExpr


        AST:

            __TokenType::__Expr
            |
            |---- __Var
            |
            |---- __Expr

            ----------------------

            __TokenType::__Expr
            |
            |---- __SimpleExpr
    */

    root = new __AST(__TokenType::__Expr, "Expr", {nullptr, nullptr});

    if (tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
        tokenPtr->__tokenType == __TokenType::__Number)
    {
        __SimpleExpr(root->__subList[0], tokenPtr);

        return;
    }
    else if (tokenPtr->__tokenType != __TokenType::__Id)
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr[1].__tokenType == __TokenType::__LeftRoundBracket)
    {
        __SimpleExpr(root->__subList[0], tokenPtr);
    }
    else
    {
        auto tokenPtrBak = tokenPtr;

        __Var(root->__subList[0], tokenPtr);

        bool isAssignBool = tokenPtr->__tokenType == __TokenType::__Assign;

        delete root->__subList[0];

        tokenPtr = tokenPtrBak;

        if (isAssignBool)
        {
            __Var(root->__subList[0], tokenPtr);

            __matchToken(__TokenType::__Assign, tokenPtr);

            __Expr(root->__subList[1], tokenPtr);
        }
        else
        {
            __SimpleExpr(root->__subList[0], tokenPtr);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Var
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Var(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Var ::= ID [ '[' Expr ']' ]


        AST:

            __TokenType::__Var
            |
            |---- __TokenType::__Id
            |
            |---- [__Expr]
    */

    root = new __AST(__TokenType::__Var, "Var", {nullptr, nullptr});

    if (tokenPtr->__tokenType == __TokenType::__Id)
    {
        root->__subList[0] = new __AST(tokenPtr);

        __matchToken(__TokenType::__Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr->__tokenType == __TokenType::__LeftSquareBracket)
    {
        __matchToken(__TokenType::__LeftSquareBracket, tokenPtr);

        __Expr(root->__subList[1], tokenPtr);

        __matchToken(__TokenType::__RightSquareBracket, tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: SimpleExpr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__SimpleExpr(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            SimpleExpr ::= AddExpr [ RelOp AddExpr ]


        AST:

            __TokenType::__SimpleExpr
            |
            |---- __AddExpr
            |
            |---- [__RelOp]
            |
            |---- [__AddExpr]
    */

    root = new __AST(__TokenType::__SimpleExpr, "SimpleExpr", {nullptr, nullptr, nullptr});

    __AddExpr(root->__subList[0], tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Less         ||
        tokenPtr->__tokenType == __TokenType::__LessEqual    ||
        tokenPtr->__tokenType == __TokenType::__Greater      ||
        tokenPtr->__tokenType == __TokenType::__GreaterEqual ||
        tokenPtr->__tokenType == __TokenType::__Equal        ||
        tokenPtr->__tokenType == __TokenType::__NotEqual)
    {
        __RelOp(root->__subList[1], tokenPtr);

        __AddExpr(root->__subList[2], tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: RelOp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__RelOp(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            RelOp ::= <
                    | <=
                    | >
                    | >=
                    | ==
                    | !=


        AST:

            __TokenType::__Less         |
            __TokenType::__LessEqual    |
            __TokenType::__Greater      |
            __TokenType::__GreaterEqual |
            __TokenType::__Equal        |
            __TokenType::__NotEqual
    */

    if (tokenPtr->__tokenType == __TokenType::__Less         ||
        tokenPtr->__tokenType == __TokenType::__LessEqual    ||
        tokenPtr->__tokenType == __TokenType::__Greater      ||
        tokenPtr->__tokenType == __TokenType::__GreaterEqual ||
        tokenPtr->__tokenType == __TokenType::__Equal        ||
        tokenPtr->__tokenType == __TokenType::__NotEqual)
    {
        root = new __AST(tokenPtr);

        __matchToken(tokenPtr->__tokenType, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: AddExpr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__AddExpr(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            AddExpr ::= Term { AddOp Term }


        AST:

            __TokenType::__AddExpr
            |
            |---- __Term
            |
            |---- [__AddOp]
            |
            |---- [__Term]
            .
            .
            .
    */

    root = new __AST(__TokenType::__AddExpr, "AddExpr", {nullptr});

    __Term(root->__subList[0], tokenPtr);

    while (tokenPtr->__tokenType == __TokenType::__Plus || tokenPtr->__tokenType == __TokenType::__Minus)
    {
        root->__subList.push_back(nullptr);

        __AddOp(root->__subList.back(), tokenPtr);

        root->__subList.push_back(nullptr);

        __Term(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: AddOp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__AddOp(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            AddOp ::= +
                    | -


        AST:

            __TokenType::__Plus | __TokenType::__Minus
    */

    if (tokenPtr->__tokenType == __TokenType::__Plus || tokenPtr->__tokenType == __TokenType::__Minus)
    {
        root = new __AST(tokenPtr);

        __matchToken(tokenPtr->__tokenType, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Term
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Term(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Term ::= Factor { MulOp Factor }


        AST:

            __TokenType::__Term
            |
            |---- __Factor
            |
            |---- [__MulOp]
            |
            |---- [__Factor]
            .
            .
            .
    */

    root = new __AST(__TokenType::__Term, "Term", {nullptr});

    __Factor(root->__subList[0], tokenPtr);

    while (tokenPtr->__tokenType == __TokenType::__Multiply || tokenPtr->__tokenType == __TokenType::__Divide)
    {
        root->__subList.push_back(nullptr);

        __MulOp(root->__subList.back(), tokenPtr);

        root->__subList.push_back(nullptr);

        __Factor(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: MulOp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__MulOp(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            MulOp ::= *
                    | /


        AST:

            __TokenType::__Multiply | __TokenType::__Divide
    */

    if (tokenPtr->__tokenType == __TokenType::__Multiply || tokenPtr->__tokenType == __TokenType::__Divide)
    {
        root = new __AST(tokenPtr);

        __matchToken(tokenPtr->__tokenType, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Factor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Factor(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Factor ::= '(' Expr ')'
                     | NUM
                     | Call
                     | Var


        AST:

            __Expr | __TokenType::__Number | __Call | __Var
    */

    if (tokenPtr->__tokenType == __TokenType::__LeftRoundBracket)
    {
        __matchToken(__TokenType::__LeftRoundBracket, tokenPtr);

        __Expr(root, tokenPtr);

        __matchToken(__TokenType::__RightRoundBracket, tokenPtr);
    }
    else if (tokenPtr->__tokenType == __TokenType::__Number)
    {
        root = new __AST(tokenPtr);

        __matchToken(tokenPtr->__tokenType, tokenPtr);
    }
    else if (tokenPtr->__tokenType == __TokenType::__Id)
    {
        if (tokenPtr[1].__tokenType == __TokenType::__LeftRoundBracket)
        {
            __Call(root, tokenPtr);
        }
        else
        {
            __Var(root, tokenPtr);
        }
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: Call
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__Call(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            Call ::= ID '(' [ ArgList ] ')'


        AST:

            __TokenType::__Call
            |
            |---- __TokenType::__Id
            |
            |---- [__ArgList]
    */

    root = new __AST(__TokenType::__Call, "Call", {nullptr, nullptr});

    if (tokenPtr->__tokenType == __TokenType::__Id)
    {
        root->__subList[0] = new __AST(tokenPtr);

        __matchToken(__TokenType::__Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    __matchToken(__TokenType::__LeftRoundBracket, tokenPtr);

    if (tokenPtr->__tokenType == __TokenType::__Id               ||
        tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
        tokenPtr->__tokenType == __TokenType::__Number)
    {
        __ArgList(root->__subList[1], tokenPtr);
    }

    __matchToken(__TokenType::__RightRoundBracket, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENBF: ArgList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SyntaxAnalyzer::__ArgList(__AST *&root, __Token *&tokenPtr)
{
    /*
        EBNF:

            ArgList ::= Expr { ',' Expr }


        AST:

            __TokenType::__ArgList
            |
            |---- __Expr
            |
            |---- [__Expr]
            .
            .
            .
    */

    root = new __AST(__TokenType::__ArgList, "ArgList", {nullptr});

    __Expr(root->__subList[0], tokenPtr);

    while (tokenPtr->__tokenType == __TokenType::__Comma)
    {
        __matchToken(__TokenType::__Comma, tokenPtr);

        root->__subList.push_back(nullptr);

        __Expr(root->__subList.back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Syntax Analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__AST *__SyntaxAnalyzer::__syntaxAnalysis()
{
    __AST *root = nullptr;

    auto tokenPtr = __tokenList.data();

    __Program(root, tokenPtr);

    return root;
}


}  // End namespace CMM
