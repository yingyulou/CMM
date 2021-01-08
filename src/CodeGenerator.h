/*
    CodeGenerator.h
    ===============
        Class CodeGenerator header.
*/

#ifndef __CMM_CODE_GENERATOR_H
#define __CMM_CODE_GENERATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "AST.h"

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
    CodeGenerator(AST *root, const string &outputFilePath);


    // Generate Code
    void GenerateCode();


private:

    // Attribute
    AST *__root;
    unordered_map<string, unordered_map<string, pair<int, int>>> __symbolTable;
    string __outputFilePath;
    string __nowFuncName;


    // Generate Number Code
    vector<string> __generateNumberCode(AST *root) const;


    // Generate CompoundStmt Code
    vector<string> __generateCompoundStmtCode(AST *root) const;


    // Generate StmtList Code
    vector<string> __generateStmtListCode(AST *root) const;


    // Generate Stmt Code
    vector<string> __generateStmtCode(AST *root) const;


    // Generate IfStmt Code
    vector<string> __generateIfStmtCode(AST *root) const;


    // Generate WhileStmt Code
    vector<string> __generateWhileStmtCode(AST *root) const;


    // Generate ReturnStmt Code
    vector<string> __generateReturnStmtCode(AST *root) const;


    // Generate Expr Code
    vector<string> __generateExprCode(AST *root) const;


    // Generate Var Code
    vector<string> __generateVarCode(AST *root) const;


    // Generate SimpleExpr Code
    vector<string> __generateSimpleExprCode(AST *root) const;


    // Generate RelOp Code
    vector<string> __generateRelOpCode(AST *root) const;


    // Generate AddExpr Code
    vector<string> __generateAddExprCode(AST *root) const;


    // Generate AddOp Code
    vector<string> __generateAddOpCode(AST *root) const;


    // Generate Term Code
    vector<string> __generateTermCode(AST *root) const;


    // Generate MulOp Code
    vector<string> __generateMulOpCode(AST *root) const;


    // Generate Factor Code
    vector<string> __generateFactorCode(AST *root) const;


    // Generate Call Code
    vector<string> __generateCallCode(AST *root) const;


    // Generate ArgList Code
    vector<string> __generateArgListCode(AST *root) const;


    // Generate Assign Code
    vector<string> __generateAssignCode(AST *root) const;


    // Create CodeMap
    unordered_map<string, vector<string>> __createCodeMap();


    // Create FuncJmpMap
    unordered_map<string, int> __createFuncJmpMap(
        const unordered_map<string, vector<string>> &codeMap) const;


    // Translate Call
    void __translateCall(unordered_map<string, vector<string>> &codeMap) const;


    // Generate Global Variable Code
    vector<string> __generateGlobalVariableCode() const;


    // Generate Main Prepare Code
    vector<string> __generateMainPrepareCode() const;


    // Output Code
    void __outputCode(const unordered_map<string, vector<string>> &codeMap,
        const vector<string> &globalVariableCodeList,
        const vector<string> &mainPrepareCodeList) const;
};


}  // End namespace CMM


#endif  // __CMM_CODE_GENERATOR_H
