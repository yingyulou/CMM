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
    __End,                 // End

    // Ast
    __Program,             // AST: __Program
    __VarDecl,             // AST: __VarDecl
    __FuncDecl,            // AST: __FuncDecl
    __ParamList,           // AST: __ParamList
    __Param,               // AST: __Param
    __LocalDecl,           // AST: __LocalDecl
    __StmtList,            // AST: __StmtList
    __IfStmt,              // AST: __IfStmt
    __WhileStmt,           // AST: __WhileStmt
    __ReturnStmt,          // AST: __ReturnStmt
    __Expr,                // AST: __Expr
    __Var,                 // AST: __Var
    __SimpleExpr,          // AST: __SimpleExpr
    __AddExpr,             // AST: __AddExpr
    __Term,                // AST: __Term
    __Call,                // AST: __Call
    __ArgList,             // AST: __ArgList
};


}  // End namespace CMM
