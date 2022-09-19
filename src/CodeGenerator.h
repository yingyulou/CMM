/*
    CodeGenerator.h
    ===============
        Class __CodeGenerator header.
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "AST.h"
#include "Instruction.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::unordered_map;
using std::pair;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __CodeGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __CodeGenerator
{
    // Friend
    friend class Core;


public:

    // Constructor
    __CodeGenerator(__AST *root = nullptr, const unordered_map<string, unordered_map<string, pair<int, int>>> &symbolTable = {});


private:

    // Attribute
    __AST *__root;
    unordered_map<string, unordered_map<string, pair<int, int>>> __symbolTable;


    // Generate Number Code
    vector<pair<__Instruction, string>> __generateNumberCode(__AST *root, const string &) const;


    // Generate CompoundStmt Code
    vector<pair<__Instruction, string>> __generateCompoundStmtCode(__AST *root, const string &curFuncName) const;


    // Generate StmtList Code
    vector<pair<__Instruction, string>> __generateStmtListCode(__AST *root, const string &curFuncName) const;


    // Generate Stmt Code
    vector<pair<__Instruction, string>> __generateStmtCode(__AST *root, const string &curFuncName) const;


    // Generate IfStmt Code
    vector<pair<__Instruction, string>> __generateIfStmtCode(__AST *root, const string &curFuncName) const;


    // Generate WhileStmt Code
    vector<pair<__Instruction, string>> __generateWhileStmtCode(__AST *root, const string &curFuncName) const;


    // Generate ReturnStmt Code
    vector<pair<__Instruction, string>> __generateReturnStmtCode(__AST *root, const string &curFuncName) const;


    // Generate Expr Code
    vector<pair<__Instruction, string>> __generateExprCode(__AST *root, const string &curFuncName) const;


    // Generate Var Code
    vector<pair<__Instruction, string>> __generateVarCode(__AST *root, const string &curFuncName) const;


    // Generate SimpleExpr Code
    vector<pair<__Instruction, string>> __generateSimpleExprCode(__AST *root, const string &curFuncName) const;


    // Generate RelOp Code
    vector<pair<__Instruction, string>> __generateRelOpCode(__AST *root, const string &) const;


    // Generate AddExpr Code
    vector<pair<__Instruction, string>> __generateAddExprCode(__AST *root, const string &curFuncName) const;


    // Generate AddOp Code
    vector<pair<__Instruction, string>> __generateAddOpCode(__AST *root, const string &) const;


    // Generate Term Code
    vector<pair<__Instruction, string>> __generateTermCode(__AST *root, const string &curFuncName) const;


    // Generate MulOp Code
    vector<pair<__Instruction, string>> __generateMulOpCode(__AST *root, const string &) const;


    // Generate Factor Code
    vector<pair<__Instruction, string>> __generateFactorCode(__AST *root, const string &curFuncName) const;


    // Generate Call Code
    vector<pair<__Instruction, string>> __generateCallCode(__AST *root, const string &curFuncName) const;


    // Generate ArgList Code
    vector<pair<__Instruction, string>> __generateArgListCode(__AST *root, const string &curFuncName) const;


    // Generate Assign Code
    vector<pair<__Instruction, string>> __generateAssignCode(__AST *root, const string &curFuncName) const;


    // Generate Global Variable Code
    vector<pair<__Instruction, string>> __generateGlobalVariableCode() const;


    // Generate Main Prepare Code
    vector<pair<__Instruction, string>> __generateMainPrepareCode() const;


    // Generate Global Code
    vector<pair<__Instruction, string>> __generateGlobalCode() const;


    // Create CodeMap
    unordered_map<string, vector<pair<__Instruction, string>>> __createCodeMap() const;


    // Merge CodeMap
    static pair<vector<pair<__Instruction, string>>, unordered_map<string, int>> __mergeCodeMap(
        const unordered_map<string, vector<pair<__Instruction, string>>> &codeMap);


    // Translate Call
    static void __translateCall(vector<pair<__Instruction, string>> &codeList, const unordered_map<string, int> &funcJmpMap);


    // Generate Code
    vector<pair<__Instruction, string>> __generateCode() const;
};


}  // End namespace CMM
