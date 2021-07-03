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


    // Run VM
    void Run();


private:

    // Attribute
    vector<pair<Instruction, int>> CS;
    int IP;
    vector<int> SS;


    // Run Instruction
    void __runInstruction(const pair<Instruction, int> &instructionPair);
};


}  // End namespace CMM
