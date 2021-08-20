/*
    LexicalAnalyzer.h
    =================
        Class LexicalAnalyzer header.
*/

#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "TokenType.hpp"
#include "LexerStage.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;


////////////////////////////////////////////////////////////////////////////////
// Class LexicalAnalyzer
////////////////////////////////////////////////////////////////////////////////

class LexicalAnalyzer
{
public:

    // Constructor
    explicit LexicalAnalyzer(const string &inputFilePath = "");


    // Lexical Analysis
    vector<Token> lexicalAnalysis() const;


private:

    // Attribute
    string __inputFilePath;


    // Invalid Char
    static void __invalidChar(char curChar, int lineNo);


    // Next Token LexerStage::Start Stage
    static void __nextTokenStartStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &lineNo);


    // Next Token LexerStage::InId Stage
    static void __nextTokenInIDStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &);


    // Next Token LexerStage::InNumber Stage
    static void __nextTokenInNumberStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &);


    // Next Token LexerStage::InDivide Stage
    static void __nextTokenInDivideStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &);


    // Next Token LexerStage::InComment Stage
    static void __nextTokenInCommentStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &, string &, int &);


    // Next Token LexerStage::EndComment Stage
    static void __nextTokenEndCommentStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &, string &, int &);


    // Next Token LexerStage::InLess Stage
    static void __nextTokenInLessStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &);


    // Next Token LexerStage::InGreater Stage
    static void __nextTokenInGreaterStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &);


    // Next Token LexerStage::InAssign Stage
    static void __nextTokenInAssignStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &);


    // Next Token LexerStage::InNot Stage
    static void __nextTokenInNotStage(const char *&codePtr,
        LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &lineNo);


    // Next Token
    static Token __nextToken(const char *&codePtr, int &lineNo);


    // Lexical Analysis
    vector<Token> __lexicalAnalysis() const;
};


}  // End namespace CMM
