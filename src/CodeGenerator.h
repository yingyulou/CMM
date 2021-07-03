/*
    CodeGenerator.h
    ===============
        Class CodeGenerator header.
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cstdio>
#include "AST.h"
#include "Instruction.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::unordered_map;
using std::pair;


////////////////////////////////////////////////////////////////////////////////
// Class CodeGenerator
////////////////////////////////////////////////////////////////////////////////

class CodeGenerator
{
public:

    // Constructor
    CodeGenerator(AST *root, const string &asmCodeFilePath,
        const string &byteCodeFilePath);


    // Generate Code
    void GenerateCode();


private:

    // Attribute
    AST *__root;
    unordered_map<string, unordered_map<string, pair<int, int>>> __symbolTable;
    string __asmCodeFilePath, __byteCodeFilePath;
    string __nowFuncName;


    // Generate Number Code
    vector<pair<INSTRUCTION, string>> __generateNumberCode(AST *root) const;


    // Generate CompoundStmt Code
    vector<pair<INSTRUCTION, string>> __generateCompoundStmtCode(AST *root) const;


    // Generate StmtList Code
    vector<pair<INSTRUCTION, string>> __generateStmtListCode(AST *root) const;


    // Generate Stmt Code
    vector<pair<INSTRUCTION, string>> __generateStmtCode(AST *root) const;


    // Generate IfStmt Code
    vector<pair<INSTRUCTION, string>> __generateIfStmtCode(AST *root) const;


    // Generate WhileStmt Code
    vector<pair<INSTRUCTION, string>> __generateWhileStmtCode(AST *root) const;


    // Generate ReturnStmt Code
    vector<pair<INSTRUCTION, string>> __generateReturnStmtCode(AST *root) const;


    // Generate Expr Code
    vector<pair<INSTRUCTION, string>> __generateExprCode(AST *root) const;


    // Generate Var Code
    vector<pair<INSTRUCTION, string>> __generateVarCode(AST *root) const;


    // Generate SimpleExpr Code
    vector<pair<INSTRUCTION, string>> __generateSimpleExprCode(AST *root) const;


    // Generate RelOp Code
    vector<pair<INSTRUCTION, string>> __generateRelOpCode(AST *root) const;


    // Generate AddExpr Code
    vector<pair<INSTRUCTION, string>> __generateAddExprCode(AST *root) const;


    // Generate AddOp Code
    vector<pair<INSTRUCTION, string>> __generateAddOpCode(AST *root) const;


    // Generate Term Code
    vector<pair<INSTRUCTION, string>> __generateTermCode(AST *root) const;


    // Generate MulOp Code
    vector<pair<INSTRUCTION, string>> __generateMulOpCode(AST *root) const;


    // Generate Factor Code
    vector<pair<INSTRUCTION, string>> __generateFactorCode(AST *root) const;


    // Generate Call Code
    vector<pair<INSTRUCTION, string>> __generateCallCode(AST *root) const;


    // Generate ArgList Code
    vector<pair<INSTRUCTION, string>> __generateArgListCode(AST *root) const;


    // Generate Assign Code
    vector<pair<INSTRUCTION, string>> __generateAssignCode(AST *root) const;


    // Create CodeMap
    unordered_map<string, vector<pair<INSTRUCTION, string>>> __createCodeMap();


    // Create FuncJmpMap
    unordered_map<string, int> __createFuncJmpMap(
        const unordered_map<string, vector<pair<INSTRUCTION, string>>> &codeMap) const;


    // Translate Call
    void __translateCall(
        unordered_map<string, vector<pair<INSTRUCTION, string>>> &codeMap) const;


    // Generate Global Variable Code
    vector<pair<INSTRUCTION, string>> __generateGlobalVariableCode() const;


    // Generate Main Prepare Code
    vector<pair<INSTRUCTION, string>> __generateMainPrepareCode() const;


    // Output ASM Code Helper
    void __outputASMCodeHelper(FILE *fo, INSTRUCTION codeEnum,
        const string &codeValStr) const;


    // Output ASM Code
    void __outputASMCode(
        const unordered_map<string, vector<pair<INSTRUCTION, string>>> &codeMap,
        const vector<pair<INSTRUCTION, string>> &globalVariableCodeList,
        const vector<pair<INSTRUCTION, string>> &mainPrepareCodeList) const;


    // Output Byte Code Helper
    void __outputByteCodeHelper(FILE *fo, INSTRUCTION codeEnum,
        const string &codeValStr) const;


    // Output Byte Code
    void __outputByteCode(
        const unordered_map<string, vector<pair<INSTRUCTION, string>>> &codeMap,
        const vector<pair<INSTRUCTION, string>> &globalVariableCodeList,
        const vector<pair<INSTRUCTION, string>> &mainPrepareCodeList) const;
};


}  // End namespace CMM
