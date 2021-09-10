/*
    SyntaxAnalyzer.h
    ================
        Class SyntaxAnalyzer header.
*/

#pragma once

#include <string>
#include <vector>
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


////////////////////////////////////////////////////////////////////////////////
// Class SyntaxAnalyzer
////////////////////////////////////////////////////////////////////////////////

class SyntaxAnalyzer
{
public:

    // Constructor
    explicit SyntaxAnalyzer(const vector<Token> &tokenList = {});


    // Syntax Analysis
    AST *syntaxAnalysis();


private:

    // Attribute
    vector<Token> __tokenList;


    // Invalid Token
    static void __invalidToken(const Token *tokenPtr);


    // Match Token
    static void __matchToken(TokenType tokenType, Token *&tokenPtr);


    // ENBF: Parse
    static void __Parse(AST *&root, Token *&tokenPtr);


    // ENBF: Program
    static void __Program(AST *&root, Token *&tokenPtr);


    // ENBF: DeclList
    static void __DeclList(AST *&root, Token *&tokenPtr);


    // ENBF: Decl
    static void __Decl(AST *&root, Token *&tokenPtr);


    // ENBF: VarDecl
    static void __VarDecl(AST *&root, Token *&tokenPtr);


    // ENBF: Type
    static void __Type(AST *&root, Token *&tokenPtr);


    // ENBF: FuncDecl
    static void __FuncDecl(AST *&root, Token *&tokenPtr);


    // ENBF: Params
    static void __Params(AST *&root, Token *&tokenPtr);


    // ENBF: ParamList
    static void __ParamList(AST *&root, Token *&tokenPtr);


    // ENBF: Param
    static void __Param(AST *&root, Token *&tokenPtr);


    // ENBF: CompoundStmt
    static void __CompoundStmt(AST *&root, Token *&tokenPtr);


    // ENBF: LocalDecl
    static void __LocalDecl(AST *&root, Token *&tokenPtr);


    // ENBF: StmtList
    static void __StmtList(AST *&root, Token *&tokenPtr);


    // ENBF: Stmt
    static void __Stmt(AST *&root, Token *&tokenPtr);


    // ENBF: ExprStmt
    static void __ExprStmt(AST *&root, Token *&tokenPtr);


    // ENBF: IfStmt
    static void __IfStmt(AST *&root, Token *&tokenPtr);


    // ENBF: WhileStmt
    static void __WhileStmt(AST *&root, Token *&tokenPtr);


    // ENBF: ReturnStmt
    static void __ReturnStmt(AST *&root, Token *&tokenPtr);


    // ENBF: Expr
    static void __Expr(AST *&root, Token *&tokenPtr);


    // ENBF: Var
    static void __Var(AST *&root, Token *&tokenPtr);


    // ENBF: SimpleExpr
    static void __SimpleExpr(AST *&root, Token *&tokenPtr);


    // ENBF: RelOp
    static void __RelOp(AST *&root, Token *&tokenPtr);


    // ENBF: AddExpr
    static void __AddExpr(AST *&root, Token *&tokenPtr);


    // ENBF: AddOp
    static void __AddOp(AST *&root, Token *&tokenPtr);


    // ENBF: Term
    static void __Term(AST *&root, Token *&tokenPtr);


    // ENBF: MulOp
    static void __MulOp(AST *&root, Token *&tokenPtr);


    // ENBF: Factor
    static void __Factor(AST *&root, Token *&tokenPtr);


    // ENBF: Call
    static void __Call(AST *&root, Token *&tokenPtr);


    // ENBF: ArgList
    static void __ArgList(AST *&root, Token *&tokenPtr);


    // Syntax Analysis
    AST *__syntaxAnalysis();
};


}  // End namespace CMM
