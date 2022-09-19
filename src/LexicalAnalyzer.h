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
// Class __LexicalAnalyzer
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
        string &tokenStr, int &lineNo);


    // Next Token
    static __Token __nextToken(const char *&codePtr, int &lineNo);


    // Lexical Analysis
    vector<__Token> __lexicalAnalysis() const;
};


}  // End namespace CMM
