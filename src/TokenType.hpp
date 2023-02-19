/*
    TokenType.hpp
    =============
        Enum class TokenType implementation.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum Class TokenType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class TokenType
{
    // Word
    Id,                  // Id
    Number,              // Number

    // Keyword
    Void,                // void
    Int,                 // int
    If,                  // if
    Else,                // else
    While,               // while
    Return,              // return

    // Operator
    Plus,                // +
    Minus,               // -
    Multiply,            // *
    Divide,              // /
    Less,                // <
    LessEqual,           // <=
    Greater,             // >
    GreaterEqual,        // >=
    Equal,               // ==
    NotEqual,            // !=
    Assign,              // =
    Semicolon,           // ;
    Comma,               // ,
    LeftRoundBracket,    // (
    RightRoundBracket,   // )
    LeftSquareBracket,   // [
    RightSquareBracket,  // ]
    LeftCurlyBracket,    // {
    RightCurlyBracket,   // }

    // End
    END,                 // END

    // Ast
    DeclList,            // AST: DeclList
    VarDecl,             // AST: VarDecl
    FuncDecl,            // AST: FuncDecl
    ParamList,           // AST: ParamList
    Param,               // AST: Param
    CompoundStmt,        // AST: CompoundStmt
    LocalDecl,           // AST: LocalDecl
    StmtList,            // AST: StmtList
    IfStmt,              // AST: IfStmt
    WhileStmt,           // AST: WhileStmt
    ReturnStmt,          // AST: ReturnStmt
    Expr,                // AST: Expr
    Var,                 // AST: Var
    SimpleExpr,          // AST: SimpleExpr
    AddExpr,             // AST: AddExpr
    Term,                // AST: Term
    Call,                // AST: Call
    ArgList,             // AST: ArgList
};


}  // End namespace CMM
