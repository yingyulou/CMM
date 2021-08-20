/*
    SemanticAnalyzer.h
    ==================
        Class SemanticAnalyzer header.
*/

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include "AST.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::unordered_map;
using std::pair;


////////////////////////////////////////////////////////////////////////////////
// Class SemanticAnalyzer
////////////////////////////////////////////////////////////////////////////////

class SemanticAnalyzer
{
public:

    // Constructor
    SemanticAnalyzer(AST *root = nullptr);


    // Semantic Analysis
    unordered_map<string, unordered_map<string, pair<int, int>>>
    semanticAnalysis() const;


private:

    // Attribute
    AST *__root;


    // Semantic Analysis
    unordered_map<string, unordered_map<string, pair<int, int>>>
    __semanticAnalysis() const;
};


}  // End namespace CMM
