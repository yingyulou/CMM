/*
    CodeGenerator.hpp
    =================
        Class CodeGenerator implementation.
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

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::to_string;
using std::stoi;
using std::vector;
using std::unordered_map;
using std::pair;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

CodeGenerator::CodeGenerator(AST *root,
    const unordered_map<string, unordered_map<string, pair<int, int>>> &symbolTable):
    __root       (root),
    __symbolTable(symbolTable) {}


////////////////////////////////////////////////////////////////////////////////
// Generate Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::generateCode() const
{
    return __generateCode();
}


////////////////////////////////////////////////////////////////////////////////
// Generate Number Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateNumberCode(
    AST *root, const string &) const
{
    /*
        TokenType::Number
    */

    return {{Instruction::LDC, root->tokenStr()}};
}


////////////////////////////////////////////////////////////////////////////////
// Generate CompoundStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateCompoundStmtCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::CompoundStmt
            |
            |---- __LocalDecl
            |
            |---- __StmtList
    */

    return __generateStmtListCode(root->subList()[1], curFuncName);
}


////////////////////////////////////////////////////////////////////////////////
// Generate StmtList Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateStmtListCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::StmtList
            |
            |---- [__Stmt]
            .
            .
            .
    */

    vector<pair<Instruction, string>> codeList;

    for (auto stmtPtr: root->subList())
    {
        auto stmtCodeList = __generateStmtCode(stmtPtr, curFuncName);

        codeList.insert(codeList.end(),
            stmtCodeList.begin(), stmtCodeList.end());
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Stmt Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateStmtCode(
    AST *root, const string &curFuncName) const
{
    /*
        __ExprStmt | __CompoundStmt | __IfStmt | __WhileStmt | __ReturnStmt

        (__ExprStmt AST: __Expr | nullptr)
    */

    if (!root)
    {
        return {};
    }

    switch (root->tokenType())
    {
        case TokenType::Expr:
            return __generateExprCode(root, curFuncName);

        case TokenType::CompoundStmt:
            return __generateCompoundStmtCode(root, curFuncName);

        case TokenType::IfStmt:
            return __generateIfStmtCode(root, curFuncName);

        case TokenType::WhileStmt:
            return __generateWhileStmtCode(root, curFuncName);

        case TokenType::ReturnStmt:
            return __generateReturnStmtCode(root, curFuncName);

        default:
            throw runtime_error("Invalid TokenType value");
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate IfStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateIfStmtCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::IfStmt
            |
            |---- __Expr
            |
            |---- __Stmt
            |
            |---- [__Stmt]
    */

    auto codeList   = __generateExprCode(root->subList()[0], curFuncName);
    auto ifCodeList = __generateStmtCode(root->subList()[1], curFuncName);

    if (root->subList().size() == 2)
    {
        /*
            if ...

                JZ END

                ...

            END: ...
        */
        codeList.emplace_back(Instruction::JZ,
            to_string(ifCodeList.size() + 1));

        codeList.insert(codeList.end(),
            ifCodeList.begin(), ifCodeList.end());
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
        auto elseCodeList = __generateStmtCode(
            root->subList()[2], curFuncName);

        ifCodeList.emplace_back(Instruction::JMP,
            to_string(elseCodeList.size() + 1));

        codeList.emplace_back(Instruction::JZ,
            to_string(ifCodeList.size() + 1));

        codeList.insert(codeList.end(),
            ifCodeList.begin(), ifCodeList.end());

        codeList.insert(codeList.end(),
            elseCodeList.begin(), elseCodeList.end());
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate WhileStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateWhileStmtCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::WhileStmt
            |
            |---- __Expr
            |
            |---- __Stmt
    */

    auto codeList     = __generateExprCode(root->subList()[0], curFuncName);
    auto stmtCodeList = __generateStmtCode(root->subList()[1], curFuncName);

    /*
        START: while ...

            JZ END

            ...

            JMP START

        END: ...
    */
    codeList.emplace_back(Instruction::JZ,
        to_string(stmtCodeList.size() + 2));

    codeList.insert(codeList.end(),
        stmtCodeList.begin(), stmtCodeList.end());

    codeList.emplace_back(Instruction::JMP,
        "-" + to_string(codeList.size()));

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate ReturnStmt Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateReturnStmtCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::ReturnStmt
            |
            |---- [__Expr]
    */

    if (root->subList().empty())
    {
        return {};
    }
    else
    {
        return __generateExprCode(root->subList()[0], curFuncName);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Expr Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateExprCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::Expr
            |
            |---- __Var
            |
            |---- __Expr

        ----------------------

        TokenType::Expr
            |
            |---- __SimpleExpr
    */

    if (root->subList().size() == 1)
    {
        return __generateSimpleExprCode(root->subList()[0], curFuncName);
    }
    else
    {
        auto codeList = __generateExprCode(
            root->subList()[1], curFuncName);

        auto assignCodeList = __generateAssignCode(
            root->subList()[0], curFuncName);

        codeList.insert(codeList.end(),
            assignCodeList.begin(), assignCodeList.end());

        return codeList;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Var Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateVarCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::Var
            |
            |---- TokenType::Id
            |
            |---- [__Expr]
    */

    vector<pair<Instruction, string>> codeList;

    // Local variable
    if (__symbolTable.at(curFuncName).count(
        root->subList()[0]->tokenStr()))
    {
        codeList.emplace_back(Instruction::LDC,
            to_string(__symbolTable.at(curFuncName).at(
            root->subList()[0]->tokenStr()).first));

        codeList.emplace_back(Instruction::LD, "");
    }
    // Global variable
    else
    {
        codeList.emplace_back(Instruction::LDC,
            to_string(__symbolTable.at("__GLOBAL__").at(
            root->subList()[0]->tokenStr()).first));

        codeList.emplace_back(Instruction::ALD, "");
    }

    // Array
    if (root->subList().size() == 2)
    {
        auto exprCodeList = __generateExprCode(
            root->subList()[1], curFuncName);

        codeList.emplace_back(Instruction::PUSH, "");

        codeList.insert(codeList.end(),
            exprCodeList.begin(), exprCodeList.end());

        codeList.emplace_back(Instruction::ADD, "");
        codeList.emplace_back(Instruction::POP, "");
        codeList.emplace_back(Instruction::ALD, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate SimpleExpr Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateSimpleExprCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::SimpleExpr
            |
            |---- __AddExpr
            |
            |---- [__RelOp]
            |
            |---- [__AddExpr]
    */

    if (root->subList().size() == 1)
    {
        return __generateAddExprCode(root->subList()[0], curFuncName);
    }
    else
    {
        auto codeList = __generateAddExprCode(
            root->subList()[0], curFuncName);

        auto midCodeList = __generateRelOpCode(
            root->subList()[1], curFuncName);

        auto rightCodeList = __generateAddExprCode(
            root->subList()[2], curFuncName);

        codeList.emplace_back(Instruction::PUSH, "");

        codeList.insert(codeList.end(),
            rightCodeList.begin(), rightCodeList.end());

        codeList.insert(codeList.end(),
            midCodeList.begin(), midCodeList.end());

        codeList.emplace_back(Instruction::POP, "");

        return codeList;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate RelOp Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateRelOpCode(
    AST *root, const string &) const
{
    /*
        TokenType::Less         |
        TokenType::LessEqual    |
        TokenType::Greater      |
        TokenType::GreaterEqual |
        TokenType::Equal        |
        TokenType::NotEqual
    */

    switch (root->tokenType())
    {
        case TokenType::Less:
            return {{Instruction::LT, ""}};

        case TokenType::LessEqual:
            return {{Instruction::LE, ""}};

        case TokenType::Greater:
            return {{Instruction::GT, ""}};

        case TokenType::GreaterEqual:
            return {{Instruction::GE, ""}};

        case TokenType::Equal:
            return {{Instruction::EQ, ""}};

        case TokenType::NotEqual:
            return {{Instruction::NE, ""}};

        default:
            throw runtime_error("Invalid TokenType value");
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate AddExpr Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateAddExprCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::AddExpr
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

    auto codeList = __generateTermCode(root->subList()[0], curFuncName);

    for (int idx = 1; idx < root->subList().size(); idx += 2)
    {
        auto midCodeList = __generateAddOpCode(
            root->subList()[idx], curFuncName);

        auto rightCodeList = __generateTermCode(
            root->subList()[idx + 1], curFuncName);

        codeList.emplace_back(Instruction::PUSH, "");

        codeList.insert(codeList.end(),
            rightCodeList.begin(), rightCodeList.end());

        codeList.insert(codeList.end(),
            midCodeList.begin(), midCodeList.end());

        codeList.emplace_back(Instruction::POP, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate AddOp Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateAddOpCode(
    AST *root, const string &) const
{
    /*
        TokenType::Plus | TokenType::Minus
    */

    if (root->tokenType() == TokenType::Plus)
    {
        return {{Instruction::ADD, ""}};
    }
    else
    {
        return {{Instruction::SUB, ""}};
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Term Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateTermCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::Term
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

    auto codeList = __generateFactorCode(root->subList()[0], curFuncName);

    for (int idx = 1; idx < root->subList().size(); idx += 2)
    {
        auto midCodeList = __generateMulOpCode(
            root->subList()[idx], curFuncName);

        auto rightCodeList = __generateFactorCode(
            root->subList()[idx + 1], curFuncName);

        codeList.emplace_back(Instruction::PUSH, "");

        codeList.insert(codeList.end(),
            rightCodeList.begin(), rightCodeList.end());

        codeList.insert(codeList.end(),
            midCodeList.begin(), midCodeList.end());

        codeList.emplace_back(Instruction::POP, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate MulOp Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateMulOpCode(
    AST *root, const string &) const
{
    /*
        TokenType::Multiply | TokenType::Divide
    */

    if (root->tokenType() == TokenType::Multiply)
    {
        return {{Instruction::MUL, ""}};
    }
    else
    {
        return {{Instruction::DIV, ""}};
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Factor Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateFactorCode(
    AST *root, const string &curFuncName) const
{
    /*
        __Expr | TokenType::Number | __Call | __Var
    */

    switch (root->tokenType())
    {
        case TokenType::Expr:
            return __generateExprCode(root, curFuncName);

        case TokenType::Number:
            return __generateNumberCode(root, curFuncName);

        case TokenType::Call:
            return __generateCallCode(root, curFuncName);

        case TokenType::Var:
            return __generateVarCode(root, curFuncName);

        default:
            throw runtime_error("Invalid TokenType value");
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Call Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateCallCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::Call
            |
            |---- TokenType::Id
            |
            |---- [__ArgList]
    */

    // xxx = input();
    if (root->subList()[0]->tokenStr() == "input")
    {
        return {{Instruction::IN, ""}};
    }
    // output(xxx);
    else if (root->subList()[0]->tokenStr() == "output")
    {
        /*
            TokenType::ArgList
                |
                |---- __Expr
        */
        auto codeList = __generateExprCode(
            root->subList()[1]->subList()[0], curFuncName);

        codeList.emplace_back(Instruction::OUT, "");

        return codeList;
    }

    vector<pair<Instruction, string>> codeList;

    vector<pair<string, pair<int, int>>> pairList(
        __symbolTable.at(root->subList()[0]->tokenStr()).size());

    // ..., Local5, Local4, Local3, Param2, Param1, Param0
    for (auto &mapPair: __symbolTable.at(root->subList()[0]->tokenStr()))
    {
        pairList[pairList.size() - mapPair.second.first - 1] = mapPair;
    }

    // We only need local variable here
    int topIdx = pairList.size() - (root->subList().size() == 2 ?

        // Call function by at least one parameter
        root->subList()[1]->subList().size() :

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
                codeList.emplace_back(Instruction::PUSH, "");
            }

            /*
                The instruction "ADDR N" calculate the array start pointer (absolute index in SS).

                SS:

                    ... X X X X X X X X X $
                        ^     Size = N    ^
                        |                 |
                     SP - N               SP

            */
            codeList.emplace_back(Instruction::ADDR,
                to_string(pairList[idx].second.second));

            codeList.emplace_back(Instruction::PUSH, "");
        }
        // Scalar
        else
        {
            codeList.emplace_back(Instruction::PUSH, "");
        }
    }

    // Push parameter
    if (root->subList().size() == 2)
    {
        auto argListCodeList = __generateArgListCode(
            root->subList()[1], curFuncName);

        codeList.insert(codeList.end(),
            argListCodeList.begin(), argListCodeList.end());
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
    codeList.emplace_back(Instruction::CALL, root->subList()[0]->tokenStr());

    // After call, we need several "POP" to pop all variables.
    for (auto &[_, mapVal]: __symbolTable.at(root->subList()[0]->tokenStr()))
    {
        // Any variable needs a "POP"
        codeList.emplace_back(Instruction::POP, "");

        // Pop array content (by array size times)
        for (int _ = 0; _ < mapVal.second; _++)
        {
            codeList.emplace_back(Instruction::POP, "");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate ArgList Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateArgListCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::ArgList
            |
            |---- __Expr
            |
            |---- [__Expr]
            .
            .
            .
    */

    vector<pair<Instruction, string>> codeList;

    for (int idx = root->subList().size() - 1; idx >= 0; idx--)
    {
        auto exprCodeList = __generateExprCode(
            root->subList()[idx], curFuncName);

        codeList.insert(codeList.end(),
            exprCodeList.begin(), exprCodeList.end());

        codeList.emplace_back(Instruction::PUSH, "");
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Assign Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateAssignCode(
    AST *root, const string &curFuncName) const
{
    /*
        TokenType::Expr
            |
            |---- __Var  -> Root
            |
            |---- __Expr -> AX
    */

    vector<pair<Instruction, string>> codeList {{Instruction::PUSH, ""}};

    // Local variable
    if (__symbolTable.at(curFuncName).count(
        root->subList()[0]->tokenStr()))
    {
        codeList.emplace_back(Instruction::LDC,
            to_string(__symbolTable.at(curFuncName).at(
            root->subList()[0]->tokenStr()).first));

        // Scalar
        if (root->subList().size() == 1)
        {
            codeList.emplace_back(Instruction::ST, "");
        }
        // Array
        else
        {
            auto exprCodeList = __generateExprCode(
                root->subList()[1], curFuncName);

            // Get the (start) pointer (is already an absolute address)
            codeList.emplace_back(Instruction::LD, "");
            codeList.emplace_back(Instruction::PUSH, "");

            codeList.insert(codeList.end(),
                exprCodeList.begin(), exprCodeList.end());

            // Pointer[Index] (Pointer + Index)
            codeList.emplace_back(Instruction::ADD, "");
            codeList.emplace_back(Instruction::POP, "");

            // Save by absolute address
            codeList.emplace_back(Instruction::AST, "");
        }
    }
    // Global variable
    else
    {
        codeList.emplace_back(Instruction::LDC,
            to_string(__symbolTable.at("__GLOBAL__").at(
            root->subList()[0]->tokenStr()).first));

        // Scalar
        if (root->subList().size() == 1)
        {
            codeList.emplace_back(Instruction::AST, "");
        }
        // Array
        else
        {
            auto exprCodeList = __generateExprCode(
                root->subList()[1], curFuncName);

            // Absolute get the (start) pointer (is already an absolute address)
            codeList.emplace_back(Instruction::ALD, "");
            codeList.emplace_back(Instruction::PUSH, "");

            codeList.insert(codeList.end(),
                exprCodeList.begin(), exprCodeList.end());

            // Pointer[Index] (Pointer + Index)
            codeList.emplace_back(Instruction::ADD, "");
            codeList.emplace_back(Instruction::POP, "");

            // Save by absolute address
            codeList.emplace_back(Instruction::AST, "");
        }
    }

    codeList.emplace_back(Instruction::POP, "");

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Global Variable Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>>
CodeGenerator::__generateGlobalVariableCode() const
{
    vector<pair<Instruction, string>> codeList;

    for (auto &[_, infoPair]: __symbolTable.at("__GLOBAL__"))
    {
        // Array
        if (infoPair.second)
        {
            // Calc the array start address (variable number + 1)
            codeList.emplace_back(Instruction::LDC,
                to_string(infoPair.first + 1));
        }

        // Push the array start address
        // (Or only a meaningless int for global scalar memeory)
        codeList.emplace_back(Instruction::PUSH, "");

        // Push array content (by array size times)
        for (int _ = 0; _ < infoPair.second; _++)
        {
            codeList.emplace_back(Instruction::PUSH, "");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Main Prepare Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>>
CodeGenerator::__generateMainPrepareCode() const
{
    /*
        The "main" function is a special function, so the following code is
        similar with the function: __generateCallCode
    */

    vector<pair<Instruction, string>> codeList;

    vector<pair<string, pair<int, int>>> pairList(
        __symbolTable.at("main").size());

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
                codeList.emplace_back(Instruction::PUSH, "");
            }

            codeList.emplace_back(Instruction::ADDR,
                to_string(infoPair.second));

            codeList.emplace_back(Instruction::PUSH, "");
        }
        else
        {
            codeList.emplace_back(Instruction::PUSH, "");
        }
    }

    // Call the "main" function automatically
    codeList.emplace_back(Instruction::CALL, "main");

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Generate Global Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateGlobalCode() const
{
    auto codeList = __generateGlobalVariableCode();

    auto mainPrepareCodeList = __generateMainPrepareCode();

    codeList.insert(codeList.end(),
        mainPrepareCodeList.begin(), mainPrepareCodeList.end());

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Create CodeMap
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, vector<pair<Instruction, string>>>
CodeGenerator::__createCodeMap() const
{
    unordered_map<string, vector<pair<Instruction, string>>> codeMap
    {
        {"__GLOBAL__", __generateGlobalCode()},
    };

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
    for (auto declPtr: __root->subList())
    {
        /*
            __VarDecl | __FuncDecl
        */
        if (declPtr->tokenType() == TokenType::FuncDecl)
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
            auto curFuncName = declPtr->subList()[1]->tokenStr();

            /*
                TokenType::CompoundStmt
                    |
                    |---- __LocalDecl
                    |
                    |---- __StmtList
            */
            auto codeList = __generateStmtListCode(
                declPtr->subList()[3]->subList()[1], curFuncName);

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
                codeList.emplace_back(Instruction::RET, "");
            }

            codeMap[curFuncName] = codeList;
        }
    }

    return codeMap;
}


////////////////////////////////////////////////////////////////////////////////
// Merge CodeMap
////////////////////////////////////////////////////////////////////////////////

pair<vector<pair<Instruction, string>>, unordered_map<string, int>>
CodeGenerator::__mergeCodeMap(
    const unordered_map<string, vector<pair<Instruction, string>>> &codeMap)
{
    vector<pair<Instruction, string>> codeList;

    // funcJmpMap: Function name => Function start IP
    unordered_map<string, int> funcJmpMap;

    // Global code must be the first part
    int jmpNum = codeMap.at("__GLOBAL__").size();

    codeList.insert(codeList.end(),
        codeMap.at("__GLOBAL__").begin(), codeMap.at("__GLOBAL__").end());

    // Other functions
    for (auto &[funcName, subCodeList]: codeMap)
    {
        if (funcName != "__GLOBAL__" && funcName != "main")
        {
            codeList.insert(codeList.end(),
                subCodeList.begin(), subCodeList.end());

            funcJmpMap[funcName] = jmpNum;

            jmpNum += subCodeList.size();
        }
    }

    // The "main" function must be the last function
    codeList.insert(codeList.end(),
        codeMap.at("main").begin(), codeMap.at("main").end());

    funcJmpMap["main"] = jmpNum;

    return {codeList, funcJmpMap};
}


////////////////////////////////////////////////////////////////////////////////
// Translate Call
////////////////////////////////////////////////////////////////////////////////

void CodeGenerator::__translateCall(
    vector<pair<Instruction, string>> &codeList,
    const unordered_map<string, int> &funcJmpMap)
{
    // A virtual "IP"
    for (int IP = 0; IP < codeList.size(); IP++)
    {
        if (codeList[IP].first == Instruction::CALL)
        {
            codeList[IP].second = to_string(
                funcJmpMap.at(codeList[IP].second) - IP);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Generate Code
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, string>> CodeGenerator::__generateCode() const
{
    auto codeMap = __createCodeMap();

    auto [codeList, funcJmpMap] = __mergeCodeMap(codeMap);

    __translateCall(codeList, funcJmpMap);

    return codeList;
}


}  // End namespace CMM
