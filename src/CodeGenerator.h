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
<<<<<<< HEAD
// Class CodeGenerator
=======
// Class __CodeGenerator
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __CodeGenerator
{
    // Friend
    friend class Core;

<<<<<<< HEAD
    // Constructor
    CodeGenerator(AST *root = nullptr, const unordered_map<string, unordered_map<string, pair<int, int>>> &symbolTable = {});
=======
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a

public:

    // Constructor
    __CodeGenerator(__AST *root = nullptr, const unordered_map<string, unordered_map<string, pair<int, int>>> &symbolTable = {});


private:

    // Attribute
    __AST *__root;
    unordered_map<string, unordered_map<string, pair<int, int>>> __symbolTable;


    // Generate Number Code
<<<<<<< HEAD
    vector<pair<Instruction, string>> __generateNumberCode(AST *root, const string &) const;


    // Generate CompoundStmt Code
    vector<pair<Instruction, string>> __generateCompoundStmtCode(AST *root, const string &curFuncName) const;


    // Generate StmtList Code
    vector<pair<Instruction, string>> __generateStmtListCode(AST *root, const string &curFuncName) const;


    // Generate Stmt Code
    vector<pair<Instruction, string>> __generateStmtCode(AST *root, const string &curFuncName) const;


    // Generate IfStmt Code
    vector<pair<Instruction, string>> __generateIfStmtCode(AST *root, const string &curFuncName) const;


    // Generate WhileStmt Code
    vector<pair<Instruction, string>> __generateWhileStmtCode(AST *root, const string &curFuncName) const;


    // Generate ReturnStmt Code
    vector<pair<Instruction, string>> __generateReturnStmtCode(AST *root, const string &curFuncName) const;


    // Generate Expr Code
    vector<pair<Instruction, string>> __generateExprCode(AST *root, const string &curFuncName) const;


    // Generate Var Code
    vector<pair<Instruction, string>> __generateVarCode(AST *root, const string &curFuncName) const;


    // Generate SimpleExpr Code
    vector<pair<Instruction, string>> __generateSimpleExprCode(AST *root, const string &curFuncName) const;


    // Generate RelOp Code
    vector<pair<Instruction, string>> __generateRelOpCode(AST *root, const string &) const;


    // Generate AddExpr Code
    vector<pair<Instruction, string>> __generateAddExprCode(AST *root, const string &curFuncName) const;


    // Generate AddOp Code
    vector<pair<Instruction, string>> __generateAddOpCode(AST *root, const string &) const;


    // Generate Term Code
    vector<pair<Instruction, string>> __generateTermCode(AST *root, const string &curFuncName) const;


    // Generate MulOp Code
    vector<pair<Instruction, string>> __generateMulOpCode(AST *root, const string &) const;


    // Generate Factor Code
    vector<pair<Instruction, string>> __generateFactorCode(AST *root, const string &curFuncName) const;


    // Generate Call Code
    vector<pair<Instruction, string>> __generateCallCode(AST *root, const string &curFuncName) const;


    // Generate ArgList Code
    vector<pair<Instruction, string>> __generateArgListCode(AST *root, const string &curFuncName) const;


    // Generate Assign Code
    vector<pair<Instruction, string>> __generateAssignCode(AST *root, const string &curFuncName) const;
=======
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
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a


    // Generate Global Variable Code
    vector<pair<__Instruction, string>> __generateGlobalVariableCode() const;


    // Generate Main Prepare Code
    vector<pair<__Instruction, string>> __generateMainPrepareCode() const;


    // Generate Global Code
    vector<pair<__Instruction, string>> __generateGlobalCode() const;


    // Create CodeMap
<<<<<<< HEAD
    unordered_map<string, vector<pair<Instruction, string>>> __createCodeMap() const;


    // Merge CodeMap
    static pair<vector<pair<Instruction, string>>, unordered_map<string, int>> __mergeCodeMap(
        const unordered_map<string, vector<pair<Instruction, string>>> &codeMap);


    // Translate Call
    static void __translateCall(vector<pair<Instruction, string>> &codeList, const unordered_map<string, int> &funcJmpMap);
=======
    unordered_map<string, vector<pair<__Instruction, string>>> __createCodeMap() const;


    // Merge CodeMap
    static pair<vector<pair<__Instruction, string>>, unordered_map<string, int>> __mergeCodeMap(
        const unordered_map<string, vector<pair<__Instruction, string>>> &codeMap);


    // Translate Call
    static void __translateCall(vector<pair<__Instruction, string>> &codeList, const unordered_map<string, int> &funcJmpMap);
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a


    // Generate Code
    vector<pair<__Instruction, string>> __generateCode() const;
};


}  // End namespace CMM
