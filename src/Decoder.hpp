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

const vector<pair<INSTRUCTION, int>> &Decoder::instructionList() const
{
    return __instructionList;
}


////////////////////////////////////////////////////////////////////////////////
// Parse Instruction File
////////////////////////////////////////////////////////////////////////////////

void Decoder::__parseByteCodeFile()
{
    INSTRUCTION codeEnum;
    int codeVal;

    FILE *f = fopen(__byteCodeFilePath.c_str(), "rb");

    while (fread(&codeEnum, sizeof(char), 1, f))
    {
        switch (codeEnum)
        {
            case INSTRUCTION::LDC:
            case INSTRUCTION::JMP:
            case INSTRUCTION::JZ:
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
