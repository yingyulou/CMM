/*
    Executor.hpp
    ============
        Class Executor implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "Executor.h"
#include "Instruction.h"
#include "VM.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::getline;
using std::stoi;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

Executer::Executer(const string &instructionFilePath):
    __instructionList(__parseInstructionFile(instructionFilePath)) {}


////////////////////////////////////////////////////////////////////////////////
// Execute
////////////////////////////////////////////////////////////////////////////////

void Executer::Execute() const
{
    VM(__instructionList).Run();
}


////////////////////////////////////////////////////////////////////////////////
// Parse Instruction File
////////////////////////////////////////////////////////////////////////////////

vector<pair<INSTRUCTION, int>> Executer::__parseInstructionFile(
    const string &instructionFilePath) const
{
    vector<pair<INSTRUCTION, int>> instructionList;

    ifstream f(instructionFilePath);
    string line;

    while (getline(f, line))
    {
        if (line.substr(0, 3) == "LDC")
        {
            instructionList.emplace_back(INSTRUCTION::LDC, stoi(line.substr(4)));
        }
        else if (line == "LD")
        {
            instructionList.emplace_back(INSTRUCTION::LD, 0);
        }
        else if (line == "ABSLD")
        {
            instructionList.emplace_back(INSTRUCTION::ABSLD, 0);
        }
        else if (line == "SAV")
        {
            instructionList.emplace_back(INSTRUCTION::SAV, 0);
        }
        else if (line == "ABSSAV")
        {
            instructionList.emplace_back(INSTRUCTION::ABSSAV, 0);
        }
        else if (line == "SAVSP")
        {
            instructionList.emplace_back(INSTRUCTION::SAVSP, 0);
        }
        else if (line == "PUSH")
        {
            instructionList.emplace_back(INSTRUCTION::PUSH, 0);
        }
        else if (line == "POP")
        {
            instructionList.emplace_back(INSTRUCTION::POP, 0);
        }
        else if (line == "PUSHBP")
        {
            instructionList.emplace_back(INSTRUCTION::PUSHBP, 0);
        }
        else if (line == "POPBP")
        {
            instructionList.emplace_back(INSTRUCTION::POPBP, 0);
        }
        else if (line == "PUSHIP")
        {
            instructionList.emplace_back(INSTRUCTION::PUSHIP, 0);
        }
        else if (line == "POPIP")
        {
            instructionList.emplace_back(INSTRUCTION::POPIP, 0);
        }
        else if (line == "PUSHSP")
        {
            instructionList.emplace_back(INSTRUCTION::PUSHSP, 0);
        }
        else if (line.substr(0, 3) == "JMP")
        {
            instructionList.emplace_back(INSTRUCTION::JMP, stoi(line.substr(4)));
        }
        else if (line.substr(0, 2) == "JZ")
        {
            instructionList.emplace_back(INSTRUCTION::JZ, stoi(line.substr(3)));
        }
        else if (line == "ADD")
        {
            instructionList.emplace_back(INSTRUCTION::ADD, 0);
        }
        else if (line == "SUB")
        {
            instructionList.emplace_back(INSTRUCTION::SUB, 0);
        }
        else if (line == "MUL")
        {
            instructionList.emplace_back(INSTRUCTION::MUL, 0);
        }
        else if (line == "DIV")
        {
            instructionList.emplace_back(INSTRUCTION::DIV, 0);
        }
        else if (line == "LT")
        {
            instructionList.emplace_back(INSTRUCTION::LT, 0);
        }
        else if (line == "LE")
        {
            instructionList.emplace_back(INSTRUCTION::LE, 0);
        }
        else if (line == "GT")
        {
            instructionList.emplace_back(INSTRUCTION::GT, 0);
        }
        else if (line == "GE")
        {
            instructionList.emplace_back(INSTRUCTION::GE, 0);
        }
        else if (line == "EQ")
        {
            instructionList.emplace_back(INSTRUCTION::EQ, 0);
        }
        else if (line == "NE")
        {
            instructionList.emplace_back(INSTRUCTION::NE, 0);
        }
        else if (line == "INPUT")
        {
            instructionList.emplace_back(INSTRUCTION::INPUT, 0);
        }
        else if (line == "OUTPUT")
        {
            instructionList.emplace_back(INSTRUCTION::OUTPUT, 0);
        }
        else if (line == "STOP")
        {
            instructionList.emplace_back(INSTRUCTION::STOP, 0);
        }
    }

    f.close();

    return instructionList;
}


}  // End namespace CMM
