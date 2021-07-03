/*
    Decoder.h
    =========
        Class Decoder header.
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

class Decoder
{
public:

    // Constructor
    explicit Decoder(const string &byteCodeFilePath);


    // Getter: __instructionList
    const vector<pair<INSTRUCTION, int>> &instructionList() const;


private:

    // Attribute
    string __byteCodeFilePath;
    vector<pair<INSTRUCTION, int>> __instructionList;


    // Parse Byte Code File
    void __parseByteCodeFile();
};


}  // End namespace CMM
