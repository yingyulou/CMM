#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <cstdio>
#include <cctype>
#include <boost/format.hpp>

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;
using std::ifstream;
using std::pair;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum Class __LexerState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class __LexerState
{
    // Start
    __Start,

    // abc...
    //  ^^^^^
    __InId,

    // 123...
    //  ^^^^^
    __InNumber,

    // /?
    //  ^
    __InDivide,

    // /* ...
    //    ^^^
    __InComment,

    // ... */
    //      ^
    __EndComment,

    // <?
    //  ^
    __InLess,

    // >?
    //  ^
    __InGreater,

    // =?
    //  ^
    __InAssign,

    // !?
    //  ^
    __InNot,

    // Done
    __Done,
};


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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Keyword Map
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const unordered_map<string, __TokenType> __KEYWORD_MAP
{
    {"void",   __TokenType::__Void},
    {"int",    __TokenType::__Int},
    {"if",     __TokenType::__If},
    {"else",   __TokenType::__Else},
    {"while",  __TokenType::__While},
    {"return", __TokenType::__Return},
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __Token
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __Token
{
    // Friend
    friend class __AST;
    friend class __Compiler;


public:

    // Constructor
    explicit __Token(__TokenType tokenType, const string &tokenStr, size_t lineNum):
        __tokenType(tokenType),
        __tokenStr (tokenStr),
        __lineNum  (lineNum) {}


private:

    // Attribute
    __TokenType __tokenType;
    string __tokenStr;
    size_t __lineNum;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __AST
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __AST
{
    // Friend
    friend class __Compiler;


public:

    // Constructor
    explicit __AST(__TokenType tokenType, const string &tokenStr = "", const vector<__AST *> &subList = {}):
        __tokenType(tokenType),
        __tokenStr (tokenStr),
        __subList  (subList) {}


    // Constructor (With __tokenPtr)
    explicit __AST(const __Token *__tokenPtr):
        __tokenType(__tokenPtr->__tokenType),
        __tokenStr (__tokenPtr->__tokenStr) {}


    // Destructor
    ~__AST()
    {
        for (auto subPtr: __subList)
        {
            delete subPtr;
        }
    }


private:

    // Attribute
    __TokenType __tokenType;
    string __tokenStr;
    vector<__AST *> __subList;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __Instruction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __Instruction
{
    // Friend
    friend class __Compiler;


public:

    // Constructor
    __Instruction(const string &insName, const string &insArg = ""):
        __insName(insName),
        __insArg (insArg) {}


private:

    // Data
    string __insName;
    string __insArg;


    // To String
    string __toString() const
    {
        string insStr = __insName;

        if (!__insArg.empty())
        {
            insStr += " " + __insArg;
        }

        return insStr;
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __Compiler
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __Compiler
{
public:

    // Constructor
    explicit __Compiler(const string &inputFilePath, const string &outputFilePath):
        __inputFilePath (inputFilePath),
        __outputFilePath(outputFilePath) {}


    // operator()
    void operator()()
    {
        __main();
    }


    // Destructor
    ~__Compiler()
    {
        delete __astRoot;
    }


private:

    // Data
    string __inputFilePath;
    string __outputFilePath;
    string __codeStr;
    const char *__codePtr = nullptr;
    size_t __lineNum = 1;
    __LexerState __lexerState;
    __TokenType __tokenType;
    string __tokenStr;
    vector<__Token> __tokenList;
    const __Token *__tokenPtr = nullptr;
    __AST *__astRoot = nullptr;
    unordered_map<string, unordered_map<string, pair<size_t, size_t>>> __symMap;
    string __curFuncName;
    vector<__Instruction> __codeList;


    // Invalid Char
    void __invalidChar(char curChar) const
    {
        throw runtime_error((boost::format("Invalid char: %c in line: %zd") %
            curChar                                                         %
            __lineNum
        ).str());
    }


    // Lexer Start State
    void __lexerStartState()
    {
        if (isalpha(*__codePtr))
        {
            __lexerState = __LexerState::__InId;
            __tokenStr += *__codePtr++;
        }
        else if (isdigit(*__codePtr))
        {
            __lexerState = __LexerState::__InNumber;
            __tokenStr += *__codePtr++;
        }
        else if (isspace(*__codePtr))
        {
            if (*__codePtr == '\n')
            {
                __lineNum++;
            }

            __codePtr++;
        }
        else
        {
            switch (*__codePtr)
            {
                case '+':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__Plus;
                    __tokenStr += *__codePtr++;
                    break;

                case '-':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__Minus;
                    __tokenStr += *__codePtr++;
                    break;

                case '*':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__Multiply;
                    __tokenStr += *__codePtr++;
                    break;

                case '/':
                    __lexerState = __LexerState::__InDivide;
                    __codePtr++;
                    break;

                case '<':
                    __lexerState = __LexerState::__InLess;
                    __tokenStr += *__codePtr++;
                    break;

                case '>':
                    __lexerState = __LexerState::__InGreater;
                    __tokenStr += *__codePtr++;
                    break;

                case '=':
                    __lexerState = __LexerState::__InAssign;
                    __tokenStr += *__codePtr++;
                    break;

                case '!':
                    __lexerState = __LexerState::__InNot;
                    __tokenStr += *__codePtr++;
                    break;

                case ';':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__Semicolon;
                    __tokenStr += *__codePtr++;
                    break;

                case ',':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__Comma;
                    __tokenStr += *__codePtr++;
                    break;

                case '(':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__LeftRoundBracket;
                    __tokenStr += *__codePtr++;
                    break;

                case ')':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__RightRoundBracket;
                    __tokenStr += *__codePtr++;
                    break;

                case '[':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__LeftSquareBracket;
                    __tokenStr += *__codePtr++;
                    break;

                case ']':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__RightSquareBracket;
                    __tokenStr += *__codePtr++;
                    break;

                case '{':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__LeftCurlyBracket;
                    __tokenStr += *__codePtr++;
                    break;

                case '}':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__RightCurlyBracket;
                    __tokenStr += *__codePtr++;
                    break;

                case '\0':
                    __lexerState = __LexerState::__Done;
                    __tokenType = __TokenType::__End;
                    break;

                default:
                    __invalidChar(*__codePtr);
                    break;
            }
        }
    }


    // Lexer InId State
    void __lexerInIDState()
    {
        if (isalpha(*__codePtr))
        {
            __tokenStr += *__codePtr++;
        }
        else
        {
            __lexerState = __LexerState::__Done;
            __tokenType  = __KEYWORD_MAP.count(__tokenStr) ? __KEYWORD_MAP.at(__tokenStr) : __TokenType::__Id;
        }
    }


    // Lexer InNumber State
    void __lexerInNumberState()
    {
        if (isdigit(*__codePtr))
        {
            __tokenStr += *__codePtr++;
        }
        else
        {
            __lexerState = __LexerState::__Done;
            __tokenType  = __TokenType::__Number;
        }
    }


    // Lexer InDivide State
    void __lexerInDivideState()
    {
        if (*__codePtr == '*')
        {
            __lexerState = __LexerState::__InComment;
            __codePtr++;
        }
        else
        {
            __lexerState = __LexerState::__Done;
            __tokenType  = __TokenType::__Divide;
            __tokenStr   = "/";
        }
    }


    // Lexer InComment State
    void __lexerInCommentState()
    {
        if (*__codePtr == '*')
        {
            __lexerState = __LexerState::__EndComment;
        }
        else if (*__codePtr == '\n')
        {
            __lineNum++;
        }

        __codePtr++;
    }


    // Lexer EndComment State
    void __lexerEndCommentState()
    {
        if (*__codePtr == '/')
        {
            __lexerState = __LexerState::__Start;
        }
        else if (*__codePtr != '*')
        {
            __lexerState = __LexerState::__InComment;

            if (*__codePtr == '\n')
            {
                __lineNum++;
            }
        }

        __codePtr++;
    }


    // Lexer InLess State
    void __lexerInLessState()
    {
        __lexerState = __LexerState::__Done;

        if (*__codePtr == '=')
        {
            __tokenType = __TokenType::__LessEqual;
            __tokenStr += *__codePtr++;
        }
        else
        {
            __tokenType = __TokenType::__Less;
        }
    }


    // Lexer InGreater State
    void __lexerInGreaterState()
    {
        __lexerState = __LexerState::__Done;

        if (*__codePtr == '=')
        {
            __tokenType = __TokenType::__GreaterEqual;
            __tokenStr += *__codePtr++;
        }
        else
        {
            __tokenType = __TokenType::__Greater;
        }
    }


    // Lexer InAssign State
    void __lexerInAssignState()
    {
        __lexerState = __LexerState::__Done;

        if (*__codePtr == '=')
        {
            __tokenType = __TokenType::__Equal;
            __tokenStr += *__codePtr++;
        }
        else
        {
            __tokenType = __TokenType::__Assign;
        }
    }


    // Lexer InNot State
    void __lexerInNotState()
    {
        if (*__codePtr == '=')
        {
            __lexerState = __LexerState::__Done;
            __tokenType = __TokenType::__NotEqual;
            __tokenStr += *__codePtr++;
        }
        else
        {
            __invalidChar(*__codePtr);
        }
    }


    // Next Token
    __Token __nextToken()
    {
        __lexerState = __LexerState::__Start;
        __tokenStr.clear();

        while (__lexerState != __LexerState::__Done)
        {
            switch (__lexerState)
            {
                case __LexerState::__Start:
                    __lexerStartState();
                    break;

                case __LexerState::__InId:
                    __lexerInIDState();
                    break;

                case __LexerState::__InNumber:
                    __lexerInNumberState();
                    break;

                case __LexerState::__InDivide:
                    __lexerInDivideState();
                    break;

                case __LexerState::__InComment:
                    __lexerInCommentState();
                    break;

                case __LexerState::__EndComment:
                    __lexerEndCommentState();
                    break;

                case __LexerState::__InLess:
                    __lexerInLessState();
                    break;

                case __LexerState::__InGreater:
                    __lexerInGreaterState();
                    break;

                case __LexerState::__InAssign:
                    __lexerInAssignState();
                    break;

                case __LexerState::__InNot:
                    __lexerInNotState();
                    break;

                default:
                    throw runtime_error("Invalid __LexerState");
            }
        }

        return __Token(__tokenType, __tokenStr, __lineNum);
    }


    // Construct __tokenList
    void __constructTokenList()
    {
        ifstream fdIn(__inputFilePath);

        if (!fdIn)
        {
            throw runtime_error("Invalid " + __inputFilePath);
        }

        getline(fdIn, __codeStr, '\0');
        __codePtr = __codeStr.data();

        for (auto curToken = __nextToken();; curToken = __nextToken())
        {
            __tokenList.push_back(curToken);

            if (curToken.__tokenType == __TokenType::__End)
            {
                break;
            }
        }

        __tokenPtr = __tokenList.data();
    }


    // Invalid Token
    void __invalidToken(const __Token *__tokenPtr)
    {
        throw runtime_error((boost::format("Invalid token: %s in line %zd") %
            __tokenPtr->__tokenStr                                          %
            __tokenPtr->__lineNum
        ).str());
    }


    // Match Token
    void __matchToken(__TokenType tokenType)
    {
        if (__tokenPtr->__tokenType == tokenType)
        {
            __tokenPtr++;
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: Program
    void __astProgram(__AST *&root)
    {
        /*
            BNF:
                Program ::= Decl { Decl }

            AST:
                __TokenType::__Program
                    |---- __Decl
                    |---- [__Decl]
                    |...
        */
        root = new __AST(__TokenType::__Program, "Program", {nullptr});
        __astDecl(root->__subList[0]);

        while (__tokenPtr->__tokenType != __TokenType::__End)
        {
            root->__subList.push_back(nullptr);
            __astDecl(root->__subList.back());
        }
    }


    // AST: Decl
    void __astDecl(__AST *&root)
    {
        /*
            BNF:
                Decl ::= VarDecl
                       | FuncDecl

            AST:
                __VarDecl | __FuncDecl
        */
        if (__tokenPtr->__tokenType != __TokenType::__Int && __tokenPtr->__tokenType != __TokenType::__Void)
        {
            __invalidToken(__tokenPtr);
        }

        if (__tokenPtr[1].__tokenType != __TokenType::__Id)
        {
            __invalidToken(__tokenPtr + 1);
        }

        if (__tokenPtr[2].__tokenType == __TokenType::__LeftSquareBracket || __tokenPtr[2].__tokenType == __TokenType::__Semicolon)
        {
            __astVarDecl(root);
        }
        else if (__tokenPtr[2].__tokenType == __TokenType::__LeftRoundBracket)
        {
            __astFuncDecl(root);
        }
        else
        {
            __invalidToken(__tokenPtr + 2);
        }
    }


    // AST: VarDecl
    void __astVarDecl(__AST *&root)
    {
        /*
            BNF:
                VarDecl ::= Type ID [ '[' Number ']' ] ';'

            AST:
                __TokenType::__VarDecl
                    |---- __Type
                    |---- __TokenType::__Id
                    |---- [__TokenType::__Number]
        */
        root = new __AST(__TokenType::__VarDecl, "VarDecl", {nullptr, nullptr});
        __astType(root->__subList[0]);

        if (__tokenPtr->__tokenType == __TokenType::__Id)
        {
            root->__subList[1] = new __AST(__tokenPtr);
            __matchToken(__TokenType::__Id);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }

        if (__tokenPtr->__tokenType == __TokenType::__LeftSquareBracket)
        {
            __matchToken(__TokenType::__LeftSquareBracket);
            root->__subList.push_back(new __AST(__tokenPtr));
            __matchToken(__TokenType::__Number);
            __matchToken(__TokenType::__RightSquareBracket);
        }

        __matchToken(__TokenType::__Semicolon);
    }


    // AST: Type
    void __astType(__AST *&root)
    {
        /*
            BNF:
                Type ::= int
                       | void

            AST:
                __TokenType::__Int | __TokenType::__Void
        */
        if (__tokenPtr->__tokenType == __TokenType::__Int || __tokenPtr->__tokenType == __TokenType::__Void)
        {
            root = new __AST(__tokenPtr);
            __matchToken(__tokenPtr->__tokenType);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: FuncDecl
    void __astFuncDecl(__AST *&root)
    {
        /*
            BNF:
                FuncDecl ::= Type ID '(' ParamList ')' '{' LocalDecl StmtList '}'

            AST:
                __TokenType::__FuncDecl
                    |---- __Type
                    |---- __TokenType::__Id
                    |---- __ParamList | nullptr
                    |---- __LocalDecl
                    |---- __StmtList
        */
        root = new __AST(__TokenType::__FuncDecl, "FuncDecl", {nullptr, nullptr, nullptr, nullptr, nullptr});
        __astType(root->__subList[0]);

        if (__tokenPtr->__tokenType == __TokenType::__Id)
        {
            root->__subList[1] = new __AST(__tokenPtr);
            __matchToken(__TokenType::__Id);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }

        __matchToken(__TokenType::__LeftRoundBracket);
        __astParamList(root->__subList[2]);
        __matchToken(__TokenType::__RightRoundBracket);
        __matchToken(__TokenType::__LeftCurlyBracket);
        __astLocalDecl(root->__subList[3]);
        __astStmtList(root->__subList[4]);
        __matchToken(__TokenType::__RightCurlyBracket);
    }


    // AST: ParamList
    void __astParamList(__AST *&root)
    {
        /*
            BNF:
                ParamList ::= [ Param { ',' Param } ]

            AST:
                __TokenType::__ParamList
                    |---- __Param
                    |---- [__Param]
                    |...
        */
        if (__tokenPtr->__tokenType == __TokenType::__Int || __tokenPtr->__tokenType == __TokenType::__Void)
        {
            root = new __AST(__TokenType::__ParamList, "ParamList", {nullptr});
            __astParam(root->__subList[0]);

            while (__tokenPtr->__tokenType == __TokenType::__Comma)
            {
                __matchToken(__TokenType::__Comma);
                root->__subList.push_back(nullptr);
                __astParam(root->__subList.back());
            }
        }
    }


    // AST: Param
    void __astParam(__AST *&root)
    {
        /*
            BNF:
                Param ::= Type ID [ '[' ']' ]

            AST:
                __TokenType::__Param
                    |---- __Type
                    |---- __TokenType::__Id
        */
        root = new __AST(__TokenType::__Param, "Param", {nullptr, nullptr});
        __astType(root->__subList[0]);

        if (__tokenPtr->__tokenType == __TokenType::__Id)
        {
            root->__subList[1] = new __AST(__tokenPtr);
            __matchToken(__TokenType::__Id);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }

        if (__tokenPtr->__tokenType == __TokenType::__LeftSquareBracket)
        {
            __matchToken(__TokenType::__LeftSquareBracket);
            __matchToken(__TokenType::__RightSquareBracket);
        }
    }


    // AST: LocalDecl
    void __astLocalDecl(__AST *&root)
    {
        /*
            BNF:
                LocalDecl ::= { VarDecl }

            AST:
                __TokenType::__LocalDecl
                    |---- [__VarDecl]
                    |...
        */
        root = new __AST(__TokenType::__LocalDecl, "LocalDecl");

        while (__tokenPtr->__tokenType == __TokenType::__Int || __tokenPtr->__tokenType == __TokenType::__Void)
        {
            root->__subList.push_back(nullptr);
            __astVarDecl(root->__subList.back());
        }
    }


    // AST: StmtList
    void __astStmtList(__AST *&root)
    {
        /*
            BNF:
                StmtList ::= { Stmt }

            AST:
                __TokenType::__StmtList
                    |---- [__Stmt]
                    |...
        */
        root = new __AST(__TokenType::__StmtList, "StmtList");

        while (__tokenPtr->__tokenType == __TokenType::__Semicolon     ||
            __tokenPtr->__tokenType == __TokenType::__Id               ||
            __tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
            __tokenPtr->__tokenType == __TokenType::__Number           ||
            __tokenPtr->__tokenType == __TokenType::__LeftCurlyBracket ||
            __tokenPtr->__tokenType == __TokenType::__If               ||
            __tokenPtr->__tokenType == __TokenType::__While            ||
            __tokenPtr->__tokenType == __TokenType::__Return)
        {
            root->__subList.push_back(nullptr);
            __astStmt(root->__subList.back());
        }
    }


    // AST: Stmt
    void __astStmt(__AST *&root)
    {
        /*
            BNF:
                Stmt ::= ExprStmt
                       | IfStmt
                       | WhileStmt
                       | ReturnStmt

            AST:
                __ExprStmt | __IfStmt | __WhileStmt | __ReturnStmt
        */
        if (__tokenPtr->__tokenType == __TokenType::__Semicolon        ||
            __tokenPtr->__tokenType == __TokenType::__Id               ||
            __tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
            __tokenPtr->__tokenType == __TokenType::__Number)
        {
            __astExprStmt(root);
        }
        else if (__tokenPtr->__tokenType == __TokenType::__If)
        {
            __astIfStmt(root);
        }
        else if (__tokenPtr->__tokenType == __TokenType::__While)
        {
            __astWhileStmt(root);
        }
        else if (__tokenPtr->__tokenType == __TokenType::__Return)
        {
            __astReturnStmt(root);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: ExprStmt
    void __astExprStmt(__AST *&root)
    {
        /*
            BNF:
                ExprStmt ::= [ Expr ] ';'

            AST:
                __Expr | nullptr
        */
        if (__tokenPtr->__tokenType == __TokenType::__Id               ||
            __tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
            __tokenPtr->__tokenType == __TokenType::__Number)
        {
            __astExpr(root);
        }

        __matchToken(__TokenType::__Semicolon);
    }


    // AST: IfStmt
    void __astIfStmt(__AST *&root)
    {
        /*
            BNF:
                IfStmt ::= if '(' Expr ')' '{' StmtList '}' [ else '{' StmtList '}' ]

            AST:
                __TokenType::__IfStmt
                    |---- __Expr
                    |---- __StmtList
                    |---- [__StmtList]
        */
        root = new __AST(__TokenType::__IfStmt, "IfStmt", {nullptr, nullptr});

        __matchToken(__TokenType::__If);
        __matchToken(__TokenType::__LeftRoundBracket);
        __astExpr(root->__subList[0]);
        __matchToken(__TokenType::__RightRoundBracket);
        __matchToken(__TokenType::__LeftCurlyBracket);
        __astStmtList(root->__subList[1]);
        __matchToken(__TokenType::__RightCurlyBracket);

        if (__tokenPtr->__tokenType == __TokenType::__Else)
        {
            __matchToken(__TokenType::__Else);
            __matchToken(__TokenType::__LeftCurlyBracket);
            root->__subList.push_back(nullptr);
            __astStmtList(root->__subList[2]);
            __matchToken(__TokenType::__RightCurlyBracket);
        }
    }


    // AST: WhileStmt
    void __astWhileStmt(__AST *&root)
    {
        /*
            BNF:
                WhileStmt ::= while '(' Expr ')' '{' StmtList '}'

            AST:
                __TokenType::__WhileStmt
                    |---- __Expr
                    |---- __StmtList
        */
        root = new __AST(__TokenType::__WhileStmt, "WhileStmt", {nullptr, nullptr});

        __matchToken(__TokenType::__While);
        __matchToken(__TokenType::__LeftRoundBracket);
        __astExpr(root->__subList[0]);
        __matchToken(__TokenType::__RightRoundBracket);
        __matchToken(__TokenType::__LeftCurlyBracket);
        __astStmtList(root->__subList[1]);
        __matchToken(__TokenType::__RightCurlyBracket);
    }


    // AST: ReturnStmt
    void __astReturnStmt(__AST *&root)
    {
        /*
            BNF:
                ReturnStmt ::= return [ Expr ] ';'

            AST:
                __TokenType::__ReturnStmt
                    |---- [__Expr]
        */
        root = new __AST(__TokenType::__ReturnStmt, "ReturnStmt");
        __matchToken(__TokenType::__Return);

        if (__tokenPtr->__tokenType == __TokenType::__Id               ||
            __tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
            __tokenPtr->__tokenType == __TokenType::__Number)
        {
            root->__subList.push_back(nullptr);
            __astExpr(root->__subList.back());
        }

        __matchToken(__TokenType::__Semicolon);
    }


    // AST: Expr
    void __astExpr(__AST *&root)
    {
        /*
            BNF:
                Expr ::= Var '=' Expr
                       | SimpleExpr

            AST:
                __TokenType::__Expr
                    |---- __Var
                    |---- __Expr
                ----------------------
                __TokenType::__Expr
                    |---- __SimpleExpr
        */
        root = new __AST(__TokenType::__Expr, "Expr", {nullptr});

        if (__tokenPtr->__tokenType == __TokenType::__LeftRoundBracket || __tokenPtr->__tokenType == __TokenType::__Number)
        {
            __astSimpleExpr(root->__subList[0]);
            return;
        }
        else if (__tokenPtr->__tokenType != __TokenType::__Id)
        {
            __invalidToken(__tokenPtr);
        }

        if (__tokenPtr[1].__tokenType == __TokenType::__LeftRoundBracket)
        {
            __astSimpleExpr(root->__subList[0]);
        }
        else
        {
            auto tokenPtrBak = __tokenPtr;
            __astVar(root->__subList[0]);
            bool isAssignBool = __tokenPtr->__tokenType == __TokenType::__Assign;
            delete root->__subList[0];
            __tokenPtr = tokenPtrBak;

            if (isAssignBool)
            {
                root->__subList.push_back(nullptr);
                __astVar(root->__subList[0]);
                __matchToken(__TokenType::__Assign);
                __astExpr(root->__subList[1]);
            }
            else
            {
                __astSimpleExpr(root->__subList[0]);
            }
        }
    }


    // AST: Var
    void __astVar(__AST *&root)
    {
        /*
            BNF:
                Var ::= ID [ '[' Expr ']' ]

            AST:
                __TokenType::__Var
                    |---- __TokenType::__Id
                    |---- [__Expr]
        */
        root = new __AST(__TokenType::__Var, "Var", {nullptr});

        if (__tokenPtr->__tokenType == __TokenType::__Id)
        {
            root->__subList[0] = new __AST(__tokenPtr);
            __matchToken(__TokenType::__Id);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }

        if (__tokenPtr->__tokenType == __TokenType::__LeftSquareBracket)
        {
            __matchToken(__TokenType::__LeftSquareBracket);
            root->__subList.push_back(nullptr);
            __astExpr(root->__subList[1]);
            __matchToken(__TokenType::__RightSquareBracket);
        }
    }


    // AST: SimpleExpr
    void __astSimpleExpr(__AST *&root)
    {
        /*
            BNF:
                SimpleExpr ::= AddExpr [ RelOp AddExpr ]

            AST:
                __TokenType::__SimpleExpr
                    |---- __AddExpr
                    |---- [__RelOp]
                    |---- [__AddExpr]
        */
        root = new __AST(__TokenType::__SimpleExpr, "SimpleExpr", {nullptr});
        __astAddExpr(root->__subList[0]);

        if (__tokenPtr->__tokenType == __TokenType::__Less         ||
            __tokenPtr->__tokenType == __TokenType::__LessEqual    ||
            __tokenPtr->__tokenType == __TokenType::__Greater      ||
            __tokenPtr->__tokenType == __TokenType::__GreaterEqual ||
            __tokenPtr->__tokenType == __TokenType::__Equal        ||
            __tokenPtr->__tokenType == __TokenType::__NotEqual)
        {
            root->__subList.push_back(nullptr);
            __astRelOp(root->__subList[1]);
            root->__subList.push_back(nullptr);
            __astAddExpr(root->__subList[2]);
        }
    }


    // AST: RelOp
    void __astRelOp(__AST *&root)
    {
        /*
            BNF:
                RelOp ::= <
                        | <=
                        | >
                        | >=
                        | ==
                        | !=

            AST:
                __TokenType::__Less         |
                __TokenType::__LessEqual    |
                __TokenType::__Greater      |
                __TokenType::__GreaterEqual |
                __TokenType::__Equal        |
                __TokenType::__NotEqual
        */
        if (__tokenPtr->__tokenType == __TokenType::__Less         ||
            __tokenPtr->__tokenType == __TokenType::__LessEqual    ||
            __tokenPtr->__tokenType == __TokenType::__Greater      ||
            __tokenPtr->__tokenType == __TokenType::__GreaterEqual ||
            __tokenPtr->__tokenType == __TokenType::__Equal        ||
            __tokenPtr->__tokenType == __TokenType::__NotEqual)
        {
            root = new __AST(__tokenPtr);
            __matchToken(__tokenPtr->__tokenType);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: AddExpr
    void __astAddExpr(__AST *&root)
    {
        /*
            BNF:
                AddExpr ::= Term { AddOp Term }

            AST:
                __TokenType::__AddExpr
                    |---- __Term
                    |---- [__AddOp]
                    |---- [__Term]
                    |...
        */
        root = new __AST(__TokenType::__AddExpr, "AddExpr", {nullptr});
        __astTerm(root->__subList[0]);

        while (__tokenPtr->__tokenType == __TokenType::__Plus || __tokenPtr->__tokenType == __TokenType::__Minus)
        {
            root->__subList.push_back(nullptr);
            __astAddOp(root->__subList.back());
            root->__subList.push_back(nullptr);
            __astTerm(root->__subList.back());
        }
    }


    // AST: AddOp
    void __astAddOp(__AST *&root)
    {
        /*
            BNF:
                AddOp ::= +
                        | -

            AST:
                __TokenType::__Plus | __TokenType::__Minus
        */
        if (__tokenPtr->__tokenType == __TokenType::__Plus || __tokenPtr->__tokenType == __TokenType::__Minus)
        {
            root = new __AST(__tokenPtr);
            __matchToken(__tokenPtr->__tokenType);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: Term
    void __astTerm(__AST *&root)
    {
        /*
            BNF:
                Term ::= Factor { MulOp Factor }

            AST:
                __TokenType::__Term
                    |---- __Factor
                    |---- [__MulOp]
                    |---- [__Factor]
                    |...
        */
        root = new __AST(__TokenType::__Term, "Term", {nullptr});
        __astFactor(root->__subList[0]);

        while (__tokenPtr->__tokenType == __TokenType::__Multiply || __tokenPtr->__tokenType == __TokenType::__Divide)
        {
            root->__subList.push_back(nullptr);
            __astMulOp(root->__subList.back());
            root->__subList.push_back(nullptr);
            __astFactor(root->__subList.back());
        }
    }


    // AST: MulOp
    void __astMulOp(__AST *&root)
    {
        /*
            BNF:
                MulOp ::= *
                        | /

            AST:
                __TokenType::__Multiply | __TokenType::__Divide
        */
        if (__tokenPtr->__tokenType == __TokenType::__Multiply || __tokenPtr->__tokenType == __TokenType::__Divide)
        {
            root = new __AST(__tokenPtr);
            __matchToken(__tokenPtr->__tokenType);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: Factor
    void __astFactor(__AST *&root)
    {
        /*
            BNF:
                Factor ::= '(' Expr ')'
                         | Number
                         | Call
                         | Var

            AST:
                __Expr | __TokenType::__Number | __Call | __Var
        */
        if (__tokenPtr->__tokenType == __TokenType::__LeftRoundBracket)
        {
            __matchToken(__TokenType::__LeftRoundBracket);
            __astExpr(root);
            __matchToken(__TokenType::__RightRoundBracket);
        }
        else if (__tokenPtr->__tokenType == __TokenType::__Number)
        {
            root = new __AST(__tokenPtr);
            __matchToken(__tokenPtr->__tokenType);
        }
        else if (__tokenPtr->__tokenType == __TokenType::__Id)
        {
            if (__tokenPtr[1].__tokenType == __TokenType::__LeftRoundBracket)
            {
                __astCall(root);
            }
            else
            {
                __astVar(root);
            }
        }
        else
        {
            __invalidToken(__tokenPtr);
        }
    }


    // AST: Call
    void __astCall(__AST *&root)
    {
        /*
            BNF:
                Call ::= ID '(' [ ArgList ] ')'

            AST:
                __TokenType::__Call
                    |---- __TokenType::__Id
                    |---- [__ArgList]
        */
        root = new __AST(__TokenType::__Call, "Call", {nullptr});

        if (__tokenPtr->__tokenType == __TokenType::__Id)
        {
            root->__subList[0] = new __AST(__tokenPtr);
            __matchToken(__TokenType::__Id);
        }
        else
        {
            __invalidToken(__tokenPtr);
        }

        __matchToken(__TokenType::__LeftRoundBracket);

        if (__tokenPtr->__tokenType == __TokenType::__Id               ||
            __tokenPtr->__tokenType == __TokenType::__LeftRoundBracket ||
            __tokenPtr->__tokenType == __TokenType::__Number)
        {
            root->__subList.push_back(nullptr);
            __astArgList(root->__subList[1]);
        }

        __matchToken(__TokenType::__RightRoundBracket);
    }


    // AST: ArgList
    void __astArgList(__AST *&root)
    {
        /*
            BNF:
                ArgList ::= Expr { ',' Expr }

            AST:
                __TokenType::__ArgList
                    |---- __Expr
                    |---- [__Expr]
                    |...
        */
        root = new __AST(__TokenType::__ArgList, "ArgList", {nullptr});
        __astExpr(root->__subList[0]);

        while (__tokenPtr->__tokenType == __TokenType::__Comma)
        {
            __matchToken(__TokenType::__Comma);
            root->__subList.push_back(nullptr);
            __astExpr(root->__subList.back());
        }
    }


    // Construct AST
    void __constructAst()
    {
        __astProgram(__astRoot);
    }


    // Construct __symMap
    void __constructSymMap()
    {
        /*
            Function name -> Var name -> (Var number, Array size)
        */
        size_t globalIdx = 0;
        __symMap["__GLOBAL__"];

        /*
            __TokenType::__Program
                |---- __Decl
                |---- [__Decl]
                |...
        */
        for (auto declNodePtr: __astRoot->__subList)
        {
            /*
                __VarDecl | __FuncDecl
            */
            if (declNodePtr->__tokenType == __TokenType::__FuncDecl)
            {
                /*
                    __TokenType::__FuncDecl
                        |---- __Type
                        |---- __TokenType::__Id
                        |---- __ParamList | nullptr
                        |---- __LocalDecl
                        |---- __StmtList
                */
                size_t varIdx = 0;
                string funcName = declNodePtr->__subList[1]->__tokenStr;
                __symMap[funcName];

                if (declNodePtr->__subList[2])
                {
                    /*
                        __TokenType::__ParamList
                            |---- __Param
                            |---- [__Param]
                            |...
                    */
                    for (auto paramPtr: declNodePtr->__subList[2]->__subList)
                    {
                        /*
                            __TokenType::__Param
                                |---- __Type
                                |---- __TokenType::__Id
                        */
                        string varName = paramPtr->__subList[1]->__tokenStr;
                        __symMap[funcName][varName] = {varIdx++, 0};
                    }
                }

                /*
                    __TokenType::__LocalDecl
                        |---- [__VarDecl]
                        |...
                */
                for (auto varDeclPtr: declNodePtr->__subList[3]->__subList)
                {
                    /*
                        __TokenType::__VarDecl
                            |---- __Type
                            |---- __TokenType::__Id
                            |---- [__TokenType::__Number]
                    */
                    string varName = varDeclPtr->__subList[1]->__tokenStr;
                    size_t varSize = varDeclPtr->__subList.size() == 2 ? 0 : stoi(varDeclPtr->__subList[2]->__tokenStr);
                    __symMap[funcName][varName] = {varIdx, varSize};
                    varIdx += varSize + 1;
                }
            }
            else
            {
                /*
                    __TokenType::__VarDecl
                        |---- __Type
                        |---- __TokenType::__Id
                        |---- [__TokenType::__Number]
                */
                string varName = declNodePtr->__subList[1]->__tokenStr;
                size_t varSize = declNodePtr->__subList.size() == 2 ? 0 : stoi(declNodePtr->__subList[2]->__tokenStr);
                __symMap["__GLOBAL__"][varName] = {globalIdx, varSize};
                globalIdx += varSize + 1;
            }
        }
    }


    // Generate Code: Number
    vector<__Instruction> __genCodeNumber(__AST *root) const
    {
        /*
            __TokenType::__Number
        */
        return {{"ldc", root->__tokenStr}};
    }


    // Generate Code: StmtList
    vector<__Instruction> __genCodeStmtList(__AST *root) const
    {
        /*
            __TokenType::__StmtList
                |---- [__Stmt]
                |...
        */
        vector<__Instruction> codeList;

        for (auto stmtPtr: root->__subList)
        {
            auto stmtCodeList = __genCodeStmt(stmtPtr);
            codeList.insert(codeList.end(), stmtCodeList.begin(), stmtCodeList.end());
        }

        return codeList;
    }


    // Generate Code: Stmt
    vector<__Instruction> __genCodeStmt(__AST *root) const
    {
        /*
            __ExprStmt | __IfStmt | __WhileStmt | __ReturnStmt
            __ExprStmt: __Expr | nullptr
        */
        if (!root)
        {
            return {};
        }

        switch (root->__tokenType)
        {
            case __TokenType::__Expr:
                return __genCodeExpr(root);

            case __TokenType::__IfStmt:
                return __genCodeIfStmt(root);

            case __TokenType::__WhileStmt:
                return __genCodeWhileStmt(root);

            case __TokenType::__ReturnStmt:
                return __genCodeReturnStmt(root);

            default:
                throw runtime_error("Invalid __TokenType");
        }
    }


    // Generate Code: IfStmt
    vector<__Instruction> __genCodeIfStmt(__AST *root) const
    {
        /*
            __TokenType::__IfStmt
                |---- __Expr
                |---- __StmtList
                |---- [__StmtList]
        */
        auto codeList   = __genCodeExpr(root->__subList[0]);
        auto ifCodeList = __genCodeStmtList(root->__subList[1]);

        if (root->__subList.size() == 2)
        {
            /*
                if ...
                    jz end
                    ...
                end:
                    ...
            */
            codeList.emplace_back("jz", to_string(ifCodeList.size() + 1));
            codeList.insert(codeList.end(), ifCodeList.begin(), ifCodeList.end());
        }
        else
        {
            /*
                if ...
                    jz else
                    ...
                    jmp end
                else:
                    ...
                end:
                    ...
            */
            auto elseCodeList = __genCodeStmtList(root->__subList[2]);

            ifCodeList.emplace_back("jmp", to_string(elseCodeList.size() + 1));
            codeList.emplace_back("jz", to_string(ifCodeList.size() + 1));
            codeList.insert(codeList.end(), ifCodeList.begin(), ifCodeList.end());
            codeList.insert(codeList.end(), elseCodeList.begin(), elseCodeList.end());
        }

        return codeList;
    }


    // Generate Code: WhileStmt
    vector<__Instruction> __genCodeWhileStmt(__AST *root) const
    {
        /*
            __TokenType::__WhileStmt
                |---- __Expr
                |---- __StmtList
        */
        auto codeList     = __genCodeExpr(root->__subList[0]);
        auto stmtCodeList = __genCodeStmtList(root->__subList[1]);

        /*
            while ...
                jz end
                ...
                jmp while
            end:
                ...
        */
        codeList.emplace_back("jz", to_string(stmtCodeList.size() + 2));
        codeList.insert(codeList.end(), stmtCodeList.begin(), stmtCodeList.end());
        codeList.emplace_back("jmp", "-" + to_string(codeList.size()));

        return codeList;
    }


    // Generate Code: ReturnStmt
    vector<__Instruction> __genCodeReturnStmt(__AST *root) const
    {
        /*
            __TokenType::__ReturnStmt
                |---- [__Expr]
        */
        if (root->__subList.empty())
        {
            return {};
        }
        else
        {
            return __genCodeExpr(root->__subList[0]);
        }
    }


    // Generate Code: Expr
    vector<__Instruction> __genCodeExpr(__AST *root) const
    {
        /*
            __TokenType::__Expr
                |---- __Var
                |---- __Expr
            ----------------------
            __TokenType::__Expr
                |---- __SimpleExpr
        */
        if (root->__subList.size() == 1)
        {
            return __genCodeSimpleExpr(root->__subList[0]);
        }
        else
        {
            auto codeList       = __genCodeExpr(root->__subList[1]);
            auto assignCodeList = __genCodeAssign(root->__subList[0]);

            codeList.insert(codeList.end(), assignCodeList.begin(), assignCodeList.end());

            return codeList;
        }
    }


    // Generate Code: Var
    vector<__Instruction> __genCodeVar(__AST *root) const
    {
        /*
            __TokenType::__Var
                |---- __TokenType::__Id
                |---- [__Expr]
        */
        vector<__Instruction> codeList;

        // Local var
        if (__symMap.at(__curFuncName).count(root->__subList[0]->__tokenStr))
        {
            codeList.emplace_back("ldc", to_string(__symMap.at(__curFuncName).at(root->__subList[0]->__tokenStr).first));
            codeList.emplace_back("ld");
        }
        // Global var
        else
        {
            codeList.emplace_back("ldc", to_string(__symMap.at("__GLOBAL__").at(root->__subList[0]->__tokenStr).first));
            codeList.emplace_back("ald");
        }

        // Array
        if (root->__subList.size() == 2)
        {
            auto exprCodeList = __genCodeExpr(root->__subList[1]);

            codeList.emplace_back("push");
            codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());
            codeList.emplace_back("add");
            codeList.emplace_back("pop");
            codeList.emplace_back("ald");
        }

        return codeList;
    }


    // Generate Code: SimpleExpr
    vector<__Instruction> __genCodeSimpleExpr(__AST *root) const
    {
        /*
            __TokenType::__SimpleExpr
                |---- __AddExpr
                |---- [__RelOp]
                |---- [__AddExpr]
        */
        if (root->__subList.size() == 1)
        {
            return __genCodeAddExpr(root->__subList[0]);
        }
        else
        {
            auto codeList      = __genCodeAddExpr(root->__subList[0]);
            auto midCodeList   = __genCodeRelOp(root->__subList[1]);
            auto rightCodeList = __genCodeAddExpr(root->__subList[2]);

            codeList.emplace_back("push");
            codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
            codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());
            codeList.emplace_back("pop");

            return codeList;
        }
    }


    // Generate Code: RelOp
    vector<__Instruction> __genCodeRelOp(__AST *root) const
    {
        /*
            __TokenType::__Less         |
            __TokenType::__LessEqual    |
            __TokenType::__Greater      |
            __TokenType::__GreaterEqual |
            __TokenType::__Equal        |
            __TokenType::__NotEqual
        */
        switch (root->__tokenType)
        {
            case __TokenType::__Less:
                return {{"lt"}};

            case __TokenType::__LessEqual:
                return {{"le"}};

            case __TokenType::__Greater:
                return {{"gt"}};

            case __TokenType::__GreaterEqual:
                return {{"ge"}};

            case __TokenType::__Equal:
                return {{"eq"}};

            case __TokenType::__NotEqual:
                return {{"ne"}};

            default:
                throw runtime_error("Invalid __TokenType");
        }
    }


    // Generate Code: AddExpr
    vector<__Instruction> __genCodeAddExpr(__AST *root) const
    {
        /*
            __TokenType::__AddExpr
                |---- __Term
                |---- [__AddOp]
                |---- [__Term]
                |...
        */
        auto codeList = __genCodeTerm(root->__subList[0]);

        for (size_t idx = 1; idx < root->__subList.size(); idx += 2)
        {
            auto midCodeList   = __genCodeAddOp(root->__subList[idx]);
            auto rightCodeList = __genCodeTerm(root->__subList[idx + 1]);

            codeList.emplace_back("push");
            codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
            codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());
            codeList.emplace_back("pop");
        }

        return codeList;
    }


    // Generate Code: AddOp
    vector<__Instruction> __genCodeAddOp(__AST *root) const
    {
        /*
            __TokenType::__Plus | __TokenType::__Minus
        */
        if (root->__tokenType == __TokenType::__Plus)
        {
            return {{"add"}};
        }
        else
        {
            return {{"sub"}};
        }
    }


    // Generate Code: Term
    vector<__Instruction> __genCodeTerm(__AST *root) const
    {
        /*
            __TokenType::__Term
                |---- __Factor
                |---- [__MulOp]
                |---- [__Factor]
                |...
        */
        auto codeList = __genCodeFactor(root->__subList[0]);

        for (size_t idx = 1; idx < root->__subList.size(); idx += 2)
        {
            auto midCodeList   = __genCodeMulOp(root->__subList[idx]);
            auto rightCodeList = __genCodeFactor(root->__subList[idx + 1]);

            codeList.emplace_back("push");
            codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
            codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());
            codeList.emplace_back("pop");
        }

        return codeList;
    }


    // Generate Code: MulOp
    vector<__Instruction> __genCodeMulOp(__AST *root) const
    {
        /*
            __TokenType::__Multiply | __TokenType::__Divide
        */
        if (root->__tokenType == __TokenType::__Multiply)
        {
            return {{"mul"}};
        }
        else
        {
            return {{"div"}};
        }
    }


    // Generate Code: Factor
    vector<__Instruction> __genCodeFactor(__AST *root) const
    {
        /*
            __Expr | __TokenType::__Number | __Call | __Var
        */
        switch (root->__tokenType)
        {
            case __TokenType::__Expr:
                return __genCodeExpr(root);

            case __TokenType::__Number:
                return __genCodeNumber(root);

            case __TokenType::__Call:
                return __genCodeCall(root);

            case __TokenType::__Var:
                return __genCodeVar(root);

            default:
                throw runtime_error("Invalid __TokenType");
        }
    }


    // Generate Code: Call
    vector<__Instruction> __genCodeCall(__AST *root) const
    {
        /*
            __TokenType::__Call
                |---- __TokenType::__Id
                |---- [__ArgList]
        */
        // xxx = input();
        if (root->__subList[0]->__tokenStr == "input")
        {
            return {{"in"}};
        }
        // output(xxx);
        else if (root->__subList[0]->__tokenStr == "output")
        {
            /*
                __TokenType::__ArgList
                    |---- __Expr
            */
            auto codeList = __genCodeExpr(root->__subList[1]->__subList[0]);

            codeList.emplace_back("out");

            return codeList;
        }

        vector<__Instruction> codeList;
        vector<pair<string, pair<size_t, size_t>>> pairList(__symMap.at(root->__subList[0]->__tokenStr).size());

        // ..., Local5, Local4, Local3, Param2, Param1, Param0
        for (auto &mapPair: __symMap.at(root->__subList[0]->__tokenStr))
        {
            pairList[pairList.size() - mapPair.second.first - 1] = mapPair;
        }

        // We only need local var here
        size_t topIdx = pairList.size() - (root->__subList.size() == 2 ?

            // Call function by at least one parameter
            root->__subList[1]->__subList.size() :

            // Call function without any parameter
            0);

        // Push local var
        for (size_t idx = 0; idx < topIdx; idx++)
        {
            // Array
            if (pairList[idx].second.second)
            {
                // Push array content (by array size times)
                for (size_t _ = 0; _ < pairList[idx].second.second; _++)
                {
                    codeList.emplace_back("push");
                }

                /*
                    The instruction "lea N" calculate the array start pointer (absolute index in SS).
                    SS:
                        ... X X X X X X X X X $
                            ^     Size = N    ^
                            |                 |
                         SP - N               SP
                */
                codeList.emplace_back("lea", to_string(pairList[idx].second.second));
                codeList.emplace_back("push");
            }
            // Scalar
            else
            {
                codeList.emplace_back("push");
            }
        }

        // Push parameter
        if (root->__subList.size() == 2)
        {
            auto argListCodeList = __genCodeArgList(root->__subList[1]);

            codeList.insert(codeList.end(), argListCodeList.begin(), argListCodeList.end());
        }

        /*
            The instruction "CALL N" perform multiple actions:

            1. SS.PUSH(BP)
                Now the SS is like:
                ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP

            2. BP = SS.SIZE() - 2
                Now the SS is like:
                ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP
                                                         ^
                                                         BP
            3. SS.PUSH(IP)
                Now the SS is like:
                ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP OldIP
                                                         ^
                                                         BP
            4. IP += N
                In addition, the "N" of the "CALL N" is only a function name right now,
                it will be translated to a number later. (See the function: __translateCall)
        */
        codeList.emplace_back("call", root->__subList[0]->__tokenStr);

        // After call, we need several "POP" to pop all vars.
        for (auto &[_, mapVal]: __symMap.at(root->__subList[0]->__tokenStr))
        {
            // Any var needs a "POP"
            codeList.emplace_back("pop");

            // Pop array content (by array size times)
            for (size_t _ = 0; _ < mapVal.second; _++)
            {
                codeList.emplace_back("pop");
            }
        }

        return codeList;
    }


    // Generate Code: ArgList
    vector<__Instruction> __genCodeArgList(__AST *root) const
    {
        /*
            __TokenType::__ArgList
                |---- __Expr
                |---- [__Expr]
                |...
        */
        vector<__Instruction> codeList;

        for (int64_t idx = (int64_t)root->__subList.size() - 1; idx >= 0; idx--)
        {
            auto exprCodeList = __genCodeExpr(root->__subList[idx]);

            codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());
            codeList.emplace_back("push");
        }

        return codeList;
    }


    // Generate Code: Assign
    vector<__Instruction> __genCodeAssign(__AST *root) const
    {
        /*
            __TokenType::__Expr
                |---- __Var  -> Root
                |---- __Expr -> AX
        */
        vector<__Instruction> codeList {{"push"}};

        // Local var
        if (__symMap.at(__curFuncName).count(root->__subList[0]->__tokenStr))
        {
            codeList.emplace_back("ldc", to_string(__symMap.at(__curFuncName).at(root->__subList[0]->__tokenStr).first));

            // Scalar
            if (root->__subList.size() == 1)
            {
                codeList.emplace_back("st");
            }
            // Array
            else
            {
                auto exprCodeList = __genCodeExpr(root->__subList[1]);

                // Get the (start) pointer (is already an absolute address)
                codeList.emplace_back("ld");
                codeList.emplace_back("push");
                codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

                // Pointer[Index] (Pointer + Index)
                codeList.emplace_back("add");
                codeList.emplace_back("pop");

                // Save by absolute address
                codeList.emplace_back("ast");
            }
        }
        // Global var
        else
        {
            codeList.emplace_back("ldc", to_string(__symMap.at("__GLOBAL__").at(root->__subList[0]->__tokenStr).first));

            // Scalar
            if (root->__subList.size() == 1)
            {
                codeList.emplace_back("ast");
            }
            // Array
            else
            {
                auto exprCodeList = __genCodeExpr(root->__subList[1]);

                // Absolute get the (start) pointer (is already an absolute address)
                codeList.emplace_back("ald");
                codeList.emplace_back("push");
                codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

                // Pointer[Index] (Pointer + Index)
                codeList.emplace_back("add");
                codeList.emplace_back("pop");

                // Save by absolute address
                codeList.emplace_back("ast");
            }
        }

        codeList.emplace_back("pop");

        return codeList;
    }


    // Generate Code: Global Var
    vector<__Instruction> __genCodeGlobalVar() const
    {
        vector<__Instruction> codeList;

        for (auto &[_, infoPair]: __symMap.at("__GLOBAL__"))
        {
            // Array
            if (infoPair.second)
            {
                // Calc the array start address (var number + 1)
                codeList.emplace_back("ldc", to_string(infoPair.first + 1));
            }

            // Push the array start address
            // (Or only a meaningless int for global scalar memeory)
            codeList.emplace_back("push");

            // Push array content (by array size times)
            for (size_t _ = 0; _ < infoPair.second; _++)
            {
                codeList.emplace_back("push");
            }
        }

        return codeList;
    }


    // Generate Code: Begin
    vector<__Instruction> __genCodeBegin() const
    {
        // The "main" function is a special function, so the following code is similar with the function: __genCodeCall
        vector<__Instruction> codeList;
        vector<pair<string, pair<size_t, size_t>>> pairList(__symMap.at("main").size());

        // ..., Local2, Local1, Local0
        for (auto &mapPair: __symMap.at("main"))
        {
            pairList[pairList.size() - mapPair.second.first - 1] = mapPair;
        }

        // The "main" function has definitely no params
        for (auto &[_, infoPair]: pairList)
        {
            if (infoPair.second)
            {
                for (size_t _ = 0; _ < infoPair.second; _++)
                {
                    codeList.emplace_back("push");
                }

                codeList.emplace_back("lea", to_string(infoPair.second));
                codeList.emplace_back("push");
            }
            else
            {
                codeList.emplace_back("push");
            }
        }

        // Call the "main" function automatically
        codeList.emplace_back("call", "main");

        return codeList;
    }


    // Generate Code: Global
    vector<__Instruction> __genCodeGlobal() const
    {
        auto codeList = __genCodeGlobalVar();
        auto beginCodeList = __genCodeBegin();

        codeList.insert(codeList.end(), beginCodeList.begin(), beginCodeList.end());

        return codeList;
    }


    // Construct __codeList
    void __constructCodeList()
    {
        unordered_map<string, vector<__Instruction>> codeMap
        {
            {"__GLOBAL__", __genCodeGlobal()},
        };

        // Function name -> Function start IP
        unordered_map<string, int64_t> funcJmpMap;

        /*
            __TokenType::__Program
                |---- __Decl
                |---- [__Decl]
                |...
        */
        for (auto declPtr: __astRoot->__subList)
        {
            /*
                __VarDecl | __FuncDecl
            */
            if (declPtr->__tokenType == __TokenType::__FuncDecl)
            {
                /*
                    __TokenType::__FuncDecl
                        |---- __Type
                        |---- __TokenType::__Id
                        |---- __ParamList | nullptr
                        |---- __LocalDecl
                        |---- __StmtList
                */
                __curFuncName = declPtr->__subList[1]->__tokenStr;
                auto codeList = __genCodeStmtList(declPtr->__subList[4]);

                if (__curFuncName != "main")
                {
                    /*
                        The instruction "RET" perform multiple actions:

                        1. IP = SS.POP()
                            Now the SS is like:
                            ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP

                        2. BP = SS.POP()
                            Now the SS is like:
                            ... Local5 Local4 Local3 Param2 Param1 Param0

                        So we still need several "POP" to pop all vars. (See the function: __genCodeCall)
                    */
                    codeList.emplace_back("ret");
                }

                codeMap[__curFuncName] = codeList;
            }
        }

        // Global code must be the first part
        int64_t jmpNum = codeMap.at("__GLOBAL__").size();

        __codeList.insert(__codeList.end(), codeMap.at("__GLOBAL__").begin(), codeMap.at("__GLOBAL__").end());

        // Other functions
        for (auto &[funcName, subCodeList]: codeMap)
        {
            if (funcName != "__GLOBAL__" && funcName != "main")
            {
                __codeList.insert(__codeList.end(), subCodeList.begin(), subCodeList.end());
                funcJmpMap[funcName] = jmpNum;
                jmpNum += subCodeList.size();
            }
        }

        // The "main" function must be the last function
        __codeList.insert(__codeList.end(), codeMap.at("main").begin(), codeMap.at("main").end());

        funcJmpMap["main"] = jmpNum;

        // A virtual "IP"
        for (size_t IP = 0; IP < __codeList.size(); IP++)
        {
            if (__codeList[IP].__insName == "call")
            {
                __codeList[IP].__insArg = to_string(funcJmpMap.at(__codeList[IP].__insArg) - (int64_t)IP);
            }
        }
    }


    // Output Result
    void __outputResult() const
    {
        FILE *fdOut = fopen(__outputFilePath.c_str(), "w");

        if (!fdOut)
        {
            throw runtime_error("Invalid " + __outputFilePath);
        }

        for (auto insObj: __codeList)
        {
            fprintf(fdOut, "%s\n", insObj.__toString().c_str());
        }

        fclose(fdOut);
    }


    // Main
    void __main()
    {
        if (__inputFilePath.empty() || __outputFilePath.empty())
        {
            return;
        }

        __constructTokenList();
        __constructAst();
        __constructSymMap();
        __constructCodeList();
        __outputResult();
    }
};


}  // End namespace CMM
