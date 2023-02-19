/*
    TokenType.hpp
    =============
        Enum class __TokenType implementation.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum Class __TokenType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class __TokenType
{
    // Word
    __Id,                  // Id
    __Number,              // Number

    // Keyword
    __Void,                // void
    __Int,                 // int
    __If,                  // if
    __Else,                // else
    __While,               // while
    __Return,              // return

    // Operator
    __Plus,                // +
    __Minus,               // -
    __Multiply,            // *
    __Divide,              // /
    __Less,                // <
    __LessEqual,           // <=
    __Greater,             // >
    __GreaterEqual,        // >=
    __Equal,               // ==
    __NotEqual,            // !=
    __Assign,              // =
    __Semicolon,           // ;
    __Comma,               // ,
    __LeftRoundBracket,    // (
    __RightRoundBracket,   // )
    __LeftSquareBracket,   // [
    __RightSquareBracket,  // ]
    __LeftCurlyBracket,    // {
    __RightCurlyBracket,   // }

    // End
    __END,                 // END

    // Ast
    __Program,             // AST: Program
    __VarDecl,             // AST: VarDecl
    __FuncDecl,            // AST: FuncDecl
    __ParamList,           // AST: ParamList
    __Param,               // AST: Param
    __LocalDecl,           // AST: LocalDecl
    __StmtList,            // AST: StmtList
    __IfStmt,              // AST: IfStmt
    __WhileStmt,           // AST: WhileStmt
    __ReturnStmt,          // AST: ReturnStmt
    __Expr,                // AST: Expr
    __Var,                 // AST: Var
    __SimpleExpr,          // AST: SimpleExpr
    __AddExpr,             // AST: AddExpr
    __Term,                // AST: Term
    __Call,                // AST: Call
    __ArgList,             // AST: ArgList
};


}  // End namespace CMM
