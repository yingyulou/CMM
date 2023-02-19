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
    __Program,             // __AST: Program
    __VarDecl,             // __AST: VarDecl
    __FuncDecl,            // __AST: FuncDecl
    __ParamList,           // __AST: ParamList
    __Param,               // __AST: Param
    __LocalDecl,           // __AST: LocalDecl
    __StmtList,            // __AST: StmtList
    __IfStmt,              // __AST: IfStmt
    __WhileStmt,           // __AST: WhileStmt
    __ReturnStmt,          // __AST: ReturnStmt
    __Expr,                // __AST: Expr
    __Var,                 // __AST: Var
    __SimpleExpr,          // __AST: SimpleExpr
    __AddExpr,             // __AST: AddExpr
    __Term,                // __AST: Term
    __Call,                // __AST: Call
    __ArgList,             // __AST: ArgList
};


}  // End namespace CMM
