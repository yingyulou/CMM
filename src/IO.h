/*
    IO.h
    ====
        Class __IO header.
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include "Instruction.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::pair;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Class IO
=======
// Class __IO
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __IO
{
<<<<<<< HEAD
public:

    // Parse Instruction File
    static vector<pair<Instruction, int>> parseInstructionFile(const string &instructionFilePath);


    // Output Instruction
    static void outputInstruction(const string &instructionFilePath, const vector<pair<Instruction, string>> &codeList);
=======
    // Friend
    friend class Core;
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a


private:

<<<<<<< HEAD
    // Parse Instruction File
    static vector<pair<Instruction, int>> __parseInstructionFile(const string &instructionFilePath);


    // Output Instruction
    static void __outputInstruction(const string &instructionFilePath, const vector<pair<Instruction, string>> &codeList);
=======
    // Parse __Instruction File
    static vector<pair<__Instruction, int>> __parseInstructionFile(const string &instructionFilePath);


    // Output __Instruction
    static void __outputInstruction(const string &instructionFilePath,const vector<pair<__Instruction, string>> &codeList);
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
};


}  // End namespace CMM
