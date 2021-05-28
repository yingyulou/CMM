/*
    TokenType.h
    ===========
        Enum class TOKEN_TYPE header.
*/

#ifndef __CMM_TOKEN_TYPE_H
#define __CMM_TOKEN_TYPE_H

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Enum Class TOKEN_TYPE
////////////////////////////////////////////////////////////////////////////////

enum class TOKEN_TYPE
{
    // Word
    ID,                    // ID
    NUMBER,                // Number

    // Keyword
    VOID,                  // void
    INT,                   // int
    IF,                    // if
    ELSE,                  // else
    WHILE,                 // while
    RETURN,                // return

    // Operator
    PLUS,                  // +
    MINUS,                 // -
    MULTIPLY,              // *
    DIVIDE,                // /
    LESS,                  // <
    LESS_EQUAL,            // <=
    GREATER,               // >
    GREATER_EQUAL,         // >=
    EQUAL,                 // ==
    NOT_EQUAL,             // !=
    ASSIGN,                // =
    SEMICOLON,             // ;
    COMMA,                 // ,
    LEFT_ROUND_BRACKET,    // (
    RIGHT_ROUND_BRACKET,   // )
    LEFT_SQUARE_BRACKET,   // [
    RIGHT_SQUARE_BRACKET,  // ]
    LEFT_CURLY_BRACKET,    // {
    RIGHT_CURLY_BRACKET,   // }

    // EOF
    END_OF_FILE,           // EOF

    // AST
    DECL_LIST,             // AST: DeclList
    VAR_DECL,              // AST: VarDecl
    FUNC_DECL,             // AST: FuncDecl
    PARAM_LIST,            // AST: ParamList
    PARAM,                 // AST: Param
    COMPOUND_STMT,         // AST: CompoundStmt
    LOCAL_DECL,            // AST: LocalDecl
    STMT_LIST,             // AST: StmtList
    IF_STMT,               // AST: IfStmt
    WHILE_STMT,            // AST: WhileStmt
    RETURN_STMT,           // AST: ReturnStmt
    EXPR,                  // AST: Expr
    VAR,                   // AST: Var
    SIMPLE_EXPR,           // AST: SimpleExpr
    ADD_EXPR,              // AST: AddExpr
    TERM,                  // AST: Term
    CALL,                  // AST: Call
    ARG_LIST,              // AST: ArgList
};


}  // End namespace CMM


#endif  // __CMM_TOKEN_TYPE_H
