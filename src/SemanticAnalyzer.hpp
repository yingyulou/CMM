/*
    SemanticAnalyzer.hpp
    ====================
        Class __SemanticAnalyzer  implementation.
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::unordered_map;
using std::pair;
using std::stoi;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__SemanticAnalyzer::__SemanticAnalyzer(__AST *root):
    __root(root) {}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get Symbol Table
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unordered_map<string, unordered_map<string, pair<int, int>>> __SemanticAnalyzer::__semanticAnalysis() const
{
    /*
        symbolTable: Function Name => Variable Name => (Variable Number, Array Size)
    */
    unordered_map<string, unordered_map<string, pair<int, int>>> symbolTable {{"__GLOBAL__", {}}};

    int globalIdx = 0;

    /*
        __TokenType::__Program
        |
        |---- __Decl
        |
        |---- [__Decl]
        .
        .
        .
    */
    for (auto declNodePtr: __root->__subList)
    {
        /*
            __VarDecl | __FuncDecl
        */
        if (declNodePtr->__tokenType == __TokenType::__VarDecl)
        {
            /*
                __TokenType::__VarDecl
                |
                |---- __Type
                |
                |---- __TokenType::__Id
                |
                |---- [__TokenType::__Number]
            */
            string varName = declNodePtr->__subList[1]->__tokenStr;

            int varSize = !declNodePtr->__subList[2] ? 0 : stoi(declNodePtr->__subList[2]->__tokenStr);

            symbolTable["__GLOBAL__"][varName] = {globalIdx, varSize};
            globalIdx += varSize + 1;
        }
        else
        {
            /*
                __TokenType::__FuncDecl
                |
                |---- __Type
                |
                |---- __TokenType::__Id
                |
                |---- [__ParamList]
                |
                |---- __LocalDecl
                |
                |---- __StmtList
            */
            int varIdx = 0;
            string funcName = declNodePtr->__subList[1]->__tokenStr;

            symbolTable[funcName];

            /*
                [__ParamList]
            */
            if (declNodePtr->__subList[2])
            {
                /*
                    __TokenType::__ParamList
                    |
                    |---- __Param
                    |
                    |---- [__Param]
                    .
                    .
                    .
                */
                for (auto paramPtr: declNodePtr->__subList[2]->__subList)
                {
                    /*
                        __TokenType::__Param
                        |
                        |---- __Type
                        |
                        |---- __TokenType::__Id
                    */
                    string varName = paramPtr->__subList[1]->__tokenStr;

                    symbolTable[funcName][varName] = {varIdx++, 0};
                }
            }

            /*
                __TokenType::__LocalDecl
                |
                |---- [__VarDecl]
                .
                .
                .
            */
            for (auto varDeclPtr: declNodePtr->__subList[3]->__subList)
            {
                /*
                    __TokenType::__VarDecl
                    |
                    |---- __Type
                    |
                    |---- __TokenType::__Id
                    |
                    |---- [__TokenType::__Number]
                */
                string varName = varDeclPtr->__subList[1]->__tokenStr;

                int varSize = !varDeclPtr->__subList[2] ? 0 : stoi(varDeclPtr->__subList[2]->__tokenStr);

                symbolTable[funcName][varName] = {varIdx, varSize};
                varIdx += varSize + 1;
            }
        }
    }

    return symbolTable;
}


}  // End namespace CMM
