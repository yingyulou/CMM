/*
    SyntaxAnalyzer.h
    ================
        Class __SyntaxAnalyzer header.
*/

#pragma once

#include <string>
#include <vector>
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __SyntaxAnalyzer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __SyntaxAnalyzer
{
    // Friend
    friend class Core;


public:

    // Constructor
    explicit __SyntaxAnalyzer(const vector<__Token> &tokenList = {});


private:

    // Attribute
    vector<__Token> __tokenList;


    // Invalid Token
    static void __invalidToken(const __Token *tokenPtr);


    // Match Token
    static void __matchToken(__TokenType tokenType, __Token *&tokenPtr);


    // ENBF: Program
    static void __Program(__AST *&root, __Token *&tokenPtr);


    // ENBF: Decl
    static void __Decl(__AST *&root, __Token *&tokenPtr);


    // ENBF: VarDecl
    static void __VarDecl(__AST *&root, __Token *&tokenPtr);


    // ENBF: Type
    static void __Type(__AST *&root, __Token *&tokenPtr);


    // ENBF: FuncDecl
    static void __FuncDecl(__AST *&root, __Token *&tokenPtr);


    // ENBF: ParamList
    static void __ParamList(__AST *&root, __Token *&tokenPtr);


    // ENBF: Param
    static void __Param(__AST *&root, __Token *&tokenPtr);


    // ENBF: LocalDecl
    static void __LocalDecl(__AST *&root, __Token *&tokenPtr);


    // ENBF: StmtList
    static void __StmtList(__AST *&root, __Token *&tokenPtr);


    // ENBF: Stmt
    static void __Stmt(__AST *&root, __Token *&tokenPtr);


    // ENBF: ExprStmt
    static void __ExprStmt(__AST *&root, __Token *&tokenPtr);


    // ENBF: IfStmt
    static void __IfStmt(__AST *&root, __Token *&tokenPtr);


    // ENBF: WhileStmt
    static void __WhileStmt(__AST *&root, __Token *&tokenPtr);


    // ENBF: ReturnStmt
    static void __ReturnStmt(__AST *&root, __Token *&tokenPtr);


    // ENBF: Expr
    static void __Expr(__AST *&root, __Token *&tokenPtr);


    // ENBF: Var
    static void __Var(__AST *&root, __Token *&tokenPtr);


    // ENBF: SimpleExpr
    static void __SimpleExpr(__AST *&root, __Token *&tokenPtr);


    // ENBF: RelOp
    static void __RelOp(__AST *&root, __Token *&tokenPtr);


    // ENBF: AddExpr
    static void __AddExpr(__AST *&root, __Token *&tokenPtr);


    // ENBF: AddOp
    static void __AddOp(__AST *&root, __Token *&tokenPtr);


    // ENBF: Term
    static void __Term(__AST *&root, __Token *&tokenPtr);


    // ENBF: MulOp
    static void __MulOp(__AST *&root, __Token *&tokenPtr);


    // ENBF: Factor
    static void __Factor(__AST *&root, __Token *&tokenPtr);


    // ENBF: Call
    static void __Call(__AST *&root, __Token *&tokenPtr);


    // ENBF: ArgList
    static void __ArgList(__AST *&root, __Token *&tokenPtr);


    // Syntax Analysis
    __AST *__syntaxAnalysis();
};


}  // End namespace CMM
