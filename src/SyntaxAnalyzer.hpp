/*
    SyntaxAnalyzer.hpp
    ==================
        Class SyntaxAnalyzer implementation.
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

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::runtime_error;
using boost::format;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

SyntaxAnalyzer::SyntaxAnalyzer(const vector<Token> &tokenList):
    __tokenList(tokenList) {}


////////////////////////////////////////////////////////////////////////////////
// Syntax Analysis
////////////////////////////////////////////////////////////////////////////////

AST *SyntaxAnalyzer::syntaxAnalysis()
{
    return __syntaxAnalysis();
}


////////////////////////////////////////////////////////////////////////////////
// Invalid Token
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__invalidToken(const Token *tokenPtr)
{
    throw runtime_error((format("Invalid token: %s in line %d") %
        tokenPtr->tokenStr()                                    %
        tokenPtr->lineNo()
    ).str());
}


////////////////////////////////////////////////////////////////////////////////
// Match Token
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__matchToken(TokenType tarTokenType, Token *&tokenPtr)
{
    if (tokenPtr->tokenType() == tarTokenType)
    {
        tokenPtr++;
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Parse
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Parse(AST *&root, Token *&tokenPtr)
{
    __Program(root, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Program
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Program(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Program ::= DeclList


        AST:

            __DeclList
    */

    __DeclList(root, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: DeclList
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__DeclList(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            DeclList ::= Decl { Decl }


        AST:

            TokenType::DeclList
                |
                |---- __Decl
                |
                |---- [__Decl]
                .
                .
                .
    */

    root = new AST(TokenType::DeclList, "DeclList", {nullptr});

    __Decl(root->subList()[0], tokenPtr);

    while (tokenPtr->tokenType() != TokenType::END)
    {
        root->subList().push_back(nullptr);
        __Decl(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Decl
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Decl(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Decl ::= VarDecl
                   | FuncDecl


        AST:

            __VarDecl | __FuncDecl
    */

    if (tokenPtr->tokenType() != TokenType::Int &&
        tokenPtr->tokenType() != TokenType::Void)
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr[1].tokenType() != TokenType::Id)
    {
        __invalidToken(tokenPtr + 1);
    }

    if (tokenPtr[2].tokenType() == TokenType::LeftSquareBracket ||
        tokenPtr[2].tokenType() == TokenType::Semicolon)
    {
        __VarDecl(root, tokenPtr);
    }
    else if (tokenPtr[2].tokenType() == TokenType::LeftRoundBracket)
    {
        __FuncDecl(root, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr + 2);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: VarDecl
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__VarDecl(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            VarDecl ::= Type ID [ '[' NUMBER ']' ] ';'


        AST:

            TokenType::VarDecl
                |
                |---- __Type
                |
                |---- TokenType::Id
                |
                |---- [TokenType::Number]
    */

    root = new AST(TokenType::VarDecl, "VarDecl", {nullptr, nullptr});

    __Type(root->subList()[0], tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[1] = new AST(tokenPtr);

        __matchToken(TokenType::Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr->tokenType() == TokenType::LeftSquareBracket)
    {
        __matchToken(TokenType::LeftSquareBracket, tokenPtr);

        root->subList().push_back(new AST(tokenPtr));

        __matchToken(TokenType::Number, tokenPtr);
        __matchToken(TokenType::RightSquareBracket, tokenPtr);
    }

    __matchToken(TokenType::Semicolon, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Type
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Type(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Type ::= int
                   | void


        AST:

            TokenType::Int | TokenType::Void
    */

    if (tokenPtr->tokenType() == TokenType::Int ||
        tokenPtr->tokenType() == TokenType::Void)
    {
        root = new AST(tokenPtr);

        __matchToken(tokenPtr->tokenType(), tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: FuncDecl
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__FuncDecl(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            FuncDecl ::= Type ID '(' Params ')' CompoundStmt


        AST:

            TokenType::FuncDecl
                |
                |---- __Type
                |
                |---- TokenType::Id
                |
                |---- __Params
                |
                |---- __CompoundStmt
    */

    root = new AST(TokenType::FuncDecl, "FuncDecl", {nullptr, nullptr, nullptr, nullptr});

    __Type(root->subList()[0], tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[1] = new AST(tokenPtr);

        __matchToken(TokenType::Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    __matchToken(TokenType::LeftRoundBracket, tokenPtr);

    __Params(root->subList()[2], tokenPtr);

    __matchToken(TokenType::RightRoundBracket, tokenPtr);

    __CompoundStmt(root->subList()[3], tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Params
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Params(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Params ::= [ ParamList ]


        AST:

            __ParamList | nullptr
    */

    if (tokenPtr->tokenType() == TokenType::Int ||
        tokenPtr->tokenType() == TokenType::Void)
    {
        __ParamList(root, tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ParamList
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__ParamList(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            ParamList ::= Param { ',' Param }


        AST:

            TokenType::ParamList
                |
                |---- __Param
                |
                |---- [__Param]
                .
                .
                .
    */

    root = new AST(TokenType::ParamList, "ParamList", {nullptr});

    __Param(root->subList()[0], tokenPtr);

    while (tokenPtr->tokenType() == TokenType::Comma)
    {
        __matchToken(TokenType::Comma, tokenPtr);

        root->subList().push_back(nullptr);

        __Param(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Param
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Param(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Param ::= Type ID [ '[' ']' ]


        AST:

            TokenType::Param
                |
                |---- __Type
                |
                |---- TokenType::Id
    */

    root = new AST(TokenType::Param, "Param", {nullptr, nullptr});

    __Type(root->subList()[0], tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[1] = new AST(tokenPtr);

        __matchToken(TokenType::Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr->tokenType() == TokenType::LeftSquareBracket)
    {
        __matchToken(TokenType::LeftSquareBracket, tokenPtr);
        __matchToken(TokenType::RightSquareBracket, tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: CompoundStmt
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__CompoundStmt(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            CompoundStmt ::= '{' LocalDecl StmtList '}'


        AST:

            TokenType::CompoundStmt
                |
                |---- __LocalDecl
                |
                |---- __StmtList
    */

    root = new AST(TokenType::CompoundStmt, "CompoundStmt", {nullptr, nullptr});

    __matchToken(TokenType::LeftCurlyBracket, tokenPtr);

    __LocalDecl(root->subList()[0], tokenPtr);

    __StmtList(root->subList()[1], tokenPtr);

    __matchToken(TokenType::RightCurlyBracket, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: LocalDecl
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__LocalDecl(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            LocalDecl ::= { VarDecl }


        AST:

            TokenType::LocalDecl
                |
                |---- [__VarDecl]
                .
                .
                .
    */

    root = new AST(TokenType::LocalDecl, "LocalDecl");

    while (tokenPtr->tokenType() == TokenType::Int ||
        tokenPtr->tokenType() == TokenType::Void)
    {
        root->subList().push_back(nullptr);

        __VarDecl(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: StmtList
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__StmtList(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            StmtList ::= { Stmt }


        AST:

            TokenType::StmtList
                |
                |---- [__Stmt]
                .
                .
                .
    */

    root = new AST(TokenType::StmtList, "StmtList");

    while (tokenPtr->tokenType() == TokenType::Semicolon     ||
        tokenPtr->tokenType() == TokenType::Id               ||
        tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        tokenPtr->tokenType() == TokenType::Number           ||
        tokenPtr->tokenType() == TokenType::LeftCurlyBracket ||
        tokenPtr->tokenType() == TokenType::If               ||
        tokenPtr->tokenType() == TokenType::While            ||
        tokenPtr->tokenType() == TokenType::Return)
    {
        root->subList().push_back(nullptr);

        __Stmt(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Stmt
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Stmt(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Stmt ::= ExprStmt
                   | CompoundStmt
                   | IfStmt
                   | WhileStmt
                   | ReturnStmt


        AST:

            __ExprStmt | __CompoundStmt | __IfStmt | __WhileStmt | __ReturnStmt
    */

    if (tokenPtr->tokenType() == TokenType::Semicolon        ||
        tokenPtr->tokenType() == TokenType::Id               ||
        tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        tokenPtr->tokenType() == TokenType::Number)
    {
        __ExprStmt(root, tokenPtr);
    }
    else if (tokenPtr->tokenType() == TokenType::LeftCurlyBracket)
    {
        __CompoundStmt(root, tokenPtr);
    }
    else if (tokenPtr->tokenType() == TokenType::If)
    {
        __IfStmt(root, tokenPtr);
    }
    else if (tokenPtr->tokenType() == TokenType::While)
    {
        __WhileStmt(root, tokenPtr);
    }
    else if (tokenPtr->tokenType() == TokenType::Return)
    {
        __ReturnStmt(root, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ExprStmt
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__ExprStmt(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            ExprStmt ::= [ Expr ] ';'


        AST:

            __Expr | nullptr
    */

    if (tokenPtr->tokenType() == TokenType::Id               ||
        tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        tokenPtr->tokenType() == TokenType::Number)
    {
        __Expr(root, tokenPtr);
    }

    __matchToken(TokenType::Semicolon, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: IfStmt
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__IfStmt(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            IfStmt ::= if '(' Expr ')' Stmt [ else Stmt ]


        AST:

            TokenType::IfStmt
                |
                |---- __Expr
                |
                |---- __Stmt
                |
                |---- [__Stmt]
    */

    root = new AST(TokenType::IfStmt, "IfStmt", {nullptr, nullptr});

    __matchToken(TokenType::If, tokenPtr);
    __matchToken(TokenType::LeftRoundBracket, tokenPtr);

    __Expr(root->subList()[0], tokenPtr);

    __matchToken(TokenType::RightRoundBracket, tokenPtr);

    __Stmt(root->subList()[1], tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Else)
    {
        __matchToken(TokenType::Else, tokenPtr);

        root->subList().push_back(nullptr);

        __Stmt(root->subList()[2], tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: WhileStmt
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__WhileStmt(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            WhileStmt ::= while '(' Expr ')' Stmt


        AST:

            TokenType::WhileStmt
                |
                |---- __Expr
                |
                |---- __Stmt
    */

    root = new AST(TokenType::WhileStmt, "WhileStmt", {nullptr, nullptr});

    __matchToken(TokenType::While, tokenPtr);
    __matchToken(TokenType::LeftRoundBracket, tokenPtr);

    __Expr(root->subList()[0], tokenPtr);

    __matchToken(TokenType::RightRoundBracket, tokenPtr);

    __Stmt(root->subList()[1], tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ReturnStmt
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__ReturnStmt(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            ReturnStmt ::= return [ Expr ] ';'


        AST:

            TokenType::ReturnStmt
                |
                |---- [__Expr]
    */

    root = new AST(TokenType::ReturnStmt, "ReturnStmt");

    __matchToken(TokenType::Return, tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Id               ||
        tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        tokenPtr->tokenType() == TokenType::Number)
    {
        root->subList().push_back(nullptr);
        __Expr(root->subList().back(), tokenPtr);
    }

    __matchToken(TokenType::Semicolon, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Expr
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Expr(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Expr ::= Var '=' Expr
                   | SimpleExpr


        AST:

            TokenType::Expr
                |
                |---- __Var
                |
                |---- __Expr

            ----------------------

            TokenType::Expr
                |
                |---- __SimpleExpr
    */

    root = new AST(TokenType::Expr, "Expr", {nullptr});

    if (tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        tokenPtr->tokenType() == TokenType::Number)
    {
        __SimpleExpr(root->subList()[0], tokenPtr);

        return;
    }
    else if (tokenPtr->tokenType() != TokenType::Id)
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr[1].tokenType() == TokenType::LeftRoundBracket)
    {
        __SimpleExpr(root->subList()[0], tokenPtr);
    }
    else
    {
        auto tokenPtrBak = tokenPtr;

        __Var(root->subList()[0], tokenPtr);

        bool isAssignBool = tokenPtr->tokenType() == TokenType::Assign;

        delete root->subList()[0];

        tokenPtr = tokenPtrBak;

        if (isAssignBool)
        {
            root->subList().push_back(nullptr);

            __Var(root->subList()[0], tokenPtr);

            __matchToken(TokenType::Assign, tokenPtr);

            __Expr(root->subList()[1], tokenPtr);
        }
        else
        {
            __SimpleExpr(root->subList()[0], tokenPtr);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Var
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Var(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Var ::= ID [ '[' Expr ']' ]


        AST:

            TokenType::Var
                |
                |---- TokenType::Id
                |
                |---- [__Expr]
    */

    root = new AST(TokenType::Var, "Var", {nullptr});

    if (tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[0] = new AST(tokenPtr);

        __matchToken(TokenType::Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    if (tokenPtr->tokenType() == TokenType::LeftSquareBracket)
    {
        __matchToken(TokenType::LeftSquareBracket, tokenPtr);

        root->subList().push_back(nullptr);

        __Expr(root->subList()[1], tokenPtr);

        __matchToken(TokenType::RightSquareBracket, tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: SimpleExpr
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__SimpleExpr(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            SimpleExpr ::= AddExpr [ RelOp AddExpr ]


        AST:

            TokenType::SimpleExpr
                |
                |---- __AddExpr
                |
                |---- [__RelOp]
                |
                |---- [__AddExpr]
    */

    root = new AST(TokenType::SimpleExpr, "SimpleExpr", {nullptr});

    __AddExpr(root->subList()[0], tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Less         ||
        tokenPtr->tokenType() == TokenType::LessEqual    ||
        tokenPtr->tokenType() == TokenType::Greater      ||
        tokenPtr->tokenType() == TokenType::GreaterEqual ||
        tokenPtr->tokenType() == TokenType::Equal        ||
        tokenPtr->tokenType() == TokenType::NotEqual)
    {
        root->subList().push_back(nullptr);

        __RelOp(root->subList()[1], tokenPtr);

        root->subList().push_back(nullptr);

        __AddExpr(root->subList()[2], tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: RelOp
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__RelOp(AST *&root, Token *&tokenPtr)
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

            TokenType::Less         |
            TokenType::LessEqual    |
            TokenType::Greater      |
            TokenType::GreaterEqual |
            TokenType::Equal        |
            TokenType::NotEqual
    */

    if (tokenPtr->tokenType() == TokenType::Less         ||
        tokenPtr->tokenType() == TokenType::LessEqual    ||
        tokenPtr->tokenType() == TokenType::Greater      ||
        tokenPtr->tokenType() == TokenType::GreaterEqual ||
        tokenPtr->tokenType() == TokenType::Equal        ||
        tokenPtr->tokenType() == TokenType::NotEqual)
    {
        root = new AST(tokenPtr);

        __matchToken(tokenPtr->tokenType(), tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: AddExpr
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__AddExpr(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            AddExpr ::= Term { AddOp Term }


        AST:

            TokenType::AddExpr
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

    root = new AST(TokenType::AddExpr, "AddExpr", {nullptr});

    __Term(root->subList()[0], tokenPtr);

    while (tokenPtr->tokenType() == TokenType::Plus ||
        tokenPtr->tokenType() == TokenType::Minus)
    {
        root->subList().push_back(nullptr);

        __AddOp(root->subList().back(), tokenPtr);

        root->subList().push_back(nullptr);

        __Term(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: AddOp
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__AddOp(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            AddOp ::= +
                    | -


        AST:

            TokenType::Plus | TokenType::Minus
    */

    if (tokenPtr->tokenType() == TokenType::Plus ||
        tokenPtr->tokenType() == TokenType::Minus)
    {
        root = new AST(tokenPtr);

        __matchToken(tokenPtr->tokenType(), tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Term
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Term(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Term ::= Factor { MulOp Factor }


        AST:

            TokenType::Term
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

    root = new AST(TokenType::Term, "Term", {nullptr});

    __Factor(root->subList()[0], tokenPtr);

    while (tokenPtr->tokenType() == TokenType::Multiply ||
        tokenPtr->tokenType() == TokenType::Divide)
    {
        root->subList().push_back(nullptr);

        __MulOp(root->subList().back(), tokenPtr);

        root->subList().push_back(nullptr);

        __Factor(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: MulOp
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__MulOp(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            MulOp ::= *
                    | /


        AST:

            TokenType::Multiply | TokenType::Divide
    */

    if (tokenPtr->tokenType() == TokenType::Multiply ||
        tokenPtr->tokenType() == TokenType::Divide)
    {
        root = new AST(tokenPtr);

        __matchToken(tokenPtr->tokenType(), tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Factor
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Factor(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Factor ::= '(' Expr ')'
                     | Var
                     | Call
                     | NUM


        AST:

            __Expr | TokenType::Number | __Call | __Var
    */

    if (tokenPtr->tokenType() == TokenType::LeftRoundBracket)
    {
        __matchToken(TokenType::LeftRoundBracket, tokenPtr);

        __Expr(root, tokenPtr);

        __matchToken(TokenType::RightRoundBracket, tokenPtr);
    }
    else if (tokenPtr->tokenType() == TokenType::Number)
    {
        root = new AST(tokenPtr);

        __matchToken(tokenPtr->tokenType(), tokenPtr);
    }
    else if (tokenPtr->tokenType() == TokenType::Id)
    {
        if (tokenPtr[1].tokenType() == TokenType::LeftRoundBracket)
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


////////////////////////////////////////////////////////////////////////////////
// ENBF: Call
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__Call(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            Call ::= ID '(' [ ArgList ] ')'


        AST:

            TokenType::Call
                |
                |---- TokenType::Id
                |
                |---- [__ArgList]
    */

    root = new AST(TokenType::Call, "Call", {nullptr});

    if (tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[0] = new AST(tokenPtr);

        __matchToken(TokenType::Id, tokenPtr);
    }
    else
    {
        __invalidToken(tokenPtr);
    }

    __matchToken(TokenType::LeftRoundBracket, tokenPtr);

    if (tokenPtr->tokenType() == TokenType::Id               ||
        tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        tokenPtr->tokenType() == TokenType::Number)
    {
        root->subList().push_back(nullptr);

        __ArgList(root->subList()[1], tokenPtr);
    }

    __matchToken(TokenType::RightRoundBracket, tokenPtr);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ArgList
////////////////////////////////////////////////////////////////////////////////

void SyntaxAnalyzer::__ArgList(AST *&root, Token *&tokenPtr)
{
    /*
        EBNF:

            ArgList ::= Expr { ',' Expr }


        AST:

            TokenType::ArgList
                |
                |---- __Expr
                |
                |---- [__Expr]
                .
                .
                .
    */

    root = new AST(TokenType::ArgList, "ArgList", {nullptr});

    __Expr(root->subList()[0], tokenPtr);

    while (tokenPtr->tokenType() == TokenType::Comma)
    {
        __matchToken(TokenType::Comma, tokenPtr);

        root->subList().push_back(nullptr);

        __Expr(root->subList().back(), tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Syntax Analysis
////////////////////////////////////////////////////////////////////////////////

AST *SyntaxAnalyzer::__syntaxAnalysis()
{
    AST *root = nullptr;

    auto tokenPtr = __tokenList.data();

    __Parse(root, tokenPtr);

    return root;
}


}  // End namespace CMM
