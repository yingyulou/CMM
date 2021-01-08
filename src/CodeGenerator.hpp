/*
    CodeGenerator.hpp
    =================
        Class CodeGenerator implementation.
*/

#ifndef __CMM_CODE_GENERATOR_HPP
#define __CMM_CODE_GENERATOR_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include "CodeGenerator.h"
#include "AST.h"
#include "SemanticAnalyzer.hpp"
#include "TokenType.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::sort;
using std::to_string;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

CodeGenerator::CodeGenerator(AST *root, const string &outputFilePath):
    __root(root),
    __symbolTable(SemanticAnalyzer(root)),
    __outputFilePath(outputFilePath) {}


////////////////////////////////////////////////////////////////////////////////
// Generate Code
////////////////////////////////////////////////////////////////////////////////

void CodeGenerator::GenerateCode()
{
    auto codeMap = __createCodeMap();

    __translateCall(codeMap);

    auto globalVariableCodeList = __generateGlobalVariableCode(),
        mainPrepareCodeList = __generateMainPrepareCode();

    __outputCode(codeMap, globalVariableCodeList, mainPrepareCodeList);
}


////////////////////////////////////////////////////////////////////////////////
// Generate Number Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateNumberCode(AST *root) const
{
    /*
        TOKEN_TYPE::NUMBER
    */

    return {"LDC " + root->tokenStr};
}


////////////////////////////////////////////////////////////////////////////////
// Generate CompoundStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateCompoundStmtCode(AST *root) const
{
    /*
        TOKEN_TYPE::COMPOUND_STMT
            |
            |---- __LocalDecl
            |
            |---- __StmtList
    */

    return __generateStmtListCode(root->subList[1]);
}


////////////////////////////////////////////////////////////////////////////////
// Generate StmtList Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateStmtListCode(AST *root) const
{
    /*
        TOKEN_TYPE::STMT_LIST
            |
            |---- [__Stmt]
            .
            .
            .
    */

    vector<string> codeList;

    for (auto stmtPtr: root->subList)
    {
        vector<string> stmtCodeList = __generateStmtCode(stmtPtr);

        codeList.insert(codeList.end(), stmtCodeList.begin(), stmtCodeList.end());
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Stmt Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateStmtCode(AST *root) const
{
    /*
        __ExprStmt | __CompoundStmt | __IfStmt | __WhileStmt | __ReturnStmt

        (__ExprStmt AST: __Expr | nullptr)
    */

    if (!root)
    {
        return {};
    }

    switch (root->tokenType)
    {
        case TOKEN_TYPE::EXPR:
            return __generateExprCode(root);

        case TOKEN_TYPE::COMPOUND_STMT:
            return __generateCompoundStmtCode(root);

        case TOKEN_TYPE::IF_STMT:
            return __generateIfStmtCode(root);

        case TOKEN_TYPE::WHILE_STMT:
            return __generateWhileStmtCode(root);

        case TOKEN_TYPE::RETURN_STMT:
        default:
            return __generateReturnStmtCode(root);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate IfStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateIfStmtCode(AST *root) const
{
    /*
        TOKEN_TYPE::IF_STMT
            |
            |---- __Expr
            |
            |---- __Stmt
            |
            |---- [__Stmt]
    */

    vector<string> codeList = __generateExprCode(root->subList[0]),
        ifCodeList = __generateStmtCode(root->subList[1]);

    if (root->subList.size() == 2)
    {
        /*
            if ...

                JZ END

                ...

            END: ...
        */
        codeList.push_back("JZ " + to_string(ifCodeList.size() + 1));
        codeList.insert(codeList.end(), ifCodeList.begin(), ifCodeList.end());
    }
    else
    {
        /*
            if ...

                JZ ELSE

                ...

                JMP END

            else

                ELSE: ...

            END: ...
        */
        vector<string> elseCodeList = __generateStmtCode(root->subList[2]);

        ifCodeList.push_back("JMP " + to_string(elseCodeList.size() + 1));
        codeList.push_back("JZ " + to_string(ifCodeList.size() + 1));
        codeList.insert(codeList.end(), ifCodeList.begin(), ifCodeList.end());
        codeList.insert(codeList.end(), elseCodeList.begin(), elseCodeList.end());
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate WhileStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateWhileStmtCode(AST *root) const
{
    /*
        TOKEN_TYPE::WHILE_STMT
            |
            |---- __Expr
            |
            |---- __Stmt
    */

    vector<string> codeList = __generateExprCode(root->subList[0]),
        stmtCodeList = __generateStmtCode(root->subList[1]);

    /*
        START: while ...

            JZ END

            ...

            JMP START

        END: ...
    */
    codeList.push_back("JZ " + to_string(stmtCodeList.size() + 2));
    codeList.insert(codeList.end(), stmtCodeList.begin(), stmtCodeList.end());
    codeList.push_back("JMP -" + to_string(codeList.size()));

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate ReturnStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateReturnStmtCode(AST *root) const
{
    /*
        TOKEN_TYPE::RETURN_STMT
            |
            |---- [__Expr]
    */

    if (root->subList.empty())
    {
        return {};
    }
    else
    {
        return __generateExprCode(root->subList[0]);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Expr Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateExprCode(AST *root) const
{
    /*
        TOKEN_TYPE::EXPR
            |
            |---- __Var
            |
            |---- __Expr

        ----------------------

        TOKEN_TYPE::EXPR
            |
            |---- __SimpleExpr
    */

    if (root->subList.size() == 1)
    {
        return __generateSimpleExprCode(root->subList[0]);
    }
    else
    {
        vector<string> codeList = __generateExprCode(root->subList[1]),
            assignCodeList = __generateAssignCode(root->subList[0]);

        codeList.insert(codeList.end(), assignCodeList.begin(), assignCodeList.end());

        return codeList;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Var Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateVarCode(AST *root) const
{
    /*
        TOKEN_TYPE::VAR
            |
            |---- TOKEN_TYPE::ID
            |
            |---- [__Expr]
    */

    vector<string> codeList;

    // Local variable
    if (__symbolTable.at(__nowFuncName).count(root->subList[0]->tokenStr))
    {
        codeList.push_back("LDC " + to_string(
            __symbolTable.at(__nowFuncName).at(root->subList[0]->tokenStr).first));

        codeList.push_back("LD");
    }
    // Global variable
    else
    {
        codeList.push_back("LDC " + to_string(
            __symbolTable.at("__GLOBAL__").at(root->subList[0]->tokenStr).first));

        codeList.push_back("ABSLD");
    }

    // Array
    if (root->subList.size() == 2)
    {
        vector<string> exprCodeList = __generateExprCode(root->subList[1]);

        codeList.push_back("PUSH");
        codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());
        codeList.push_back("ADD");
        codeList.push_back("POP");
        codeList.push_back("ABSLD");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate SimpleExpr Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateSimpleExprCode(AST *root) const
{
    /*
        TOKEN_TYPE::SIMPLE_EXPR
            |
            |---- __AddExpr
            |
            |---- [__RelOp]
            |
            |---- [__AddExpr]
    */

    if (root->subList.size() == 1)
    {
        return __generateAddExprCode(root->subList[0]);
    }
    else
    {
        vector<string> codeList = __generateAddExprCode(root->subList[0]),
            midCodeList = __generateRelOpCode(root->subList[1]),
            rightCodeList = __generateAddExprCode(root->subList[2]);

        codeList.push_back("PUSH");
        codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
        codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());
        codeList.push_back("POP");

        return codeList;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate RelOp Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateRelOpCode(AST *root) const
{
    switch (root->tokenType)
    {
        case TOKEN_TYPE::LESS:
            return {"LT"};

        case TOKEN_TYPE::LESS_EQUAL:
            return {"LE"};

        case TOKEN_TYPE::GREATER:
            return {"GT"};

        case TOKEN_TYPE::GREATER_EQUAL:
            return {"GE"};

        case TOKEN_TYPE::EQUAL:
            return {"EQ"};

        case TOKEN_TYPE::NOT_EQUAL:
        default:
            return {"NE"};
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate AddExpr Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateAddExprCode(AST *root) const
{
    /*
        TOKEN_TYPE::ADD_EXPR
            |
            |---- __Term
            |
            |---- [__AddOp]
            |
            |---- [__Term]
            .
            .
            .
    */

    vector<string> codeList = __generateTermCode(root->subList[0]);

    for (int idx = 1; idx < root->subList.size(); idx += 2)
    {
        codeList.push_back("PUSH");

        vector<string> midCodeList = __generateAddOpCode(root->subList[idx]),
            rightCodeList = __generateTermCode(root->subList[idx + 1]);

        codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
        codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());
        codeList.push_back("POP");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate AddOp Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateAddOpCode(AST *root) const
{
    /*
        TOKEN_TYPE::PLUS | TOKEN_TYPE::MINUS
    */

    if (root->tokenType == TOKEN_TYPE::PLUS)
    {
        return {"ADD"};
    }
    else
    {
        return {"SUB"};
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Term Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateTermCode(AST *root) const
{
    /*
        TOKEN_TYPE::TERM
            |
            |---- __Factor
            |
            |---- [__MulOp]
            |
            |---- [__Factor]
            .
            .
            .
    */

    vector<string> codeList = __generateFactorCode(root->subList[0]);

    for (int idx = 1; idx < root->subList.size(); idx += 2)
    {
        vector<string> midCodeList = __generateMulOpCode(root->subList[idx]),
            rightCodeList = __generateFactorCode(root->subList[idx + 1]);

        codeList.push_back("PUSH");
        codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
        codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());
        codeList.push_back("POP");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate MulOp Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateMulOpCode(AST *root) const
{
    /*
        TOKEN_TYPE::MULTIPLY | TOKEN_TYPE::DIVIDE
    */

    if (root->tokenType == TOKEN_TYPE::MULTIPLY)
    {
        return {"MUL"};
    }
    else
    {
        return {"DIV"};
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Factor Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateFactorCode(AST *root) const
{
    /*
        __Expr | TOKEN_TYPE::NUMBER | __Call | __Var
    */

    switch (root->tokenType)
    {
        case TOKEN_TYPE::EXPR:
            return __generateExprCode(root);

        case TOKEN_TYPE::NUMBER:
            return __generateNumberCode(root);

        case TOKEN_TYPE::CALL:
            return __generateCallCode(root);

        case TOKEN_TYPE::VAR:
        default:
            return __generateVarCode(root);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Call Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateCallCode(AST *root) const
{
    /*
        TOKEN_TYPE::CALL
            |
            |---- TOKEN_TYPE::ID
            |
            |---- [__ArgList]
    */

    // xxx = input();
    if (root->subList[0]->tokenStr == "input")
    {
        return {"INPUT"};
    }
    // output(xxx);
    else if (root->subList[0]->tokenStr == "output")
    {
        /*
            TOKEN_TYPE::ARG_LIST
                |
                |---- __Expr
        */
        vector<string> codeList = __generateExprCode(root->subList[1]->subList[0]);

        codeList.push_back("OUTPUT");

        return codeList;
    }

    vector<string> codeList;

    vector<pair<string, pair<int, int>>> pairList(
        __symbolTable.at(root->subList[0]->tokenStr).begin(),
        __symbolTable.at(root->subList[0]->tokenStr).end());

    // ..., Local5, Local4, Local3, Param2, Param1, Param0
    sort(pairList.begin(), pairList.end(),
        [] (const pair<string, pair<int, int>> &lhs, const pair<string, pair<int, int>> &rhs)
        {
            return lhs.second.first > rhs.second.first;
        }
    );

    // We only need local variable here
    int topIdx = pairList.size() - (root->subList.size() == 2 ?

        // Call function by at least one parameter
        root->subList[1]->subList.size() :

        // Call function without any parameter
        0);

    // Push local variable
    for (int idx = 0; idx < topIdx; idx++)
    {
        // Array
        if (pairList[idx].second.second)
        {
            // Push array content (By array size times)
            for (int _ = 0; _ < pairList[idx].second.second; _++)
            {
                codeList.push_back("PUSH");
            }

            /*
                The following code calc the array start pointer (Absolute index in SS).

                SS:

                    ... X X X X X X X X X X  PUSH
                        ^^^^^^^^^^^^^^^^^^^   ^
                        |    Size = 10    |   |
                        |                 SP  |
                  SP - (10 - 1) ---------------
            */
            codeList.push_back("PUSHSP");
            codeList.push_back("LDC " + to_string(pairList[idx].second.second - 1));
            codeList.push_back("SUB");
            codeList.push_back("POP");
            codeList.push_back("PUSH");
        }
        // Scalar
        else
        {
            codeList.push_back("PUSH");
        }
    }

    // Push parameter
    if (root->subList.size() == 2)
    {
        vector<string> argListCodeList = __generateArgListCode(root->subList[1]);

        codeList.insert(codeList.end(), argListCodeList.begin(), argListCodeList.end());
    }

    /*
        The following code prepare for CALL.

        1. PUSHBP
        2. SAVSP

        Now the SS is like:

        ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP
                                                      ^^^^^
                                                       BP (So LD needs "-1")

        3. PUSHIP

        Now the SS is like:

        ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP OldIP
                                                      ^^^^^
                                                       BP
    */
    codeList.push_back("PUSHBP");
    codeList.push_back("SAVSP");
    codeList.push_back("PUSHIP");

    // CALL will be translated to a JMP later (See function: __translateCall)
    codeList.push_back("CALL " + root->subList[0]->tokenStr);

    /*
        After call, POPIP will be executed.

        POPIP must use IP++ (don't forget the IP++ in VM) to overgo the PUSHIP and JMP(CALL):

        ... PUSHIP JMP(CALL) POPBP ...
            ^^^^^^ ^^^^^^^^^ ^^^^^
            OldIP      |       |
                       |       |
                 IP++ by POPIP |
                               |
                           IP++ by VM


        After POPIP, the SS is like:

        ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP

        So we need POPBP to restore BP, then several POP to pop all variables.
    */
    codeList.push_back("POPBP");

    // Pop all variables
    for (auto &mapPair: __symbolTable.at(root->subList[0]->tokenStr))
    {
        // Any variable needs a POP
        codeList.push_back("POP");

        // Pop array content (By array size times)
        for (int _ = 0; _ < mapPair.second.second; _++)
        {
            codeList.push_back("POP");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate ArgList Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateArgListCode(AST *root) const
{
    /*
        TOKEN_TYPE::ARG_LIST
            |
            |---- __Expr
            |
            |---- [__Expr]
            .
            .
            .
    */

    vector<string> codeList;

    for (int idx = root->subList.size() - 1; idx >= 0; idx--)
    {
        vector<string> exprCodeList = __generateExprCode(root->subList[idx]);

        codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());
        codeList.push_back("PUSH");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Assign Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateAssignCode(AST *root) const
{
    /*
        TOKEN_TYPE::EXPR
            |
            |---- __Var
            |     ^^^^^  -> Root
            |
            |---- __Expr
                  ^^^^^^ -> AX
    */

    vector<string> codeList {"PUSH"};

    // Local variable
    if (__symbolTable.at(__nowFuncName).count(root->subList[0]->tokenStr))
    {
        codeList.push_back("LDC " + to_string(
            __symbolTable.at(__nowFuncName).at(root->subList[0]->tokenStr).first));

        // Scalar
        if (root->subList.size() == 1)
        {
            codeList.push_back("SAV");
        }
        // Array
        else
        {
            vector<string> exprCodeList = __generateExprCode(root->subList[1]);

            // Get the (start) pointer (Is already an absolute address)
            codeList.push_back("LD");
            codeList.push_back("PUSH");
            codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

            // Pointer[Index] (Pointer + Index)
            codeList.push_back("ADD");
            codeList.push_back("POP");

            // Save by absolute address
            codeList.push_back("ABSSAV");
        }
    }
    // Global variable
    else
    {
        codeList.push_back("LDC " + to_string(
            __symbolTable.at("__GLOBAL__").at(root->subList[0]->tokenStr).first));

        // Scalar
        if (root->subList.size() == 1)
        {
            codeList.push_back("ABSSAV");
        }
        // Array
        else
        {
            vector<string> exprCodeList = __generateExprCode(root->subList[1]);

            // Absolute get the (start) pointer (Is already an absolute address)
            codeList.push_back("ABSLD");
            codeList.push_back("PUSH");
            codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

            // Pointer[Index] (Pointer + Index)
            codeList.push_back("ADD");
            codeList.push_back("POP");

            // Save by absolute address
            codeList.push_back("ABSSAV");
        }
    }

    codeList.push_back("POP");

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Create CodeMap
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, vector<string>> CodeGenerator::__createCodeMap()
{
    unordered_map<string, vector<string>> codeMap;

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
    for (auto declPtr: __root->subList)
    {
        /*
            __VarDecl | __FuncDecl
        */
        if (declPtr->tokenType == TOKEN_TYPE::FUNC_DECL)
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
            __nowFuncName = declPtr->subList[1]->tokenStr;

            /*
                TOKEN_TYPE::COMPOUND_STMT
                    |
                    |---- __LocalDecl
                    |
                    |---- __StmtList
            */
            vector<string> codeList = __generateStmtListCode(declPtr->subList[3]->subList[1]);

            codeList.push_back(__nowFuncName == "main" ? "STOP" : "POPIP");

            codeMap[__nowFuncName] = codeList;
        }
    }

    return codeMap;
}


////////////////////////////////////////////////////////////////////////////////
// Create FuncJmpMap
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, int> CodeGenerator::__createFuncJmpMap(
    const unordered_map<string, vector<string>> &codeMap) const
{
    // funcJmpMap: Function name => Function start IP
    unordered_map<string, int> funcJmpMap;

    // "main" function must be the first function
    int jmpNum = codeMap.at("main").size();

    // Other function
    for (auto &mapPair: codeMap)
    {
        if (mapPair.first != "main")
        {
            funcJmpMap[mapPair.first] = jmpNum;
            jmpNum += mapPair.second.size();
        }
    }

    return funcJmpMap;
}


////////////////////////////////////////////////////////////////////////////////
// Translate Call
////////////////////////////////////////////////////////////////////////////////

void CodeGenerator::__translateCall(unordered_map<string, vector<string>> &codeMap) const
{
    auto funcJmpMap = __createFuncJmpMap(codeMap);

    // A virtual "IP"
    int IP = 0;

    // "main" function must be the first function
    for (auto &codeStr: codeMap.at("main"))
    {
        if (codeStr.substr(0, 4) == "CALL")
        {
            // Function start IP - IP => Jump distance
            codeStr = "JMP " + to_string(funcJmpMap.at(codeStr.substr(5)) - IP);
        }

        IP++;
    }

    // Other function
    for (auto &mapPair: codeMap)
    {
        if (mapPair.first != "main")
        {
            for (auto &codeStr: mapPair.second)
            {
                if (codeStr.substr(0, 4) == "CALL")
                {
                    codeStr = "JMP " + to_string(funcJmpMap.at(codeStr.substr(5)) - IP);
                }

                IP++;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Global Variable Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateGlobalVariableCode() const
{
    vector<string> codeList;

    for (auto &mapPair: __symbolTable.at("__GLOBAL__"))
    {
        // Array
        if (mapPair.second.second)
        {
            // Calc the array start address (Variable number + 1)
            codeList.push_back("LDC " + to_string(mapPair.second.first + 1));
        }

        // Push the array start address
        // (Or only a meaningless int for global scalar memeory)
        codeList.push_back("PUSH");

        // Push array content (By array size times)
        for (int _ = 0; _ < mapPair.second.second; _++)
        {
            codeList.push_back("PUSH");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Main Prepare Code
////////////////////////////////////////////////////////////////////////////////

vector<string> CodeGenerator::__generateMainPrepareCode() const
{
    /*
        "main" function is a special function, so the following code is similar
        with the function: __generateCallCode
    */

    vector<string> codeList;

    vector<pair<string, pair<int, int>>> pairList(
        __symbolTable.at("main").begin(),
        __symbolTable.at("main").end());

    sort(pairList.begin(), pairList.end(),
        [] (const pair<string, pair<int, int>> &lhs, const pair<string, pair<int, int>> &rhs)
        {
            return lhs.second.first > rhs.second.first;
        }
    );

    // "main" function has definitely no params
    for (auto &mapPair: pairList)
    {
        if (mapPair.second.second)
        {
            for (int _ = 0; _ < mapPair.second.second; _++)
            {
                codeList.push_back("PUSH");
            }

            codeList.push_back("PUSHSP");
            codeList.push_back("LDC " + to_string(mapPair.second.second - 1));
            codeList.push_back("SUB");
            codeList.push_back("POP");
            codeList.push_back("PUSH");
        }
        else
        {
            codeList.push_back("PUSH");
        }
    }

    codeList.push_back("PUSHBP");
    codeList.push_back("SAVSP");

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Output Code
////////////////////////////////////////////////////////////////////////////////

void CodeGenerator::__outputCode(
    const unordered_map<string, vector<string>> &codeMap,
    const vector<string> &globalVariableCodeList,
    const vector<string> &mainPrepareCodeList) const
{
    FILE *fo = fopen(__outputFilePath.c_str(), "w");

    for (auto &codeStr: globalVariableCodeList)
    {
        fprintf(fo, "%s\n", codeStr.c_str());
    }

    for (auto &codeStr: mainPrepareCodeList)
    {
        fprintf(fo, "%s\n", codeStr.c_str());
    }

    for (auto &codeStr: codeMap.at("main"))
    {
        fprintf(fo, "%s\n", codeStr.c_str());
    }

    for (auto &mapPair: codeMap)
    {
        if (mapPair.first != "main")
        {
            for (auto &codeStr: mapPair.second)
            {
                fprintf(fo, "%s\n", codeStr.c_str());
            }
        }
    }

    fclose(fo);
}


}  // End namespace CMM


#endif  // __CMM_CODE_GENERATOR_HPP
