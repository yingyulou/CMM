/*
    VM.h
    ====
        Class VM header.
*/

#pragma once

#include <vector>
#include <utility>
#include "Instruction.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::pair;


////////////////////////////////////////////////////////////////////////////////
// Class VM
////////////////////////////////////////////////////////////////////////////////

class VM
{
public:

    // Constructor
    explicit VM(const vector<pair<Instruction, int>> &instructionList);


    // Start
    void Start();


private:

    // Attribute
    vector<pair<Instruction, int>> __CS;
    int __IP;
    vector<int> __SS;


    // Exec Instruction
    void __execInstruction(const pair<Instruction, int> &instructionPair);
};


}  // End namespace CMM
