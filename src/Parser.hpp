/*
    Parser.hpp
    ==========
        Class Parser implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "Parser.h"
#include "Token.h"
#include "AST.h"
#include "TokenType.hpp"
#include "Lexer.h"

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

Parser::Parser(const string &inputFilePath):
    __root     (nullptr),
    __tokenList(__getTokenList(inputFilePath)),
    __tokenPtr (__tokenList.data())
{
    __Parse(__root);
}


////////////////////////////////////////////////////////////////////////////////
// Getter: __root
////////////////////////////////////////////////////////////////////////////////

AST *Parser::root() const
{
    return __root;
}


////////////////////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////////////////////

Parser::~Parser()
{
    delete __root;
}


////////////////////////////////////////////////////////////////////////////////
// Get Token List
////////////////////////////////////////////////////////////////////////////////

vector<Token> Parser::__getTokenList(const string &inputFilePath)
{
    vector<Token> tokenList;

    Lexer lexer(inputFilePath);

    for (auto nowToken = lexer.nextToken();
        /* See below */;
        nowToken = lexer.nextToken())
    {
        tokenList.push_back(nowToken);

        if (nowToken.tokenType() == TokenType::EndOfFile)
        {
            break;
        }
    }

    return tokenList;
}


////////////////////////////////////////////////////////////////////////////////
// Invalid Token
////////////////////////////////////////////////////////////////////////////////

void Parser::__invalidToken(const Token *invalidTokenPtr)
{
    printf("Invalid token: %s in line %d\n",
        invalidTokenPtr->tokenStr().c_str(),
        invalidTokenPtr->lineNo());

    exit(1);
}


////////////////////////////////////////////////////////////////////////////////
// Match Token
////////////////////////////////////////////////////////////////////////////////

void Parser::__matchToken(TokenType tarTokenType)
{
    if (__tokenPtr->tokenType() == tarTokenType)
    {
        __tokenPtr++;
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Parse
////////////////////////////////////////////////////////////////////////////////

void Parser::__Parse(AST *&root)
{
    __Program(root);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Program
////////////////////////////////////////////////////////////////////////////////

void Parser::__Program(AST *&root)
{
    /*
        EBNF:

            Program ::= DeclList


        AST:

            __DeclList
    */

    __DeclList(root);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: DeclList
////////////////////////////////////////////////////////////////////////////////

void Parser::__DeclList(AST *&root)
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

    __root = new AST(TokenType::DeclList, "DeclList", {nullptr});

    __Decl(__root->subList()[0]);

    while (__tokenPtr->tokenType() != TokenType::EndOfFile)
    {
        __root->subList().push_back(nullptr);
        __Decl(__root->subList().back());
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Decl
////////////////////////////////////////////////////////////////////////////////

void Parser::__Decl(AST *&root)
{
    /*
        EBNF:

            Decl ::= VarDecl
                   | FuncDecl


        AST:

            __VarDecl | __FuncDecl
    */

    if (__tokenPtr->tokenType() != TokenType::Int &&
        __tokenPtr->tokenType() != TokenType::Void)
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr[1].tokenType() != TokenType::Id)
    {
        __invalidToken(__tokenPtr + 1);
    }

    if (__tokenPtr[2].tokenType() == TokenType::LeftSquareBracket ||
        __tokenPtr[2].tokenType() == TokenType::Semicolon)
    {
        __VarDecl(root);
    }
    else if (__tokenPtr[2].tokenType() == TokenType::LeftRoundBracket)
    {
        __FuncDecl(root);
    }
    else
    {
        __invalidToken(__tokenPtr + 2);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: VarDecl
////////////////////////////////////////////////////////////////////////////////

void Parser::__VarDecl(AST *&root)
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

    __Type(root->subList()[0]);

    if (__tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[1] = new AST(__tokenPtr);

        __matchToken(TokenType::Id);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr->tokenType() == TokenType::LeftSquareBracket)
    {
        __matchToken(TokenType::LeftSquareBracket);

        root->subList().push_back(new AST(__tokenPtr));

        __matchToken(TokenType::Number);
        __matchToken(TokenType::RightSquareBracket);
    }

    __matchToken(TokenType::Semicolon);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Type
////////////////////////////////////////////////////////////////////////////////

void Parser::__Type(AST *&root)
{
    /*
        EBNF:

            Type ::= int
                   | void


        AST:

            TokenType::Int | TokenType::Void
    */

    if (__tokenPtr->tokenType() == TokenType::Int ||
        __tokenPtr->tokenType() == TokenType::Void)
    {
        root = new AST(__tokenPtr);

        __matchToken(__tokenPtr->tokenType());
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: FuncDecl
////////////////////////////////////////////////////////////////////////////////

void Parser::__FuncDecl(AST *&root)
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

    __Type(root->subList()[0]);

    if (__tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[1] = new AST(__tokenPtr);

        __matchToken(TokenType::Id);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    __matchToken(TokenType::LeftRoundBracket);

    __Params(root->subList()[2]);

    __matchToken(TokenType::RightRoundBracket);

    __CompoundStmt(root->subList()[3]);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Params
////////////////////////////////////////////////////////////////////////////////

void Parser::__Params(AST *&root)
{
    /*
        EBNF:

            Params ::= [ ParamList ]


        AST:

            __ParamList | nullptr
    */

    if (__tokenPtr->tokenType() == TokenType::Int ||
        __tokenPtr->tokenType() == TokenType::Void)
    {
        __ParamList(root);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ParamList
////////////////////////////////////////////////////////////////////////////////

void Parser::__ParamList(AST *&root)
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

    __Param(root->subList()[0]);

    while (__tokenPtr->tokenType() == TokenType::Comma)
    {
        __matchToken(TokenType::Comma);

        root->subList().push_back(nullptr);

        __Param(root->subList().back());
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Param
////////////////////////////////////////////////////////////////////////////////

void Parser::__Param(AST *&root)
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

    __Type(root->subList()[0]);

    if (__tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[1] = new AST(__tokenPtr);

        __matchToken(TokenType::Id);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr->tokenType() == TokenType::LeftSquareBracket)
    {
        __matchToken(TokenType::LeftSquareBracket);
        __matchToken(TokenType::RightSquareBracket);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: CompoundStmt
////////////////////////////////////////////////////////////////////////////////

void Parser::__CompoundStmt(AST *&root)
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

    __matchToken(TokenType::LeftCurlyBracket);

    __LocalDecl(root->subList()[0]);

    __StmtList(root->subList()[1]);

    __matchToken(TokenType::RightCurlyBracket);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: LocalDecl
////////////////////////////////////////////////////////////////////////////////

void Parser::__LocalDecl(AST *&root)
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

    while (__tokenPtr->tokenType() == TokenType::Int ||
        __tokenPtr->tokenType() == TokenType::Void)
    {
        root->subList().push_back(nullptr);

        __VarDecl(root->subList().back());
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: StmtList
////////////////////////////////////////////////////////////////////////////////

void Parser::__StmtList(AST *&root)
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

    while (__tokenPtr->tokenType() == TokenType::Semicolon     ||
        __tokenPtr->tokenType() == TokenType::Id               ||
        __tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        __tokenPtr->tokenType() == TokenType::Number           ||
        __tokenPtr->tokenType() == TokenType::LeftCurlyBracket ||
        __tokenPtr->tokenType() == TokenType::If               ||
        __tokenPtr->tokenType() == TokenType::While            ||
        __tokenPtr->tokenType() == TokenType::Return)
    {
        root->subList().push_back(nullptr);

        __Stmt(root->subList().back());
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Stmt
////////////////////////////////////////////////////////////////////////////////

void Parser::__Stmt(AST *&root)
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

    if (__tokenPtr->tokenType() == TokenType::Semicolon        ||
        __tokenPtr->tokenType() == TokenType::Id               ||
        __tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        __tokenPtr->tokenType() == TokenType::Number)
    {
        __ExprStmt(root);
    }
    else if (__tokenPtr->tokenType() == TokenType::LeftCurlyBracket)
    {
        __CompoundStmt(root);
    }
    else if (__tokenPtr->tokenType() == TokenType::If)
    {
        __IfStmt(root);
    }
    else if (__tokenPtr->tokenType() == TokenType::While)
    {
        __WhileStmt(root);
    }
    else if (__tokenPtr->tokenType() == TokenType::Return)
    {
        __ReturnStmt(root);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ExprStmt
////////////////////////////////////////////////////////////////////////////////

void Parser::__ExprStmt(AST *&root)
{
    /*
        EBNF:

            ExprStmt ::= [ Expr ] ';'


        AST:

            __Expr | nullptr
    */

    if (__tokenPtr->tokenType() == TokenType::Id               ||
        __tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        __tokenPtr->tokenType() == TokenType::Number)
    {
        __Expr(root);
    }

    __matchToken(TokenType::Semicolon);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: IfStmt
////////////////////////////////////////////////////////////////////////////////

void Parser::__IfStmt(AST *&root)
{
    /*
        EBNF:

            IfStmt ::= if '(' Expr ')' Stmt [ else Stmt ]


        AST:

            TokenType::If_STMT
                |
                |---- __Expr
                |
                |---- __Stmt
                |
                |---- [__Stmt]
    */

    root = new AST(TokenType::IfStmt, "IfStmt", {nullptr, nullptr});

    __matchToken(TokenType::If);
    __matchToken(TokenType::LeftRoundBracket);

    __Expr(root->subList()[0]);

    __matchToken(TokenType::RightRoundBracket);

    __Stmt(root->subList()[1]);

    if (__tokenPtr->tokenType() == TokenType::Else)
    {
        __matchToken(TokenType::Else);

        root->subList().push_back(nullptr);

        __Stmt(root->subList()[2]);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: WhileStmt
////////////////////////////////////////////////////////////////////////////////

void Parser::__WhileStmt(AST *&root)
{
    /*
        EBNF:

            WhileStmt ::= while '(' Expr ')' Stmt


        AST:

            TokenType::While_STMT
                |
                |---- __Expr
                |
                |---- __Stmt
    */

    root = new AST(TokenType::WhileStmt, "WhileStmt", {nullptr, nullptr});

    __matchToken(TokenType::While);
    __matchToken(TokenType::LeftRoundBracket);

    __Expr(root->subList()[0]);

    __matchToken(TokenType::RightRoundBracket);

    __Stmt(root->subList()[1]);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ReturnStmt
////////////////////////////////////////////////////////////////////////////////

void Parser::__ReturnStmt(AST *&root)
{
    /*
        EBNF:

            ReturnStmt ::= return [ Expr ] ';'


        AST:

            TokenType::Return_STMT
                |
                |---- [__Expr]
    */

    root = new AST(TokenType::ReturnStmt, "ReturnStmt");

    __matchToken(TokenType::Return);

    if (__tokenPtr->tokenType() == TokenType::Id               ||
        __tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        __tokenPtr->tokenType() == TokenType::Number)
    {
        root->subList().push_back(nullptr);
        __Expr(root->subList().back());
    }

    __matchToken(TokenType::Semicolon);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Expr
////////////////////////////////////////////////////////////////////////////////

void Parser::__Expr(AST *&root)
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

    if (__tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        __tokenPtr->tokenType() == TokenType::Number)
    {
        __SimpleExpr(root->subList()[0]);

        return;
    }
    else if (__tokenPtr->tokenType() != TokenType::Id)
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr[1].tokenType() == TokenType::LeftRoundBracket)
    {
        __SimpleExpr(root->subList()[0]);
    }
    else
    {
        auto tokenPtrBak = __tokenPtr;

        __Var(root->subList()[0]);

        bool isAssignBool = __tokenPtr->tokenType() == TokenType::Assign;

        delete root->subList()[0];

        __tokenPtr = tokenPtrBak;

        if (isAssignBool)
        {
            root->subList().push_back(nullptr);

            __Var(root->subList()[0]);

            __matchToken(TokenType::Assign);

            __Expr(root->subList()[1]);
        }
        else
        {
            __SimpleExpr(root->subList()[0]);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Var
////////////////////////////////////////////////////////////////////////////////

void Parser::__Var(AST *&root)
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

    if (__tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[0] = new AST(__tokenPtr);

        __matchToken(TokenType::Id);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr->tokenType() == TokenType::LeftSquareBracket)
    {
        __matchToken(TokenType::LeftSquareBracket);

        root->subList().push_back(nullptr);

        __Expr(root->subList()[1]);

        __matchToken(TokenType::RightSquareBracket);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: SimpleExpr
////////////////////////////////////////////////////////////////////////////////

void Parser::__SimpleExpr(AST *&root)
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

    __AddExpr(root->subList()[0]);

    if (__tokenPtr->tokenType() == TokenType::Less         ||
        __tokenPtr->tokenType() == TokenType::LessEqual    ||
        __tokenPtr->tokenType() == TokenType::Greater      ||
        __tokenPtr->tokenType() == TokenType::GreaterEqual ||
        __tokenPtr->tokenType() == TokenType::Equal        ||
        __tokenPtr->tokenType() == TokenType::NotEqual)
    {
        root->subList().push_back(nullptr);

        __RelOp(root->subList()[1]);

        root->subList().push_back(nullptr);

        __AddExpr(root->subList()[2]);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: RelOp
////////////////////////////////////////////////////////////////////////////////

void Parser::__RelOp(AST *&root)
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

            TokenType::Less          |
            TokenType::Less_EQUAL    |
            TokenType::Greater       |
            TokenType::Greater_EQUAL |
            TokenType::Equal         |
            TokenType::NotEqual
    */

    if (__tokenPtr->tokenType() == TokenType::Less         ||
        __tokenPtr->tokenType() == TokenType::LessEqual    ||
        __tokenPtr->tokenType() == TokenType::Greater      ||
        __tokenPtr->tokenType() == TokenType::GreaterEqual ||
        __tokenPtr->tokenType() == TokenType::Equal        ||
        __tokenPtr->tokenType() == TokenType::NotEqual)
    {
        root = new AST(__tokenPtr);

        __matchToken(__tokenPtr->tokenType());
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: AddExpr
////////////////////////////////////////////////////////////////////////////////

void Parser::__AddExpr(AST *&root)
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

    __Term(root->subList()[0]);

    while (__tokenPtr->tokenType() == TokenType::Plus ||
        __tokenPtr->tokenType() == TokenType::Minus)
    {
        root->subList().push_back(nullptr);

        __AddOp(root->subList().back());

        root->subList().push_back(nullptr);

        __Term(root->subList().back());
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: AddOp
////////////////////////////////////////////////////////////////////////////////

void Parser::__AddOp(AST *&root)
{
    /*
        EBNF:

            AddOp ::= +
                    | -


        AST:

            TokenType::Plus | TokenType::Minus
    */

    if (__tokenPtr->tokenType() == TokenType::Plus ||
        __tokenPtr->tokenType() == TokenType::Minus)
    {
        root = new AST(__tokenPtr);

        __matchToken(__tokenPtr->tokenType());
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Term
////////////////////////////////////////////////////////////////////////////////

void Parser::__Term(AST *&root)
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

    __Factor(root->subList()[0]);

    while (__tokenPtr->tokenType() == TokenType::Multiply ||
        __tokenPtr->tokenType() == TokenType::Divide)
    {
        root->subList().push_back(nullptr);

        __MulOp(root->subList().back());

        root->subList().push_back(nullptr);

        __Factor(root->subList().back());
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: MulOp
////////////////////////////////////////////////////////////////////////////////

void Parser::__MulOp(AST *&root)
{
    /*
        EBNF:

            MulOp ::= *
                    | /


        AST:

            TokenType::Multiply | TokenType::Divide
    */

    if (__tokenPtr->tokenType() == TokenType::Multiply ||
        __tokenPtr->tokenType() == TokenType::Divide)
    {
        root = new AST(__tokenPtr);

        __matchToken(__tokenPtr->tokenType());
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Factor
////////////////////////////////////////////////////////////////////////////////

void Parser::__Factor(AST *&root)
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

    if (__tokenPtr->tokenType() == TokenType::LeftRoundBracket)
    {
        __matchToken(TokenType::LeftRoundBracket);

        __Expr(root);

        __matchToken(TokenType::RightRoundBracket);
    }
    else if (__tokenPtr->tokenType() == TokenType::Number)
    {
        root = new AST(__tokenPtr);

        __matchToken(__tokenPtr->tokenType());
    }
    else if (__tokenPtr->tokenType() == TokenType::Id)
    {
        if (__tokenPtr[1].tokenType() == TokenType::LeftRoundBracket)
        {
            __Call(root);
        }
        else
        {
            __Var(root);
        }
    }
    else
    {
        __invalidToken(__tokenPtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Call
////////////////////////////////////////////////////////////////////////////////

void Parser::__Call(AST *&root)
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

    if (__tokenPtr->tokenType() == TokenType::Id)
    {
        root->subList()[0] = new AST(__tokenPtr);

        __matchToken(TokenType::Id);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    __matchToken(TokenType::LeftRoundBracket);

    if (__tokenPtr->tokenType() == TokenType::Id               ||
        __tokenPtr->tokenType() == TokenType::LeftRoundBracket ||
        __tokenPtr->tokenType() == TokenType::Number)
    {
        root->subList().push_back(nullptr);

        __ArgList(root->subList()[1]);
    }

    __matchToken(TokenType::RightRoundBracket);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: ArgList
////////////////////////////////////////////////////////////////////////////////

void Parser::__ArgList(AST *&root)
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

    __Expr(root->subList()[0]);

    while (__tokenPtr->tokenType() == TokenType::Comma)
    {
        __matchToken(TokenType::Comma);

        root->subList().push_back(nullptr);

        __Expr(root->subList().back());
    }
}


}  // End namespace CMM
