/*
    Parser.hpp
    ==========
        Class Parser implementation.
*/

#ifndef __CMM_PARSER_HPP
#define __CMM_PARSER_HPP

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "Parser.h"
#include "Token.h"
#include "AST.h"
#include "TokenType.h"
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

Parser::Parser(const string &inputFilePath): __root(nullptr)
{
    Lexer lexer(inputFilePath);

    for (auto nowToken = lexer.NextToken();
        /* See below */;
        nowToken = lexer.NextToken())
    {
        __tokenList.push_back(nowToken);

        if (nowToken.tokenType == TOKEN_TYPE::END_OF_FILE)
        {
            break;
        }
    }

    __tokenPtr = __tokenList.data();
    __Parse(__root);
}


////////////////////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////////////////////

Parser::~Parser()
{
    delete __root;
}


////////////////////////////////////////////////////////////////////////////////
// Invalid Token
////////////////////////////////////////////////////////////////////////////////

void Parser::__invalidToken(const Token *invalidTokenPtr)
{
    printf("Invalid token: %s in line %d\n",
        invalidTokenPtr->tokenStr.c_str(),
        invalidTokenPtr->lineNo);

    exit(1);
}


////////////////////////////////////////////////////////////////////////////////
// Match Token
////////////////////////////////////////////////////////////////////////////////

void Parser::__MatchToken(TOKEN_TYPE tarTokenType)
{
    if (__tokenPtr->tokenType == tarTokenType)
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

            TOKEN_TYPE::DECL_LIST
                |
                |---- __Decl
                |
                |---- [__Decl]
                .
                .
                .
    */

    root = new AST(TOKEN_TYPE::DECL_LIST, "DeclList", {nullptr});

    __Decl(root->subList[0]);

    while (__tokenPtr->tokenType != TOKEN_TYPE::END_OF_FILE)
    {
        root->subList.push_back(nullptr);
        __Decl(root->subList.back());
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

    if (__tokenPtr->tokenType != TOKEN_TYPE::INT &&
        __tokenPtr->tokenType != TOKEN_TYPE::VOID)
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr[1].tokenType != TOKEN_TYPE::ID)
    {
        __invalidToken(__tokenPtr + 1);
    }

    if (__tokenPtr[2].tokenType == TOKEN_TYPE::LEFT_SQUARE_BRACKET ||
        __tokenPtr[2].tokenType == TOKEN_TYPE::SEMICOLON)
    {
        __VarDecl(root);
    }
    else if (__tokenPtr[2].tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET)
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

            TOKEN_TYPE::VAR_DECL
                |
                |---- __Type
                |
                |---- TOKEN_TYPE::ID
                |
                |---- [TOKEN_TYPE::NUMBER]
    */

    root = new AST(TOKEN_TYPE::VAR_DECL, "VarDecl", {nullptr, nullptr});

    __Type(root->subList[0]);

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID)
    {
        root->subList[1] = new AST(__tokenPtr);

        __MatchToken(TOKEN_TYPE::ID);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr->tokenType == TOKEN_TYPE::LEFT_SQUARE_BRACKET)
    {
        __MatchToken(TOKEN_TYPE::LEFT_SQUARE_BRACKET);

        root->subList.push_back(new AST(__tokenPtr));

        __MatchToken(TOKEN_TYPE::NUMBER);
        __MatchToken(TOKEN_TYPE::RIGHT_SQUARE_BRACKET);
    }

    __MatchToken(TOKEN_TYPE::SEMICOLON);
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

            TOKEN_TYPE::INT | TOKEN_TYPE::VOID
    */

    if (__tokenPtr->tokenType == TOKEN_TYPE::INT ||
        __tokenPtr->tokenType == TOKEN_TYPE::VOID)
    {
        root = new AST(__tokenPtr);

        __MatchToken(__tokenPtr->tokenType);
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

            TOKEN_TYPE::FUNC_DECL
                |
                |---- __Type
                |
                |---- TOKEN_TYPE::ID
                |
                |---- __Params
                |
                |---- __CompoundStmt
    */

    root = new AST(TOKEN_TYPE::FUNC_DECL, "FuncDecl", {nullptr, nullptr, nullptr, nullptr});

    __Type(root->subList[0]);

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID)
    {
        root->subList[1] = new AST(__tokenPtr);

        __MatchToken(TOKEN_TYPE::ID);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    __MatchToken(TOKEN_TYPE::LEFT_ROUND_BRACKET);

    __Params(root->subList[2]);

    __MatchToken(TOKEN_TYPE::RIGHT_ROUND_BRACKET);

    __CompoundStmt(root->subList[3]);
}


////////////////////////////////////////////////////////////////////////////////
// ENBF: Params
////////////////////////////////////////////////////////////////////////////////

void Parser::__Params(AST *&root)
{
    /*
        EBNF:

            Params ::= ParamList
                     | void


        AST:

            __ParamList | TOKEN_TYPE::VOID
    */

    if (__tokenPtr->tokenType == TOKEN_TYPE::INT)
    {
        __ParamList(root);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::VOID)
    {
        if (__tokenPtr[1].tokenType == TOKEN_TYPE::ID)
        {
            __ParamList(root);
        }
        else
        {
            root = new AST(__tokenPtr);

            __MatchToken(TOKEN_TYPE::VOID);
        }
    }
    else
    {
        __invalidToken(__tokenPtr);
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

            TOKEN_TYPE::PARAM_LIST
                |
                |---- __Param
                |
                |---- [__Param]
                .
                .
                .
    */

    root = new AST(TOKEN_TYPE::PARAM_LIST, "ParamList", {nullptr});

    __Param(root->subList[0]);

    while (__tokenPtr->tokenType == TOKEN_TYPE::COMMA)
    {
        __MatchToken(TOKEN_TYPE::COMMA);

        root->subList.push_back(nullptr);

        __Param(root->subList.back());
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

            TOKEN_TYPE::PARAM
                |
                |---- __Type
                |
                |---- TOKEN_TYPE::ID
    */

    root = new AST(TOKEN_TYPE::PARAM, "Param", {nullptr, nullptr});

    __Type(root->subList[0]);

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID)
    {
        root->subList[1] = new AST(__tokenPtr);

        __MatchToken(TOKEN_TYPE::ID);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr->tokenType == TOKEN_TYPE::LEFT_SQUARE_BRACKET)
    {
        __MatchToken(TOKEN_TYPE::LEFT_SQUARE_BRACKET);
        __MatchToken(TOKEN_TYPE::RIGHT_SQUARE_BRACKET);
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

            TOKEN_TYPE::COMPOUND_STMT
                |
                |---- __LocalDecl
                |
                |---- __StmtList
    */

    root = new AST(TOKEN_TYPE::COMPOUND_STMT, "CompoundStmt", {nullptr, nullptr});

    __MatchToken(TOKEN_TYPE::LEFT_CURLY_BRACKET);

    __LocalDecl(root->subList[0]);

    __StmtList(root->subList[1]);

    __MatchToken(TOKEN_TYPE::RIGHT_CURLY_BRACKET);
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

            TOKEN_TYPE::LOCAL_DECL
                |
                |---- [__VarDecl]
                .
                .
                .
    */

    root = new AST(TOKEN_TYPE::LOCAL_DECL, "LocalDecl");

    while (__tokenPtr->tokenType == TOKEN_TYPE::INT ||
        __tokenPtr->tokenType == TOKEN_TYPE::VOID)
    {
        root->subList.push_back(nullptr);

        __VarDecl(root->subList.back());
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

            TOKEN_TYPE::STMT_LIST
                |
                |---- [__Stmt]
                .
                .
                .
    */

    root = new AST(TOKEN_TYPE::STMT_LIST, "StmtList");

    while (__tokenPtr->tokenType == TOKEN_TYPE::SEMICOLON ||
        __tokenPtr->tokenType == TOKEN_TYPE::ID ||
        __tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::NUMBER ||
        __tokenPtr->tokenType == TOKEN_TYPE::LEFT_CURLY_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::IF ||
        __tokenPtr->tokenType == TOKEN_TYPE::WHILE ||
        __tokenPtr->tokenType == TOKEN_TYPE::RETURN)
    {
        root->subList.push_back(nullptr);

        __Stmt(root->subList.back());
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

    if (__tokenPtr->tokenType == TOKEN_TYPE::SEMICOLON ||
        __tokenPtr->tokenType == TOKEN_TYPE::ID ||
        __tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::NUMBER)
    {
        __ExprStmt(root);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::LEFT_CURLY_BRACKET)
    {
        __CompoundStmt(root);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::IF)
    {
        __IfStmt(root);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::WHILE)
    {
        __WhileStmt(root);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::RETURN)
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

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID ||
        __tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::NUMBER)
    {
        __Expr(root);
    }

    __MatchToken(TOKEN_TYPE::SEMICOLON);
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

            TOKEN_TYPE::IF_STMT
                |
                |---- __Expr
                |
                |---- __Stmt
                |
                |---- [__Stmt]
    */

    root = new AST(TOKEN_TYPE::IF_STMT, "IfStmt", {nullptr, nullptr});

    __MatchToken(TOKEN_TYPE::IF);
    __MatchToken(TOKEN_TYPE::LEFT_ROUND_BRACKET);

    __Expr(root->subList[0]);

    __MatchToken(TOKEN_TYPE::RIGHT_ROUND_BRACKET);

    __Stmt(root->subList[1]);

    if (__tokenPtr->tokenType == TOKEN_TYPE::ELSE)
    {
        __MatchToken(TOKEN_TYPE::ELSE);

        root->subList.push_back(nullptr);

        __Stmt(root->subList[2]);
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

            TOKEN_TYPE::WHILE_STMT
                |
                |---- __Expr
                |
                |---- __Stmt
    */

    root = new AST(TOKEN_TYPE::WHILE_STMT, "WhileStmt", {nullptr, nullptr});

    __MatchToken(TOKEN_TYPE::WHILE);
    __MatchToken(TOKEN_TYPE::LEFT_ROUND_BRACKET);

    __Expr(root->subList[0]);

    __MatchToken(TOKEN_TYPE::RIGHT_ROUND_BRACKET);

    __Stmt(root->subList[1]);
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

            TOKEN_TYPE::RETURN_STMT
                |
                |---- [__Expr]
    */

    root = new AST(TOKEN_TYPE::RETURN_STMT, "ReturnStmt");

    __MatchToken(TOKEN_TYPE::RETURN);

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID ||
        __tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::NUMBER)
    {
        root->subList.push_back(nullptr);
        __Expr(root->subList.back());
    }

    __MatchToken(TOKEN_TYPE::SEMICOLON);
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

            TOKEN_TYPE::EXPR
                |
                |---- __Var
                |
                |---- __Expr

            ----------------------

            TOKEN_TYPE::EXPR
                |
                |---- __SimpleExpr
    */

    root = new AST(TOKEN_TYPE::EXPR, "Expr", {nullptr});

    if (__tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::NUMBER)
    {
        __SimpleExpr(root->subList[0]);

        return;
    }
    else if (__tokenPtr->tokenType != TOKEN_TYPE::ID)
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr[1].tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET)
    {
        __SimpleExpr(root->subList[0]);
    }
    else
    {
        auto tokenPtrBak = __tokenPtr;

        __Var(root->subList[0]);

        bool isAssignBool = __tokenPtr->tokenType == TOKEN_TYPE::ASSIGN;

        delete root->subList[0];

        __tokenPtr = tokenPtrBak;

        if (isAssignBool)
        {
            root->subList.push_back(nullptr);

            __Var(root->subList[0]);

            __MatchToken(TOKEN_TYPE::ASSIGN);

            __Expr(root->subList[1]);
        }
        else
        {
            __SimpleExpr(root->subList[0]);
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

            TOKEN_TYPE::VAR
                |
                |---- TOKEN_TYPE::ID
                |
                |---- [__Expr]
    */

    root = new AST(TOKEN_TYPE::VAR, "Var", {nullptr});

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID)
    {
        root->subList[0] = new AST(__tokenPtr);

        __MatchToken(TOKEN_TYPE::ID);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    if (__tokenPtr->tokenType == TOKEN_TYPE::LEFT_SQUARE_BRACKET)
    {
        __MatchToken(TOKEN_TYPE::LEFT_SQUARE_BRACKET);

        root->subList.push_back(nullptr);

        __Expr(root->subList[1]);

        __MatchToken(TOKEN_TYPE::RIGHT_SQUARE_BRACKET);
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

            TOKEN_TYPE::SIMPLE_EXPR
                |
                |---- __AddExpr
                |
                |---- [__RelOp]
                |
                |---- [__AddExpr]
    */

    root = new AST(TOKEN_TYPE::SIMPLE_EXPR, "SimpleExpr", {nullptr});

    __AddExpr(root->subList[0]);

    if (__tokenPtr->tokenType == TOKEN_TYPE::LESS ||
        __tokenPtr->tokenType == TOKEN_TYPE::LESS_EQUAL ||
        __tokenPtr->tokenType == TOKEN_TYPE::GREATER ||
        __tokenPtr->tokenType == TOKEN_TYPE::GREATER_EQUAL ||
        __tokenPtr->tokenType == TOKEN_TYPE::EQUAL ||
        __tokenPtr->tokenType == TOKEN_TYPE::NOT_EQUAL)
    {
        root->subList.push_back(nullptr);

        __RelOp(root->subList[1]);

        root->subList.push_back(nullptr);

        __AddExpr(root->subList[2]);
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

            TOKEN_TYPE::LESS          |
            TOKEN_TYPE::LESS_EQUAL    |
            TOKEN_TYPE::GREATER       |
            TOKEN_TYPE::GREATER_EQUAL |
            TOKEN_TYPE::EQUAL         |
            TOKEN_TYPE::NOT_EQUAL
    */

    if (__tokenPtr->tokenType == TOKEN_TYPE::LESS ||
        __tokenPtr->tokenType == TOKEN_TYPE::LESS_EQUAL ||
        __tokenPtr->tokenType == TOKEN_TYPE::GREATER ||
        __tokenPtr->tokenType == TOKEN_TYPE::GREATER_EQUAL ||
        __tokenPtr->tokenType == TOKEN_TYPE::EQUAL ||
        __tokenPtr->tokenType == TOKEN_TYPE::NOT_EQUAL)
    {
        root = new AST(__tokenPtr);

        __MatchToken(__tokenPtr->tokenType);
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

            TOKEN_TYPE::ADD_EXPR
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

    root = new AST(TOKEN_TYPE::ADD_EXPR, "AddExpr", {nullptr});

    __Term(root->subList[0]);

    while (__tokenPtr->tokenType == TOKEN_TYPE::PLUS ||
        __tokenPtr->tokenType == TOKEN_TYPE::MINUS)
    {
        root->subList.push_back(nullptr);

        __AddOp(root->subList.back());

        root->subList.push_back(nullptr);

        __Term(root->subList.back());
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

            TOKEN_TYPE::PLUS | TOKEN_TYPE::MINUS
    */

    if (__tokenPtr->tokenType == TOKEN_TYPE::PLUS ||
        __tokenPtr->tokenType == TOKEN_TYPE::MINUS)
    {
        root = new AST(__tokenPtr);

        __MatchToken(__tokenPtr->tokenType);
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

            TOKEN_TYPE::TERM
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

    root = new AST(TOKEN_TYPE::TERM, "Term", {nullptr});

    __Factor(root->subList[0]);

    while (__tokenPtr->tokenType == TOKEN_TYPE::MULTIPLY ||
        __tokenPtr->tokenType == TOKEN_TYPE::DIVIDE)
    {
        root->subList.push_back(nullptr);

        __MulOp(root->subList.back());

        root->subList.push_back(nullptr);

        __Factor(root->subList.back());
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

            TOKEN_TYPE::MULTIPLY | TOKEN_TYPE::DIVIDE
    */

    if (__tokenPtr->tokenType == TOKEN_TYPE::MULTIPLY ||
        __tokenPtr->tokenType == TOKEN_TYPE::DIVIDE)
    {
        root = new AST(__tokenPtr);

        __MatchToken(__tokenPtr->tokenType);
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

            __Expr | TOKEN_TYPE::NUMBER | __Call | __Var
    */

    if (__tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET)
    {
        __MatchToken(TOKEN_TYPE::LEFT_ROUND_BRACKET);

        __Expr(root);

        __MatchToken(TOKEN_TYPE::RIGHT_ROUND_BRACKET);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::NUMBER)
    {
        root = new AST(__tokenPtr);

        __MatchToken(__tokenPtr->tokenType);
    }
    else if (__tokenPtr->tokenType == TOKEN_TYPE::ID)
    {
        if (__tokenPtr[1].tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET)
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

            TOKEN_TYPE::CALL
                |
                |---- TOKEN_TYPE::ID
                |
                |---- [__ArgList]
    */

    root = new AST(TOKEN_TYPE::CALL, "Call", {nullptr});

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID)
    {
        root->subList[0] = new AST(__tokenPtr);

        __MatchToken(TOKEN_TYPE::ID);
    }
    else
    {
        __invalidToken(__tokenPtr);
    }

    __MatchToken(TOKEN_TYPE::LEFT_ROUND_BRACKET);

    if (__tokenPtr->tokenType == TOKEN_TYPE::ID ||
        __tokenPtr->tokenType == TOKEN_TYPE::LEFT_ROUND_BRACKET ||
        __tokenPtr->tokenType == TOKEN_TYPE::NUMBER)
    {
        root->subList.push_back(nullptr);

        __ArgList(root->subList[1]);
    }

    __MatchToken(TOKEN_TYPE::RIGHT_ROUND_BRACKET);
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

            TOKEN_TYPE::ARG_LIST
                |
                |---- __Expr
                |
                |---- [__Expr]
                .
                .
                .
    */

    root = new AST(TOKEN_TYPE::ARG_LIST, "ArgList", {nullptr});

    __Expr(root->subList[0]);

    while (__tokenPtr->tokenType == TOKEN_TYPE::COMMA)
    {
        __MatchToken(TOKEN_TYPE::COMMA);

        root->subList.push_back(nullptr);

        __Expr(root->subList.back());
    }
}


}  // End namespace CMM


#endif  // __CMM_PARSER_HPP
