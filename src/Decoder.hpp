/*
    Decoder.hpp
    ===========
        Class Decoder implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <cstdio>
#include "Decoder.h"
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
// Constructor
////////////////////////////////////////////////////////////////////////////////

Decoder::Decoder(const string &byteCodeFilePath):
    __byteCodeFilePath(byteCodeFilePath)
{
    __parseByteCodeFile();
}


////////////////////////////////////////////////////////////////////////////////
// Getter: __instructionList
////////////////////////////////////////////////////////////////////////////////

const vector<pair<Instruction, int>> &Decoder::instructionList() const
{
    return __instructionList;
}


////////////////////////////////////////////////////////////////////////////////
// Parse Instruction File
////////////////////////////////////////////////////////////////////////////////

void Decoder::__parseByteCodeFile()
{
    Instruction codeEnum;
    int codeVal;

    FILE *f = fopen(__byteCodeFilePath.c_str(), "rb");

    while (fread(&codeEnum, sizeof(Instruction), 1, f))
    {
        switch (codeEnum)
        {
            case Instruction::LDC:
            case Instruction::JMP:
            case Instruction::JZ:
                fread(&codeVal, sizeof(int), 1, f);
                break;

            default:
                break;
        }

        __instructionList.emplace_back(codeEnum, codeVal);
    }

    fclose(f);
}


}  // End namespace CMM
