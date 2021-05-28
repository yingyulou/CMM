/*
    VM.h
    ====
        Class VM header.
*/

#ifndef __CMM_VM_H
#define __CMM_VM_H

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
    explicit VM(const vector<pair<INSTRUCTION, int>> &instructionList);


    // Run VM
    void Run();


private:

    // Attribute
    int AX, IP, BP;
    vector<pair<INSTRUCTION, int>> CS;
    vector<int> SS;


    // Run Instruction
    void __runInstruction(const pair<INSTRUCTION, int> &instructionPair);
};


}  // End namespace CMM


#endif  // __CMM_VM_H
