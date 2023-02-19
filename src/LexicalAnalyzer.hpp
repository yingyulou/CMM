/*
    LexicalAnalyzer.hpp
    ===================
        Class __LexicalAnalyzer implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <boost/format.hpp>
#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenType.hpp"
#include "LexerStage.hpp"
#include "Constants.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::getline;
using std::vector;
using std::ifstream;
using std::runtime_error;
using boost::format;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__LexicalAnalyzer::__LexicalAnalyzer(const string &inputFilePath):
    __inputFilePath(inputFilePath) {}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Lexical Analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<Token> LexicalAnalyzer::lexicalAnalysis() const
{
    return __lexicalAnalysis();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
=======
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
// Invalid Char
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__invalidChar(char curChar, int lineNo)
{
    throw runtime_error((format("Invalid char: %c in line: %d") %
        curChar                                                 %
        lineNo
    ).str());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::Start Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenStartStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__Start Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenStartStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &lineNo)
{
    if (isalpha(*codePtr))
    {
        lexerStage = __LexerStage::__InId;
        tokenStr += *codePtr++;
    }
    else if (isdigit(*codePtr))
    {
        lexerStage = __LexerStage::__InNumber;
        tokenStr += *codePtr++;
    }
    else if (isspace(*codePtr))
    {
        if (*codePtr == '\n')
        {
            lineNo++;
        }

        codePtr++;
    }
    else
    {
        switch (*codePtr)
        {
            case '+':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__Plus;
                tokenStr += *codePtr++;
                break;

            case '-':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__Minus;
                tokenStr += *codePtr++;
                break;

            case '*':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__Multiply;
                tokenStr += *codePtr++;
                break;

            case '/':
                lexerStage = __LexerStage::__InDivide;
                codePtr++;
                break;

            case '<':
                lexerStage = __LexerStage::__InLess;
                tokenStr += *codePtr++;
                break;

            case '>':
                lexerStage = __LexerStage::__InGreater;
                tokenStr += *codePtr++;
                break;

            case '=':
                lexerStage = __LexerStage::__InAssign;
                tokenStr += *codePtr++;
                break;

            case '!':
                lexerStage = __LexerStage::__InNot;
                tokenStr += *codePtr++;
                break;

            case ';':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__Semicolon;
                tokenStr += *codePtr++;
                break;

            case ',':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__Comma;
                tokenStr += *codePtr++;
                break;

            case '(':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__LeftRoundBracket;
                tokenStr += *codePtr++;
                break;

            case ')':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__RightRoundBracket;
                tokenStr += *codePtr++;
                break;

            case '[':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__LeftSquareBracket;
                tokenStr += *codePtr++;
                break;

            case ']':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__RightSquareBracket;
                tokenStr += *codePtr++;
                break;

            case '{':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__LeftCurlyBracket;
                tokenStr += *codePtr++;
                break;

            case '}':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__RightCurlyBracket;
                tokenStr += *codePtr++;
                break;

            case '\0':
                lexerStage = __LexerStage::__Done;
                tokenType = __TokenType::__End;
                break;

            default:
                __invalidChar(*codePtr, lineNo);
                break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InId Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInIDStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InId Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInIDStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &)
{
    if (isalpha(*codePtr))
    {
        tokenStr += *codePtr++;
    }
    else
    {
<<<<<<< HEAD
        lexerStage = LexerStage::Done;
        tokenType  = KEYWORD_MAP.count(tokenStr) ? KEYWORD_MAP.at(tokenStr) : TokenType::Id;
=======
        lexerStage = __LexerStage::__Done;

        tokenType = __Constants::__KEYWORD_MAP.count(tokenStr) ? __Constants::__KEYWORD_MAP.at(tokenStr) : __TokenType::__Id;
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InNumber Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInNumberStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InNumber Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInNumberStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &)
{
    if (isdigit(*codePtr))
    {
        tokenStr += *codePtr++;
    }
    else
    {
<<<<<<< HEAD
        lexerStage = LexerStage::Done;
        tokenType  = TokenType::Number;
=======
        lexerStage = __LexerStage::__Done;
        tokenType = __TokenType::__Number;
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InDivide Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInDivideStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InDivide Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInDivideStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &)
{
    if (*codePtr == '*')
    {
        lexerStage = __LexerStage::__InComment;
        codePtr++;
    }
    else
    {
<<<<<<< HEAD
        lexerStage = LexerStage::Done;
        tokenType  = TokenType::Divide;
        tokenStr   = "/";
=======
        lexerStage = __LexerStage::__Done;
        tokenType = __TokenType::__Divide;
        tokenStr = "/";
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InComment Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInCommentStage(const char *&codePtr, LexerStage &lexerStage, TokenType &,
=======
// Next Token __LexerStage::__InComment Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInCommentStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &, int &lineNo)
{
    if (*codePtr == '*')
    {
        lexerStage = __LexerStage::__EndComment;
    }
    else if (*codePtr == '\n')
    {
        lineNo++;
    }

    codePtr++;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::EndComment Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenEndCommentStage(const char *&codePtr, LexerStage &lexerStage, TokenType &,
=======
// Next Token __LexerStage::__EndComment Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenEndCommentStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &, int &lineNo)
{
    if (*codePtr == '/')
    {
        lexerStage = __LexerStage::__Start;
    }
    else if (*codePtr != '*')
    {
        lexerStage = __LexerStage::__InComment;

        if (*codePtr == '\n')
        {
            lineNo++;
        }
    }

    codePtr++;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InLess Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInLessStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InLess Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInLessStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &)
{
    lexerStage = __LexerStage::__Done;

    if (*codePtr == '=')
    {
        tokenType = __TokenType::__LessEqual;
        tokenStr += *codePtr++;
    }
    else
    {
        tokenType = __TokenType::__Less;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InGreater Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInGreaterStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InGreater Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInGreaterStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &)
{
    lexerStage = __LexerStage::__Done;

    if (*codePtr == '=')
    {
        tokenType = __TokenType::__GreaterEqual;
        tokenStr += *codePtr++;
    }
    else
    {
        tokenType = __TokenType::__Greater;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InAssign Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInAssignStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InAssign Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInAssignStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &)
{
    lexerStage = __LexerStage::__Done;

    if (*codePtr == '=')
    {
        tokenType = __TokenType::__Equal;
        tokenStr += *codePtr++;
    }
    else
    {
        tokenType = __TokenType::__Assign;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Next Token LexerStage::InNot Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInNotStage(const char *&codePtr, LexerStage &lexerStage, TokenType &tokenType,
=======
// Next Token __LexerStage::__InNot Stage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __LexicalAnalyzer::__nextTokenInNotStage(const char *&codePtr, __LexerStage &lexerStage, __TokenType &tokenType,
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
    string &tokenStr, int &lineNo)
{
    if (*codePtr == '=')
    {
        lexerStage = __LexerStage::__Done;
        tokenType = __TokenType::__NotEqual;
        tokenStr += *codePtr++;
    }
    else
    {
        __invalidChar(*codePtr, lineNo);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Next Token
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__Token __LexicalAnalyzer::__nextToken(const char *&codePtr, int &lineNo)
{
    __LexerStage lexerStage = __LexerStage::__Start;
    __TokenType tokenType;
    string tokenStr;

    while (lexerStage != __LexerStage::__Done)
    {
        switch (lexerStage)
        {
<<<<<<< HEAD
            case LexerStage::Start:
                __nextTokenStartStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InId:
                __nextTokenInIDStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InNumber:
                __nextTokenInNumberStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InDivide:
                __nextTokenInDivideStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InComment:
                __nextTokenInCommentStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::EndComment:
                __nextTokenEndCommentStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InLess:
                __nextTokenInLessStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InGreater:
                __nextTokenInGreaterStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InAssign:
                __nextTokenInAssignStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case LexerStage::InNot:
=======
            case __LexerStage::__Start:
                __nextTokenStartStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InId:
                __nextTokenInIDStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InNumber:
                __nextTokenInNumberStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InDivide:
                __nextTokenInDivideStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InComment:
                __nextTokenInCommentStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__EndComment:
                __nextTokenEndCommentStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InLess:
                __nextTokenInLessStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InGreater:
                __nextTokenInGreaterStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InAssign:
                __nextTokenInAssignStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            case __LexerStage::__InNot:
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
                __nextTokenInNotStage(codePtr, lexerStage, tokenType, tokenStr, lineNo);
                break;

            default:
<<<<<<< HEAD
                throw runtime_error("Invalid LexerStage value");
=======
                throw runtime_error("Invalid __LexerStage value");
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
        }
    }

    return __Token(tokenType, tokenStr, lineNo);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lexical Analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<__Token> __LexicalAnalyzer::__lexicalAnalysis() const
{
    vector<__Token> tokenList;

    ifstream f(__inputFilePath);
    string codeStr;
    int lineNo = 1;

    getline(f, codeStr, '\0');

    auto codePtr = codeStr.c_str();

    for (auto tokenObj = __nextToken(codePtr, lineNo); /* See below */; tokenObj = __nextToken(codePtr, lineNo))
    {
        tokenList.push_back(tokenObj);

        if (tokenObj.__tokenType == __TokenType::__End)
        {
            break;
        }
    }

    return tokenList;
}


}  // End namespace CMM
