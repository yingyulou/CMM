/*
    VM.h
    ====
        Class VM header.
*/

#pragma once

#include <vector>
#include <utility>
#include "Instruction.hpp"

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
    explicit VM(const vector<pair<Instruction, int>> &CS);


    // Run
    void run();


private:

    // Attribute
    vector<pair<Instruction, int>> __CS;
    int __IP;
    int __AX;
    int __BP;
    vector<int> __SS;


    // Exec Instruction
    void __execInstruction(const pair<Instruction, int> &instructionPair);


    // Run
    void __run();
};


}  // End namespace CMM
