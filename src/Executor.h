/*
    Executor.h
    ==========
        Class Executor header.
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include "Instruction.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::pair;


////////////////////////////////////////////////////////////////////////////////
// Class Executer
////////////////////////////////////////////////////////////////////////////////

class Executer
{
public:

    // Constructor
    explicit Executer(const string &instructionFilePath);


    // Execute
    void Execute() const;


private:

    // Attribute
    vector<pair<INSTRUCTION, int>> __instructionList;


    // Parse Instruction File
    vector<pair<INSTRUCTION, int>> __parseInstructionFile(
        const string &instructionFilePath) const;
};


}  // End namespace CMM
