/*
    IO.h
    ====
        Class IO header.
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include "Instruction.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::pair;


////////////////////////////////////////////////////////////////////////////////
// Class IO
////////////////////////////////////////////////////////////////////////////////

class IO
{
public:

    // Parse Instruction File
    static vector<pair<Instruction, int>> parseInstructionFile(
        const string &instructionFilePath);


    // Output Instruction
    static void outputInstruction(const string &instructionFilePath,
        const vector<pair<Instruction, string>> &codeList);


private:

    // Parse Instruction File
    static vector<pair<Instruction, int>> __parseInstructionFile(
        const string &instructionFilePath);


    // Output Instruction
    static void __outputInstruction(const string &instructionFilePath,
        const vector<pair<Instruction, string>> &codeList);
};


}  // End namespace CMM
