/*
    Parser.h
    ========
        Class Parser header.
*/

#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "AST.h"
#include "TokenType.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;


////////////////////////////////////////////////////////////////////////////////
// Class Parser
////////////////////////////////////////////////////////////////////////////////

class Parser
{
public:

    // Constructor
    explicit Parser(const string &inputFilePath);


    // Getter: __root
    AST *root() const;


    // Destructor
    ~Parser();


private:

    // Attribute
    vector<Token> __tokenList;
    Token *__tokenPtr;
    AST *__root;


    // Invalid Token
    static void __invalidToken(const Token *invalidTokenPtr);


    // Match Token
    void __MatchToken(TokenType tokenType);


    // ENBF: Parse
    void __Parse(AST *&root);


    // ENBF: Program
    void __Program(AST *&root);


    // ENBF: DeclList
    void __DeclList(AST *&root);


    // ENBF: Decl
    void __Decl(AST *&root);


    // ENBF: VarDecl
    void __VarDecl(AST *&root);


    // ENBF: Type
    void __Type(AST *&root);


    // ENBF: FuncDecl
    void __FuncDecl(AST *&root);


    // ENBF: Params
    void __Params(AST *&root);


    // ENBF: ParamList
    void __ParamList(AST *&root);


    // ENBF: Param
    void __Param(AST *&root);


    // ENBF: CompoundStmt
    void __CompoundStmt(AST *&root);


    // ENBF: LocalDecl
    void __LocalDecl(AST *&root);


    // ENBF: StmtList
    void __StmtList(AST *&root);


    // ENBF: Stmt
    void __Stmt(AST *&root);


    // ENBF: ExprStmt
    void __ExprStmt(AST *&root);


    // ENBF: IfStmt
    void __IfStmt(AST *&root);


    // ENBF: WhileStmt
    void __WhileStmt(AST *&root);


    // ENBF: ReturnStmt
    void __ReturnStmt(AST *&root);


    // ENBF: Expr
    void __Expr(AST *&root);


    // ENBF: Var
    void __Var(AST *&root);


    // ENBF: SimpleExpr
    void __SimpleExpr(AST *&root);


    // ENBF: RelOp
    void __RelOp(AST *&root);


    // ENBF: AddExpr
    void __AddExpr(AST *&root);


    // ENBF: AddOp
    void __AddOp(AST *&root);


    // ENBF: Term
    void __Term(AST *&root);


    // ENBF: MulOp
    void __MulOp(AST *&root);


    // ENBF: Factor
    void __Factor(AST *&root);


    // ENBF: Call
    void __Call(AST *&root);


    // ENBF: ArgList
    void __ArgList(AST *&root);
};


}  // End namespace CMM
