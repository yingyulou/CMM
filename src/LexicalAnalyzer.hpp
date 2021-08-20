/*
    LexicalAnalyzer.hpp
    ===================
        Class LexicalAnalyzer implementation.
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
#include "Constants.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::getline;
using std::vector;
using std::ifstream;
using std::runtime_error;
using boost::format;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

LexicalAnalyzer::LexicalAnalyzer(const string &inputFilePath):
    __inputFilePath(inputFilePath) {}


////////////////////////////////////////////////////////////////////////////////
// Lexical Analysis
////////////////////////////////////////////////////////////////////////////////

vector<Token> LexicalAnalyzer::lexicalAnalysis() const
{
    return __lexicalAnalysis();
}


////////////////////////////////////////////////////////////////////////////////
// Invalid Char
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__invalidChar(char curChar, int lineNo)
{
    throw runtime_error((format("Invalid char: %c in line: %d") %
        curChar                                                 %
        lineNo
    ).str());
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::Start Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenStartStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &lineNo)
{
    if (isalpha(*codePtr))
    {
        lexerStage = LexerStage::InId;
        tokenStr += *codePtr++;
    }
    else if (isdigit(*codePtr))
    {
        lexerStage = LexerStage::InNumber;
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
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Plus;
                tokenStr += *codePtr++;
                break;

            case '-':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Minus;
                tokenStr += *codePtr++;
                break;

            case '*':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Multiply;
                tokenStr += *codePtr++;
                break;

            case '/':
                lexerStage = LexerStage::InDivide;
                codePtr++;
                break;

            case '<':
                lexerStage = LexerStage::InLess;
                tokenStr += *codePtr++;
                break;

            case '>':
                lexerStage = LexerStage::InGreater;
                tokenStr += *codePtr++;
                break;

            case '=':
                lexerStage = LexerStage::InAssign;
                tokenStr += *codePtr++;
                break;

            case '!':
                lexerStage = LexerStage::InNot;
                tokenStr += *codePtr++;
                break;

            case ';':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Semicolon;
                tokenStr += *codePtr++;
                break;

            case ',':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Comma;
                tokenStr += *codePtr++;
                break;

            case '(':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::LeftRoundBracket;
                tokenStr += *codePtr++;
                break;

            case ')':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::RightRoundBracket;
                tokenStr += *codePtr++;
                break;

            case '[':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::LeftSquareBracket;
                tokenStr += *codePtr++;
                break;

            case ']':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::RightSquareBracket;
                tokenStr += *codePtr++;
                break;

            case '{':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::LeftCurlyBracket;
                tokenStr += *codePtr++;
                break;

            case '}':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::RightCurlyBracket;
                tokenStr += *codePtr++;
                break;

            case '\0':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::END;
                break;

            default:
                __invalidChar(*codePtr, lineNo);
                break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InId Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInIDStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &)
{
    if (isalpha(*codePtr))
    {
        tokenStr += *codePtr++;
    }
    else
    {
        lexerStage = LexerStage::Done;

        tokenType = KEYWORD_MAP.count(tokenStr) ?
            KEYWORD_MAP.at(tokenStr) : TokenType::Id;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InNumber Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInNumberStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &)
{
    if (isdigit(*codePtr))
    {
        tokenStr += *codePtr++;
    }
    else
    {
        lexerStage = LexerStage::Done;
        tokenType = TokenType::Number;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InDivide Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInDivideStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &)
{
    if (*codePtr == '*')
    {
        lexerStage = LexerStage::InComment;
        codePtr++;
    }
    else
    {
        lexerStage = LexerStage::Done;
        tokenType = TokenType::Divide;
        tokenStr = "/";
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InComment Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInCommentStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &, string &, int &lineNo)
{
    if (*codePtr == '*')
    {
        lexerStage = LexerStage::EndComment;
    }
    else if (*codePtr == '\n')
    {
        lineNo++;
    }

    codePtr++;
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::EndComment Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenEndCommentStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &, string &, int &lineNo)
{
    if (*codePtr == '/')
    {
        lexerStage = LexerStage::Start;
    }
    else if (*codePtr != '*')
    {
        lexerStage = LexerStage::InComment;

        if (*codePtr == '\n')
        {
            lineNo++;
        }
    }

    codePtr++;
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InLess Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInLessStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &)
{
    lexerStage = LexerStage::Done;

    if (*codePtr == '=')
    {
        tokenType = TokenType::LessEqual;
        tokenStr += *codePtr++;
    }
    else
    {
        tokenType = TokenType::Less;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InGreater Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInGreaterStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &)
{
    lexerStage = LexerStage::Done;

    if (*codePtr == '=')
    {
        tokenType = TokenType::GreaterEqual;
        tokenStr += *codePtr++;
    }
    else
    {
        tokenType = TokenType::Greater;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InAssign Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInAssignStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &)
{
    lexerStage = LexerStage::Done;

    if (*codePtr == '=')
    {
        tokenType = TokenType::Equal;
        tokenStr += *codePtr++;
    }
    else
    {
        tokenType = TokenType::Assign;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token LexerStage::InNot Stage
////////////////////////////////////////////////////////////////////////////////

void LexicalAnalyzer::__nextTokenInNotStage(const char *&codePtr,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr, int &lineNo)
{
    if (*codePtr == '=')
    {
        lexerStage = LexerStage::Done;
        tokenType = TokenType::NotEqual;
        tokenStr += *codePtr++;
    }
    else
    {
        __invalidChar(*codePtr, lineNo);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Next Token
////////////////////////////////////////////////////////////////////////////////

Token LexicalAnalyzer::__nextToken(const char *&codePtr, int &lineNo)
{
    LexerStage lexerStage = LexerStage::Start;
    TokenType tokenType;
    string tokenStr;

    while (lexerStage != LexerStage::Done)
    {
        switch (lexerStage)
        {
            case LexerStage::Start:

                __nextTokenStartStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InId:

                __nextTokenInIDStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InNumber:

                __nextTokenInNumberStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InDivide:

                __nextTokenInDivideStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InComment:

                __nextTokenInCommentStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::EndComment:

                __nextTokenEndCommentStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InLess:

                __nextTokenInLessStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InGreater:

                __nextTokenInGreaterStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InAssign:

                __nextTokenInAssignStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            case LexerStage::InNot:

                __nextTokenInNotStage(codePtr, lexerStage, tokenType,
                    tokenStr, lineNo);

                break;

            default:

                throw runtime_error("Invalid LexerStage value");
        }
    }

    return Token(tokenType, tokenStr, lineNo);
}


////////////////////////////////////////////////////////////////////////////////
// Lexical Analysis
////////////////////////////////////////////////////////////////////////////////

vector<Token> LexicalAnalyzer::__lexicalAnalysis() const
{
    vector<Token> tokenList;

    ifstream f(__inputFilePath);
    string codeStr;
    int lineNo = 1;

    getline(f, codeStr, '\0');

    auto codePtr = codeStr.c_str();

    for (auto tokenObj = __nextToken(codePtr, lineNo);
        /* See below */;
        tokenObj = __nextToken(codePtr, lineNo))
    {
        tokenList.push_back(tokenObj);

        if (tokenObj.tokenType() == TokenType::END)
        {
            break;
        }
    }

    return tokenList;
}


}  // End namespace CMM
