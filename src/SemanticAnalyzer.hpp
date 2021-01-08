/*
    SemanticAnalyzer.hpp
    ====================
        Semantic analyzer implementation.
*/

#ifndef __CMM_SEMANTIC_ANALYZER_HPP
#define __CMM_SEMANTIC_ANALYZER_HPP

#include <string>
#include <unordered_map>
#include <utility>
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
// SemanticAnalyzer
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, unordered_map<string, pair<int, int>>> SemanticAnalyzer(AST *root)
{
    /*
        symbolTable: Function Name => Variable Name => (Variable Number, Array Size)
    */
    unordered_map<string, unordered_map<string, pair<int, int>>> symbolTable {{"__GLOBAL__", {}}};
    int globalIdx = 0;

    /*
        TOKEN_TYPE::DECL_LIST
            |
            |---- __Decl
            |
            |---- [__Decl]
            .
            .
            .
    */
    for (auto declNodePtr: root->subList)
    {
        /*
            __VarDecl | __FuncDecl
        */
        if (declNodePtr->tokenType == TOKEN_TYPE::FUNC_DECL)
        {
            /*
                TOKEN_TYPE::FUNC_DECL
                    |
                    |---- __Type
                    |
                    |---- TOKEN_TYPE::ID
                    |
                    |---- __Params
                    |
                    |---- __CompoundStmt
            */
            int varIdx = 0;
            string funcName = declNodePtr->subList[1]->tokenStr;

            /*
                __ParamList | TOKEN_TYPE::VOID
            */
            if (declNodePtr->subList[2]->tokenType == TOKEN_TYPE::PARAM_LIST)
            {
                /*
                    TOKEN_TYPE::PARAM_LIST
                        |
                        |---- __Param
                        |
                        |---- [__Param]
                        .
                        .
                        .
                */
                for (auto paramPtr: declNodePtr->subList[2]->subList)
                {
                    /*
                        TOKEN_TYPE::PARAM
                            |
                            |---- __Type
                            |
                            |---- TOKEN_TYPE::ID
                    */
                    string varName = paramPtr->subList[1]->tokenStr;

                    symbolTable[funcName][varName] = {varIdx++, 0};
                }
            }

            /*
                TOKEN_TYPE::COMPOUND_STMT
                    |
                    |---- __LocalDecl
                    |
                    |---- __StmtList


                TOKEN_TYPE::LOCAL_DECL
                    |
                    |---- [__VarDecl]
                    .
                    .
                    .
            */
            for (auto varDeclPtr: declNodePtr->subList[3]->subList[0]->subList)
            {
                /*
                    TOKEN_TYPE::VAR_DECL
                        |
                        |---- __Type
                        |
                        |---- TOKEN_TYPE::ID
                        |
                        |---- [TOKEN_TYPE::NUMBER]
                */
                string varName = varDeclPtr->subList[1]->tokenStr;

                int varSize = varDeclPtr->subList.size() == 2 ? 0 :
                    stoi(varDeclPtr->subList[2]->tokenStr);

                symbolTable[funcName][varName] = {varIdx, varSize};
                varIdx += varSize + 1;
            }
        }
        else
        {
            /*
                TOKEN_TYPE::VAR_DECL
                    |
                    |---- __Type
                    |
                    |---- TOKEN_TYPE::ID
                    |
                    |---- [TOKEN_TYPE::NUMBER]
            */
            string varName = declNodePtr->subList[1]->tokenStr;

            int varSize = declNodePtr->subList.size() == 2 ? 0 :
                stoi(declNodePtr->subList[2]->tokenStr);

            symbolTable["__GLOBAL__"][varName] = {globalIdx, varSize};
            globalIdx += varSize + 1;
        }
    }

    return symbolTable;
}


}  // End namespace CMM


#endif  // __CMM_SEMANTIC_ANALYZER_HPP
