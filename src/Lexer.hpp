/*
    Lexer.hpp
    =========
        Class Lexer implementation.
*/

#ifndef __CMM_LEXER_HPP
#define __CMM_LEXER_HPP

#include <string>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "Lexer.h"
#include "TokenType.hpp"
#include "LexerStage.hpp"
#include "Token.h"
#include "Constants.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

Lexer::Lexer(const string &inputFilePath):
    __filePtr(fopen(inputFilePath.c_str(), "r")) {}


////////////////////////////////////////////////////////////////////////////////
// NextToken
////////////////////////////////////////////////////////////////////////////////

Token Lexer::NextToken()
{
    LEXER_STAGE lexerStage = LEXER_STAGE::START;
    TOKEN_TYPE tokenType;
    string tokenStr;

    while (lexerStage != LEXER_STAGE::DONE)
    {
        int nowChar = fgetc(__filePtr);
        bool saveBool = true;

        switch (lexerStage)
        {
            case LEXER_STAGE::START:
                __nextTokenStartStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_ID:
                __nextTokenInIDStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_NUMBER:
                __nextTokenInNumberStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_DIVIDE:
                __nextTokenInDivideStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_COMMENT:
                __nextTokenInCommentStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::END_COMMENT:
                __nextTokenEndCommentStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_LESS:
                __nextTokenInLessStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_GREATER:
                __nextTokenInGreaterStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_ASSIGN:
                __nextTokenInAssignStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;

            case LEXER_STAGE::IN_NOT:
                __nextTokenInNotStage(nowChar, saveBool, lexerStage, tokenType, tokenStr);
                break;
        }

        if (saveBool)
        {
            tokenStr += nowChar;
        }
    }

    return {tokenType, tokenStr, __lineNo};
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
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    if (isalpha(nowChar))
    {
        lexerStage = LEXER_STAGE::IN_ID;
    }
    else if (isdigit(nowChar))
    {
        lexerStage = LEXER_STAGE::IN_NUMBER;
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
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::PLUS;
                break;

            case '-':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::MINUS;
                break;

            case '*':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::MULTIPLY;
                break;

            case '/':
                saveBool = false;
                lexerStage = LEXER_STAGE::IN_DIVIDE;
                break;

            case '<':
                lexerStage = LEXER_STAGE::IN_LESS;
                break;

            case '>':
                lexerStage = LEXER_STAGE::IN_GREATER;
                break;

            case '=':
                lexerStage = LEXER_STAGE::IN_ASSIGN;
                break;

            case '!':
                lexerStage = LEXER_STAGE::IN_NOT;
                break;

            case ';':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::SEMICOLON;
                break;

            case ',':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::COMMA;
                break;

            case '(':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::LEFT_ROUND_BRACKET;
                break;

            case ')':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::RIGHT_ROUND_BRACKET;
                break;

            case '[':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::LEFT_SQUARE_BRACKET;
                break;

            case ']':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::RIGHT_SQUARE_BRACKET;
                break;

            case '{':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::LEFT_CURLY_BRACKET;
                break;

            case '}':
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::RIGHT_CURLY_BRACKET;
                break;

            case EOF:
                lexerStage = LEXER_STAGE::DONE;
                tokenType = TOKEN_TYPE::END_OF_FILE;
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
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    if (!isalpha(nowChar))
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        lexerStage = LEXER_STAGE::DONE;
        tokenType = KEYWORD_MAP.count(tokenStr) ? KEYWORD_MAP.at(tokenStr) : TOKEN_TYPE::ID;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_NUMBER Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInNumberStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    if (!isdigit(nowChar))
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        lexerStage = LEXER_STAGE::DONE;
        tokenType = TOKEN_TYPE::NUMBER;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_DIVIDE Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInDivideStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    if (nowChar == '*')
    {
        saveBool = false;
        lexerStage = LEXER_STAGE::IN_COMMENT;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        lexerStage = LEXER_STAGE::DONE;
        tokenType = TOKEN_TYPE::DIVIDE;
        tokenStr = "/";
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_COMMENT Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInCommentStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    saveBool = false;

    if (nowChar == '*')
    {
        lexerStage = LEXER_STAGE::END_COMMENT;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken END_COMMENT Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenEndCommentStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    saveBool = false;

    if (nowChar == '/')
    {
        lexerStage = LEXER_STAGE::START;
    }
    else if (nowChar != '*')
    {
        lexerStage = LEXER_STAGE::IN_COMMENT;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_LESS Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInLessStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    lexerStage = LEXER_STAGE::DONE;

    if (nowChar == '=')
    {
        tokenType = TOKEN_TYPE::LESS_EQUAL;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        tokenType = TOKEN_TYPE::LESS;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_GREATER Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInGreaterStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    lexerStage = LEXER_STAGE::DONE;

    if (nowChar == '=')
    {
        tokenType = TOKEN_TYPE::GREATER_EQUAL;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        tokenType = TOKEN_TYPE::GREATER;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_ASSIGN Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInAssignStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    lexerStage = LEXER_STAGE::DONE;

    if (nowChar == '=')
    {
        tokenType = TOKEN_TYPE::EQUAL;
    }
    else
    {
        saveBool = false;
        ungetc(nowChar, __filePtr);
        tokenType = TOKEN_TYPE::ASSIGN;
    }
}


////////////////////////////////////////////////////////////////////////////////
// NextToken IN_NOT Stage Dispatch
////////////////////////////////////////////////////////////////////////////////

void Lexer::__nextTokenInNotStage(int nowChar, bool &saveBool,
    LEXER_STAGE &lexerStage, TOKEN_TYPE &tokenType, string &tokenStr)
{
    if (nowChar == '=')
    {
        lexerStage = LEXER_STAGE::DONE;
        tokenType = TOKEN_TYPE::NOT_EQUAL;
    }
    else
    {
        __invalidChar(nowChar, __lineNo);
    }
}


}  // End namespace CMM


#endif  // __CMM_LEXER_HPP
