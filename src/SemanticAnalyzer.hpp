/*
    SemanticAnalyzer.hpp
    ====================
        Class SemanticAnalyzer  implementation.
*/

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include "SemanticAnalyzer.h"
#include "AST.h"
#include "TokenType.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::unordered_map;
using std::pair;
using std::stoi;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

SemanticAnalyzer::SemanticAnalyzer(AST *root):
    __root(root) {}


////////////////////////////////////////////////////////////////////////////////
// Getter: __symbolTable
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, unordered_map<string, pair<int, int>>>
SemanticAnalyzer::semanticAnalysis() const
{
    return __semanticAnalysis();
}


////////////////////////////////////////////////////////////////////////////////
// Get Symbol Table
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, unordered_map<string, pair<int, int>>>
SemanticAnalyzer::__semanticAnalysis() const
{
    /*
        symbolTable: Function Name => Variable Name => (Variable Number, Array Size)
    */
    unordered_map<string, unordered_map<string, pair<int, int>>> symbolTable
    {
        {"__GLOBAL__", {}}
    };

    int globalIdx = 0;

    /*
        TokenType::DeclList
            |
            |---- __Decl
            |
            |---- [__Decl]
            .
            .
            .
    */
    for (auto declNodePtr: __root->subList())
    {
        /*
            __VarDecl | __FuncDecl
        */
        if (declNodePtr->tokenType() == TokenType::FuncDecl)
        {
            /*
                TokenType::FuncDecl
                    |
                    |---- __Type
                    |
                    |---- TokenType::Id
                    |
                    |---- __Params
                    |
                    |---- __CompoundStmt
            */
            int varIdx = 0;
            string funcName = declNodePtr->subList()[1]->tokenStr();

            symbolTable[funcName];

            /*
                __ParamList | nullptr
            */
            if (declNodePtr->subList()[2])
            {
                /*
                    TokenType::ParamList
                        |
                        |---- __Param
                        |
                        |---- [__Param]
                        .
                        .
                        .
                */
                for (auto paramPtr: declNodePtr->subList()[2]->subList())
                {
                    /*
                        TokenType::Param
                            |
                            |---- __Type
                            |
                            |---- TokenType::Id
                    */
                    string varName = paramPtr->subList()[1]->tokenStr();

                    symbolTable[funcName][varName] = {varIdx++, 0};
                }
            }

            /*
                TokenType::CompoundStmt
                    |
                    |---- __LocalDecl
                    |
                    |---- __StmtList


                TokenType::LocalDecl
                    |
                    |---- [__VarDecl]
                    .
                    .
                    .
            */
            for (auto varDeclPtr: declNodePtr->subList()[3]->subList()[0]->subList())
            {
                /*
                    TokenType::VarDecl
                        |
                        |---- __Type
                        |
                        |---- TokenType::Id
                        |
                        |---- [TokenType::Number]
                */
                string varName = varDeclPtr->subList()[1]->tokenStr();

                int varSize = varDeclPtr->subList().size() == 2 ? 0 :
                    stoi(varDeclPtr->subList()[2]->tokenStr());

                symbolTable[funcName][varName] = {varIdx, varSize};
                varIdx += varSize + 1;
            }
        }
        else
        {
            /*
                TokenType::VarDecl
                    |
                    |---- __Type
                    |
                    |---- TokenType::Id
                    |
                    |---- [TokenType::Number]
            */
            string varName = declNodePtr->subList()[1]->tokenStr();

            int varSize = declNodePtr->subList().size() == 2 ? 0 :
                stoi(declNodePtr->subList()[2]->tokenStr());

            symbolTable["__GLOBAL__"][varName] = {globalIdx, varSize};
            globalIdx += varSize + 1;
        }
    }

    return symbolTable;
}


}  // End namespace CMM
