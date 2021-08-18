/*
    Lexer.hpp
    =========
        Class Lexer implementation.
*/

#pragma once

#include <string>
#include <stdexcept>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "Lexer.h"
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
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

Lexer::Lexer(const string &inputFilePath):
    __filePtr(fopen(inputFilePath.c_str(), "r")),
    __lineNo (1) {}


////////////////////////////////////////////////////////////////////////////////
// Next Token
////////////////////////////////////////////////////////////////////////////////

Token Lexer::nextToken()
{
    LexerStage lexerStage = LexerStage::Start;
    TokenType tokenType;
    string tokenStr;

    while (lexerStage != LexerStage::Done)
    {
        int nowChar = fgetc(__filePtr);
        bool saveBool = true;

        switch (lexerStage)
        {
            case LexerStage::Start:
                __nextTokenStartStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InId:
                __nextTokenInIDStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InNumber:
                __nextTokenInNumberStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InDivide:
                __nextTokenInDivideStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InComment:
                __nextTokenInCommentStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::EndComment:
                __nextTokenEndCommentStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InLess:
                __nextTokenInLessStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InGreater:
                __nextTokenInGreaterStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InAssign:
                __nextTokenInAssignStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LexerStage::InNot:
                __nextTokenInNotStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            default:
                throw runtime_error("Invalid LexerStage");
        }

        if (saveBool)
        {
            tokenStr += nowChar;
        }
    }

    return Token(tokenType, tokenStr, __lineNo);
}


////////////////////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////////////////////

Lexer::~Lexer()
{
    fclose(__filePtr);
}


////////////////////////////////////////////////////////////////////////////////
// Invalid Char
////////////////////////////////////////////////////////////////////////////////

void Lexer::__invalidChar(char invalidChar, int lineNo)
{
    printf("Invalid char: %c in line: %d\n", invalidChar, lineNo);

    exit(1);
}


////////////////////////////////////////////////////////////////////////////////
// NextToken START Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenStartStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    if (isalpha(nowChar))
    {
        lexerStage = LexerStage::InId;
    }
    else if (isdigit(nowChar))
    {
        lexerStage = LexerStage::InNumber;
    }
    else if (isspace(nowChar))
    {
        saveBool = false;

        if (nowChar == '\n')
        {
            __lineNo++;
        }
    }
    else
    {
        switch (nowChar)
        {
            case '+':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Plus;
                break;

            case '-':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Minus;
                break;

            case '*':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Multiply;
                break;

            case '/':
                saveBool = false;
                lexerStage = LexerStage::InDivide;
                break;

            case '<':
                lexerStage = LexerStage::InLess;
                break;

            case '>':
                lexerStage = LexerStage::InGreater;
                break;

            case '=':
                lexerStage = LexerStage::InAssign;
                break;

            case '!':
                lexerStage = LexerStage::InNot;
                break;

            case ';':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Semicolon;
                break;

            case ',':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::Comma;
                break;

            case '(':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::LeftRoundBracket;
                break;

            case ')':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::RightRoundBracket;
                break;

            case '[':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::LeftSquareBracket;
                break;

            case ']':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::RightSquareBracket;
                break;

            case '{':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::LeftCurlyBracket;
                break;

            case '}':
                lexerStage = LexerStage::Done;
                tokenType = TokenType::RightCurlyBracket;
                break;

            case EOF:
                lexerStage = LexerStage::Done;
                tokenType = TokenType::EndOfFile;
                break;

            default:
                __invalidChar(nowChar, __lineNo);
                break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_ID Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInIDStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    if (!isalpha(nowChar))
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        lexerStage = LexerStage::Done;
        tokenType = KEYWORD_MAP.count(tokenStr) ? KEYWORD_MAP.at(tokenStr) : TokenType::Id;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_NUMBER Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInNumberStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    if (!isdigit(nowChar))
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        lexerStage = LexerStage::Done;
        tokenType = TokenType::Number;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_DIVIDE Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInDivideStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    if (nowChar == '*')
    {
        saveBool = false;
        lexerStage = LexerStage::InComment;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        lexerStage = LexerStage::Done;
        tokenType = TokenType::Divide;
        tokenStr = "/";
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_COMMENT Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInCommentStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    saveBool = false;

    if (nowChar == '*')
    {
        lexerStage = LexerStage::EndComment;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken END_COMMENT Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenEndCommentStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    saveBool = false;

    if (nowChar == '/')
    {
        lexerStage = LexerStage::Start;
    }
    else if (nowChar != '*')
    {
        lexerStage = LexerStage::InComment;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_LESS Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInLessStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    lexerStage = LexerStage::Done;

    if (nowChar == '=')
    {
        tokenType = TokenType::LessEqual;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        tokenType = TokenType::Less;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_GREATER Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInGreaterStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    lexerStage = LexerStage::Done;

    if (nowChar == '=')
    {
        tokenType = TokenType::GreaterEqual;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        tokenType = TokenType::Greater;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_ASSIGN Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInAssignStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    lexerStage = LexerStage::Done;

    if (nowChar == '=')
    {
        tokenType = TokenType::Equal;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        tokenType = TokenType::Assign;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_NOT Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInNotStage(int nowChar, bool &saveBool,
    LexerStage &lexerStage, TokenType &tokenType, string &tokenStr)
{
    if (nowChar == '=')
    {
        lexerStage = LexerStage::Done;
        tokenType = TokenType::NotEqual;
    }
    else
    {
        __invalidChar(nowChar, __lineNo);
    }
}


}  // End namespace CMM
