/*
    LexicalAnalyzer.h
    =================
        Class __LexicalAnalyzer header.
*/

#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "TokenType.hpp"
#include "LexerStage.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Class LexicalAnalyzer
=======
// Class __LexicalAnalyzer
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __LexicalAnalyzer
{
    // Friend
    friend class Core;


public:

    // Constructor
    explicit __LexicalAnalyzer(const string &inputFilePath = "");


private:

    // Attribute
    string __inputFilePath;


    // Invalid Char
    static void __invalidChar(char curChar, int lineNo);


<<<<<<< HEAD
    // Next Token LexerStage::Start Stage
    static void __nextTokenStartStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &lineNo);


    // Next Token LexerStage::InId Stage
    static void __nextTokenInIDStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token LexerStage::InNumber Stage
    static void __nextTokenInNumberStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token LexerStage::InDivide Stage
    static void __nextTokenInDivideStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token LexerStage::InComment Stage
    static void __nextTokenInCommentStage(const char *&codePtr, LexerStage &lexerStage, TokenType &,
        string &, int &);


    // Next Token LexerStage::EndComment Stage
    static void __nextTokenEndCommentStage(const char *&codePtr, LexerStage &lexerStage, TokenType &,
        string &, int &);


    // Next Token LexerStage::InLess Stage
    static void __nextTokenInLessStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token LexerStage::InGreater Stage
    static void __nextTokenInGreaterStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token LexerStage::InAssign Stage
    static void __nextTokenInAssignStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token LexerStage::InNot Stage
    static void __nextTokenInNotStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
    // Next Token __LexerStage::__Start Stage
    static void __nextTokenStartStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &lineNo);


    // Next Token __LexerStage::__InId Stage
    static void __nextTokenInIDStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token __LexerStage::__InNumber Stage
    static void __nextTokenInNumberStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token __LexerStage::__InDivide Stage
    static void __nextTokenInDivideStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token __LexerStage::__InComment Stage
    static void __nextTokenInCommentStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &,
        string &, int &);


    // Next Token __LexerStage::__EndComment Stage
    static void __nextTokenEndCommentStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &,
        string &, int &);


    // Next Token __LexerStage::__InLess Stage
    static void __nextTokenInLessStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token __LexerStage::__InGreater Stage
    static void __nextTokenInGreaterStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token __LexerStage::__InAssign Stage
    static void __nextTokenInAssignStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
        string &tokenStr, int &);


    // Next Token __LexerStage::__InNot Stage
    static void __nextTokenInNotStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
        string &tokenStr, int &lineNo);


    // Next Token
    static __Token __nextToken(const char *&codePtr, int &lineNo);


    // Lexical Analysis
    vector<__Token> __lexicalAnalysis() const;
};


}  // End namespace CMM
