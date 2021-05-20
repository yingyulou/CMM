/*
    SemanticAnalyzer.h
    ==================
        Class SemanticAnalyzer header.
*/

#ifndef __CMM_SEMANTIC_ANALYZER_H
#define __CMM_SEMANTIC_ANALYZER_H

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
    SemanticAnalyzer(AST *root);


    // Get Symbol Table
    unordered_map<string, unordered_map<string, pair<int, int>>> getSymbolTable() const;


private:

    // Attribute
    AST *__root;
};


}  // End namespace CMM


#endif  // __CMM_SEMANTIC_ANALYZER_H
