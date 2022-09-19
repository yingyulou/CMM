/*
    CodeGenerator.hpp
    =================
        Class __CodeGenerator implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <stdexcept>
#include "CodeGenerator.h"
#include "AST.h"
#include "Instruction.hpp"
#include "TokenType.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::to_string;
using std::stoi;
using std::vector;
using std::unordered_map;
using std::pair;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__CodeGenerator::__CodeGenerator(__AST *root, const unordered_map<string, unordered_map<string, pair<int, int>>> &symbolTable):
    __root       (root),
    __symbolTable(symbolTable) {}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Number Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateNumberCode(__AST *root, const string &) const
{
    /*
        __TokenType::__Number
    */

    return {{__Instruction::__LDC, root->__tokenStr}};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate StmtList Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateStmtListCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__StmtList
        |
        |---- [__Stmt]
        .
        .
        .
    */

    vector<pair<__Instruction, string>> codeList;

    for (auto stmtPtr: root->__subList)
    {
        auto stmtCodeList = __generateStmtCode(stmtPtr, curFuncName);

        codeList.insert(codeList.end(), stmtCodeList.begin(), stmtCodeList.end());
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Stmt Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateStmtCode(__AST *root, const string &curFuncName) const
{
    /*
        __ExprStmt | __IfStmt | __WhileStmt | __ReturnStmt

        (__ExprStmt: [__Expr])
    */

    if (!root)
    {
        return {};
    }

    switch (root->__tokenType)
    {
        case __TokenType::__Expr:
            return __generateExprCode(root, curFuncName);

        case __TokenType::__IfStmt:
            return __generateIfStmtCode(root, curFuncName);

        case __TokenType::__WhileStmt:
            return __generateWhileStmtCode(root, curFuncName);

        case __TokenType::__ReturnStmt:
            return __generateReturnStmtCode(root, curFuncName);

        default:
            throw runtime_error("Invalid __TokenType value");
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate IfStmt Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateIfStmtCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__IfStmt
        |
        |---- __Expr
        |
        |---- __StmtList
        |
        |---- [__StmtList]
    */

    auto codeList   = __generateExprCode(root->__subList[0], curFuncName);
    auto ifCodeList = __generateStmtListCode(root->__subList[1], curFuncName);

    if (!root->__subList[2])
    {
        /*
            if ...

                JZ END

                ...

            END: ...
        */
        codeList.emplace_back(__Instruction::__JZ, to_string(ifCodeList.size() + 1));

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
        auto elseCodeList = __generateStmtListCode(root->__subList[2], curFuncName);

        ifCodeList.emplace_back(__Instruction::__JMP, to_string(elseCodeList.size() + 1));

        codeList.emplace_back(__Instruction::__JZ, to_string(ifCodeList.size() + 1));

        codeList.insert(codeList.end(), ifCodeList.begin(), ifCodeList.end());
        codeList.insert(codeList.end(), elseCodeList.begin(), elseCodeList.end());
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate WhileStmt Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateWhileStmtCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__WhileStmt
        |
        |---- __Expr
        |
        |---- __StmtList
    */

    auto codeList     = __generateExprCode(root->__subList[0], curFuncName);
    auto stmtCodeList = __generateStmtListCode(root->__subList[1], curFuncName);

    /*
        START: while ...

            JZ END

            ...

            JMP START

        END: ...
    */
    codeList.emplace_back(__Instruction::__JZ, to_string(stmtCodeList.size() + 2));

    codeList.insert(codeList.end(), stmtCodeList.begin(), stmtCodeList.end());

    codeList.emplace_back(__Instruction::__JMP, "-" + to_string(codeList.size()));

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate ReturnStmt Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateReturnStmtCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__ReturnStmt
        |
        |---- [__Expr]
    */

    if (root->__subList[0])
    {
        return __generateExprCode(root->__subList[0], curFuncName);
    }
    else
    {
        return {};
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Expr Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateExprCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__Expr
        |
        |---- __Var
        |
        |---- __Expr

        ----------------------

        __TokenType::__Expr
        |
        |---- __SimpleExpr
    */

    if (!root->__subList[1])
    {
        return __generateSimpleExprCode(root->__subList[0], curFuncName);
    }
    else
    {
        auto codeList       = __generateExprCode(root->__subList[1], curFuncName);
        auto assignCodeList = __generateAssignCode(root->__subList[0], curFuncName);

        codeList.insert(codeList.end(), assignCodeList.begin(), assignCodeList.end());

        return codeList;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Var Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateVarCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__Var
        |
        |---- __TokenType::__Id
        |
        |---- [__Expr]
    */

    vector<pair<__Instruction, string>> codeList;

    // Local variable
    if (__symbolTable.at(curFuncName).count(root->__subList[0]->__tokenStr))
    {
        codeList.emplace_back(__Instruction::__LDC, to_string(__symbolTable.at(curFuncName).at(root->__subList[0]->__tokenStr).first));
        codeList.emplace_back(__Instruction::__LD, "");
    }
    // Global variable
    else
    {
        codeList.emplace_back(__Instruction::__LDC, to_string(__symbolTable.at("__GLOBAL__").at(root->__subList[0]->__tokenStr).first));
        codeList.emplace_back(__Instruction::__ALD, "");
    }

    // Array
    if (root->__subList[1])
    {
        auto exprCodeList = __generateExprCode(root->__subList[1], curFuncName);

        codeList.emplace_back(__Instruction::__PUSH, "");

        codeList.insert(codeList.end(),exprCodeList.begin(), exprCodeList.end());

        codeList.emplace_back(__Instruction::__ADD, "");
        codeList.emplace_back(__Instruction::__POP, "");
        codeList.emplace_back(__Instruction::__ALD, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate SimpleExpr Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateSimpleExprCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__SimpleExpr
        |
        |---- __AddExpr
        |
        |---- [__RelOp]
        |
        |---- [__AddExpr]
    */

    if (!root->__subList[1] && !root->__subList[2])
    {
        return __generateAddExprCode(root->__subList[0], curFuncName);
    }
    else
    {
        auto codeList      = __generateAddExprCode(root->__subList[0], curFuncName);
        auto midCodeList   = __generateRelOpCode(root->__subList[1], curFuncName);
        auto rightCodeList = __generateAddExprCode(root->__subList[2], curFuncName);

        codeList.emplace_back(__Instruction::__PUSH, "");

        codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
        codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());

        codeList.emplace_back(__Instruction::__POP, "");

        return codeList;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate RelOp Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateRelOpCode(__AST *root, const string &) const
{
    /*
        __TokenType::__Less         |
        __TokenType::__LessEqual    |
        __TokenType::__Greater      |
        __TokenType::__GreaterEqual |
        __TokenType::__Equal        |
        __TokenType::__NotEqual
    */

    switch (root->__tokenType)
    {
        case __TokenType::__Less:
            return {{__Instruction::__LT, ""}};

        case __TokenType::__LessEqual:
            return {{__Instruction::__LE, ""}};

        case __TokenType::__Greater:
            return {{__Instruction::__GT, ""}};

        case __TokenType::__GreaterEqual:
            return {{__Instruction::__GE, ""}};

        case __TokenType::__Equal:
            return {{__Instruction::__EQ, ""}};

        case __TokenType::__NotEqual:
            return {{__Instruction::__NE, ""}};

        default:
            throw runtime_error("Invalid __TokenType value");
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate AddExpr Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateAddExprCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__AddExpr
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

    auto codeList = __generateTermCode(root->__subList[0], curFuncName);

    for (int idx = 1; idx < (int)root->__subList.size(); idx += 2)
    {
        auto midCodeList = __generateAddOpCode(root->__subList[idx], curFuncName);
        auto rightCodeList = __generateTermCode(root->__subList[idx + 1], curFuncName);

        codeList.emplace_back(__Instruction::__PUSH, "");

        codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
        codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());

        codeList.emplace_back(__Instruction::__POP, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate AddOp Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateAddOpCode(__AST *root, const string &) const
{
    /*
        __TokenType::__Plus | __TokenType::__Minus
    */

    if (root->__tokenType == __TokenType::__Plus)
    {
        return {{__Instruction::__ADD, ""}};
    }
    else
    {
        return {{__Instruction::__SUB, ""}};
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Term Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateTermCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__Term
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

    auto codeList = __generateFactorCode(root->__subList[0], curFuncName);

    for (int idx = 1; idx < (int)root->__subList.size(); idx += 2)
    {
        auto midCodeList   = __generateMulOpCode(root->__subList[idx], curFuncName);
        auto rightCodeList = __generateFactorCode(root->__subList[idx + 1], curFuncName);

        codeList.emplace_back(__Instruction::__PUSH, "");

        codeList.insert(codeList.end(), rightCodeList.begin(), rightCodeList.end());
        codeList.insert(codeList.end(), midCodeList.begin(), midCodeList.end());

        codeList.emplace_back(__Instruction::__POP, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate MulOp Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateMulOpCode(__AST *root, const string &) const
{
    /*
        __TokenType::__Multiply | __TokenType::__Divide
    */

    if (root->__tokenType == __TokenType::__Multiply)
    {
        return {{__Instruction::__MUL, ""}};
    }
    else
    {
        return {{__Instruction::__DIV, ""}};
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Factor Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateFactorCode(__AST *root, const string &curFuncName) const
{
    /*
        __Expr | __TokenType::__Number | __Call | __Var
    */

    switch (root->__tokenType)
    {
        case __TokenType::__Expr:
            return __generateExprCode(root, curFuncName);

        case __TokenType::__Number:
            return __generateNumberCode(root, curFuncName);

        case __TokenType::__Call:
            return __generateCallCode(root, curFuncName);

        case __TokenType::__Var:
            return __generateVarCode(root, curFuncName);

        default:
            throw runtime_error("Invalid __TokenType value");
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Call Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateCallCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__Call
        |
        |---- __TokenType::__Id
        |
        |---- [__ArgList]
    */

    // xxx = input();
    if (root->__subList[0]->__tokenStr == "input")
    {
        return {{__Instruction::__IN, ""}};
    }
    // output(xxx);
    else if (root->__subList[0]->__tokenStr == "output")
    {
        /*
            __TokenType::__ArgList
            |
            |---- __Expr
        */
        auto codeList = __generateExprCode(root->__subList[1]->__subList[0], curFuncName);

        codeList.emplace_back(__Instruction::__OUT, "");

        return codeList;
    }

    vector<pair<__Instruction, string>> codeList;
    vector<pair<string, pair<int, int>>> pairList(__symbolTable.at(root->__subList[0]->__tokenStr).size());

    // ..., Local5, Local4, Local3, Param2, Param1, Param0
    for (auto &mapPair: __symbolTable.at(root->__subList[0]->__tokenStr))
    {
        pairList[pairList.size() - mapPair.second.first - 1] = mapPair;
    }

    // We only need local variable here
    int topIdx = pairList.size() - (root->__subList[1] ?

        // Call function by at least one parameter
        root->__subList[1]->__subList.size() :

        // Call function without any parameter
        0);

    // Push local variable
    for (int idx = 0; idx < topIdx; idx++)
    {
        // Array
        if (pairList[idx].second.second)
        {
            // Push array content (by array size times)
            for (int _ = 0; _ < pairList[idx].second.second; _++)
            {
                codeList.emplace_back(__Instruction::__PUSH, "");
            }

            /*
                The instruction "ADDR N" calculate the array start pointer (absolute index in SS).

                SS:

                    ... X X X X X X X X X $
                        ^     Size = N    ^
                        |                 |
                     SP - N               SP

            */
            codeList.emplace_back(__Instruction::__ADDR, to_string(pairList[idx].second.second));
            codeList.emplace_back(__Instruction::__PUSH, "");
        }
        // Scalar
        else
        {
            codeList.emplace_back(__Instruction::__PUSH, "");
        }
    }

    // Push parameter
    if (root->__subList[1])
    {
        auto argListCodeList = __generateArgListCode(root->__subList[1], curFuncName);

        codeList.insert(codeList.end(), argListCodeList.begin(), argListCodeList.end());
    }

    /*
        The instruction "CALL N" perform multiple actions:

        1. SS.PUSH(BP)

            Now the SS is like:

            ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP

        2. BP = SS.SIZE() - 2

            Now the SS is like:

            ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP
                                                     ^
                                                     |
                                                     BP

        3. SS.PUSH(IP)

            Now the SS is like:

            ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP OldIP
                                                     ^
                                                     |
                                                     BP

        4. IP += N

        In addition, the "N" of the "CALL N" is only a function name right now,
        it will be translated to a number later. (See the function: __translateCall)
    */
    codeList.emplace_back(__Instruction::__CALL, root->__subList[0]->__tokenStr);

    // After call, we need several "POP" to pop all variables.
    for (auto &[_, mapVal]: __symbolTable.at(root->__subList[0]->__tokenStr))
    {
        // Any variable needs a "POP"
        codeList.emplace_back(__Instruction::__POP, "");

        // Pop array content (by array size times)
        for (int _ = 0; _ < mapVal.second; _++)
        {
            codeList.emplace_back(__Instruction::__POP, "");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate ArgList Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateArgListCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__ArgList
        |
        |---- __Expr
        |
        |---- [__Expr]
        .
        .
        .
    */

    vector<pair<__Instruction, string>> codeList;

    for (int idx = (int)root->__subList.size() - 1; idx >= 0; idx--)
    {
        auto exprCodeList = __generateExprCode(root->__subList[idx], curFuncName);

        codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

        codeList.emplace_back(__Instruction::__PUSH, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Assign Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateAssignCode(__AST *root, const string &curFuncName) const
{
    /*
        __TokenType::__Var
        |
        |---- __TokenType::__Id
        |
        |---- [__Expr]
    */

    vector<pair<__Instruction, string>> codeList {{__Instruction::__PUSH, ""}};

    // Local variable
    if (__symbolTable.at(curFuncName).count(root->__subList[0]->__tokenStr))
    {
        codeList.emplace_back(__Instruction::__LDC, to_string(__symbolTable.at(curFuncName).at(root->__subList[0]->__tokenStr).first));

        // Scalar
        if (!root->__subList[1])
        {
            codeList.emplace_back(__Instruction::__ST, "");
        }
        // Array
        else
        {
            auto exprCodeList = __generateExprCode(root->__subList[1], curFuncName);

            // Get the (start) pointer (is already an absolute address)
            codeList.emplace_back(__Instruction::__LD, "");
            codeList.emplace_back(__Instruction::__PUSH, "");

            codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

            // Pointer[Index] (Pointer + Index)
            codeList.emplace_back(__Instruction::__ADD, "");
            codeList.emplace_back(__Instruction::__POP, "");

            // Save by absolute address
            codeList.emplace_back(__Instruction::__AST, "");
        }
    }
    // Global variable
    else
    {
        codeList.emplace_back(__Instruction::__LDC, to_string(__symbolTable.at("__GLOBAL__").at(root->__subList[0]->__tokenStr).first));

        // Scalar
        if (!root->__subList[1])
        {
            codeList.emplace_back(__Instruction::__AST, "");
        }
        // Array
        else
        {
            auto exprCodeList = __generateExprCode(root->__subList[1], curFuncName);

            // Absolute get the (start) pointer (is already an absolute address)
            codeList.emplace_back(__Instruction::__ALD, "");
            codeList.emplace_back(__Instruction::__PUSH, "");

            codeList.insert(codeList.end(), exprCodeList.begin(), exprCodeList.end());

            // Pointer[Index] (Pointer + Index)
            codeList.emplace_back(__Instruction::__ADD, "");
            codeList.emplace_back(__Instruction::__POP, "");

            // Save by absolute address
            codeList.emplace_back(__Instruction::__AST, "");
        }
    }

    codeList.emplace_back(__Instruction::__POP, "");

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Global Variable Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateGlobalVariableCode() const
{
    vector<pair<__Instruction, string>> codeList;

    for (auto &[_, infoPair]: __symbolTable.at("__GLOBAL__"))
    {
        // Array
        if (infoPair.second)
        {
            // Calc the array start address (variable number + 1)
            codeList.emplace_back(__Instruction::__LDC, to_string(infoPair.first + 1));
        }

        // Push the array start address
        // (Or only a meaningless int for global scalar memeory)
        codeList.emplace_back(__Instruction::__PUSH, "");

        // Push array content (by array size times)
        for (int _ = 0; _ < infoPair.second; _++)
        {
            codeList.emplace_back(__Instruction::__PUSH, "");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Main Prepare Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateMainPrepareCode() const
{
    /*
        The "main" function is a special function, so the following code is
        similar with the function: __generateCallCode
    */

    vector<pair<__Instruction, string>> codeList;

    vector<pair<string, pair<int, int>>> pairList(__symbolTable.at("main").size());

    // ..., Local2, Local1, Local0
    for (auto &mapPair: __symbolTable.at("main"))
    {
        pairList[pairList.size() - mapPair.second.first - 1] = mapPair;
    }

    // The "main" function has definitely no params
    for (auto &[_, infoPair]: pairList)
    {
        if (infoPair.second)
        {
            for (int _ = 0; _ < infoPair.second; _++)
            {
                codeList.emplace_back(__Instruction::__PUSH, "");
            }

            codeList.emplace_back(__Instruction::__ADDR, to_string(infoPair.second));
            codeList.emplace_back(__Instruction::__PUSH, "");
        }
        else
        {
            codeList.emplace_back(__Instruction::__PUSH, "");
        }
    }

    // Call the "main" function automatically
    codeList.emplace_back(__Instruction::__CALL, "main");

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Global Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateGlobalCode() const
{
    auto codeList = __generateGlobalVariableCode();

    auto mainPrepareCodeList = __generateMainPrepareCode();

    codeList.insert(codeList.end(), mainPrepareCodeList.begin(), mainPrepareCodeList.end());

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create CodeMap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unordered_map<string, vector<pair<__Instruction, string>>> __CodeGenerator::__createCodeMap() const
{
    unordered_map<string, vector<pair<__Instruction, string>>> codeMap {{"__GLOBAL__", __generateGlobalCode()}};

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
    for (auto declPtr: __root->__subList)
    {
        /*
            __VarDecl | __FuncDecl
        */
        if (declPtr->__tokenType == __TokenType::__FuncDecl)
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
            auto curFuncName = declPtr->__subList[1]->__tokenStr;

            auto codeList = __generateStmtListCode(declPtr->__subList[4], curFuncName);

            if (curFuncName != "main")
            {
                /*
                    The instruction "RET" perform multiple actions:

                    1. IP = SS.POP()

                        Now the SS is like:

                        ... Local5 Local4 Local3 Param2 Param1 Param0 OldBP

                    2. BP = SS.POP()

                        Now the SS is like:

                        ... Local5 Local4 Local3 Param2 Param1 Param0

                    So we still need several "POP" to pop all variables.
                    (See the function: __generateCallCode)
                */
                codeList.emplace_back(__Instruction::__RET, "");
            }

            codeMap[curFuncName] = codeList;
        }
    }

    return codeMap;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Merge CodeMap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pair<vector<pair<__Instruction, string>>, unordered_map<string, int>> __CodeGenerator::__mergeCodeMap(
    const unordered_map<string, vector<pair<__Instruction, string>>> &codeMap)
{
    vector<pair<__Instruction, string>> codeList;

    // funcJmpMap: Function name => Function start IP
    unordered_map<string, int> funcJmpMap;

    // Global code must be the first part
    int jmpNum = (int)codeMap.at("__GLOBAL__").size();

    codeList.insert(codeList.end(), codeMap.at("__GLOBAL__").begin(), codeMap.at("__GLOBAL__").end());

    // Other functions
    for (auto &[funcName, subCodeList]: codeMap)
    {
        if (funcName != "__GLOBAL__" && funcName != "main")
        {
            codeList.insert(codeList.end(), subCodeList.begin(), subCodeList.end());

            funcJmpMap[funcName] = jmpNum;

            jmpNum += (int)subCodeList.size();
        }
    }

    // The "main" function must be the last function
    codeList.insert(codeList.end(), codeMap.at("main").begin(), codeMap.at("main").end());

    funcJmpMap["main"] = jmpNum;

    return {codeList, funcJmpMap};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Translate Call
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __CodeGenerator::__translateCall(vector<pair<__Instruction, string>> &codeList, const unordered_map<string, int> &funcJmpMap)
{
    // A virtual "IP"
    for (int IP = 0; IP < (int)codeList.size(); IP++)
    {
        if (codeList[IP].first == __Instruction::__CALL)
        {
            codeList[IP].second = to_string(funcJmpMap.at(codeList[IP].second) - IP);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, string>> __CodeGenerator::__generateCode() const
{
    auto codeMap = __createCodeMap();

    auto [codeList, funcJmpMap] = __mergeCodeMap(codeMap);

    __translateCall(codeList, funcJmpMap);

    return codeList;
}


}  // End namespace CMM
