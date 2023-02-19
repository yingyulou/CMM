/*
    SemanticAnalyzer.h
    ==================
        Class __SemanticAnalyzer header.
*/

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include "AST.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::unordered_map;
using std::pair;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Class SemanticAnalyzer
=======
// Class __SemanticAnalyzer
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __SemanticAnalyzer
{
    // Friend
    friend class Core;


public:

<<<<<<< HEAD
    // Semantic Analysis
    unordered_map<string, unordered_map<string, pair<int, int>>> semanticAnalysis() const;
=======
    // Constructor
    __SemanticAnalyzer(__AST *root = nullptr);
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a


private:

    // Attribute
    __AST *__root;


    // Semantic Analysis
    unordered_map<string, unordered_map<string, pair<int, int>>> __semanticAnalysis() const;
};


}  // End namespace CMM
