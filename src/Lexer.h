/*
    Lexer.h
    =======
        Class Lexer header.
*/

#pragma once

#include <cstdio>
#include <string>
#include "Token.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;


////////////////////////////////////////////////////////////////////////////////
// Class Lexer
////////////////////////////////////////////////////////////////////////////////

class Lexer
{
public:

    // Constructor
    explicit Lexer(const string &inputFilePath);


    // NextToken
    Token NextToken();


    // Destructor
    ~Lexer();


private:

    // Attribute
    FILE *__filePtr;
    int __lineNo;


    // Invalid Char
    static void __invalidChar(char invalidChar, int lineNo);


    // NextToken START Stage Dispatch
    void __nextTokenStartStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_ID Stage Dispatch
    void __nextTokenInIDStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_NUMBER Stage Dispatch
    void __nextTokenInNumberStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_DIVIDE Stage Dispatch
    void __nextTokenInDivideStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_COMMENT Stage Dispatch
    void __nextTokenInCommentStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken END_COMMENT Stage Dispatch
    void __nextTokenEndCommentStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_LESS Stage Dispatch
    void __nextTokenInLessStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_GREATER Stage Dispatch
    void __nextTokenInGreaterStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_ASSIGN Stage Dispatch
    void __nextTokenInAssignStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);


    // NextToken IN_NOT Stage Dispatch
    void __nextTokenInNotStage(int nowChar, bool &saveBool, LexerStage &lexerStage,
        TokenType &tokenType, string &tokenStr);
};


}  // End namespace CMM
