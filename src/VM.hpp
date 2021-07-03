/*
    VM.hpp
    ======
        Class VM implementation.
*/

#pragma once

#include <vector>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include "VM.h"
#include "Instruction.h"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::pair;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

VM::VM(const vector<pair<Instruction, int>> &instructionList):
    CS(instructionList),
    IP(0),
    SS(2) {}


////////////////////////////////////////////////////////////////////////////////
// Run
////////////////////////////////////////////////////////////////////////////////

void VM::Run()
{
    while (true)
    {
        __runInstruction(CS[IP]);
        IP++;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Run Instruction
////////////////////////////////////////////////////////////////////////////////

void VM::__runInstruction(const pair<Instruction, int> &instructionPair)
{
    switch (instructionPair.first)
    {
        case Instruction::LDC:
            SS[0] = instructionPair.second;
            break;

        case Instruction::LD:
            SS[0] = SS[SS[1] - SS[0] - 1];
            break;

        case Instruction::ABSLD:
            SS[0] = SS[SS[0] + 2];
            break;

        case Instruction::SAV:
            SS[SS[1] - SS[0] - 1] = SS.back();
            break;

        case Instruction::ABSSAV:
            SS[SS[0] + 2] = SS.back();
            break;

        case Instruction::SAVSP:
            SS[1] = SS.size() - 1;
            break;

        case Instruction::PUSH:
            SS.push_back(SS[0]);
            break;

        case Instruction::POP:
            SS.pop_back();
            break;

        case Instruction::PUSHBP:
            SS.push_back(SS[1]);
            break;

        case Instruction::POPBP:
            SS[1] = SS.back();
            SS.pop_back();
            break;

        case Instruction::PUSHIP:
            SS.push_back(IP);
            break;

        case Instruction::POPIP:
            IP = SS.back() + 1;
            SS.pop_back();
            break;

        case Instruction::PUSHSP:
            SS.push_back(SS.size() - 1);
            break;

        case Instruction::JMP:
            IP += instructionPair.second - 1;
            break;

        case Instruction::JZ:

            if (!SS[0])
            {
                IP += instructionPair.second - 1;
            }

            break;

        case Instruction::ADD:
            SS[0] = SS.back() + SS[0];
            break;

        case Instruction::SUB:
            SS[0] = SS.back() - SS[0];
            break;

        case Instruction::MUL:
            SS[0] = SS.back() * SS[0];
            break;

        case Instruction::DIV:
            SS[0] = SS.back() / SS[0];
            break;

        case Instruction::LT:
            SS[0] = SS.back() < SS[0];
            break;

        case Instruction::LE:
            SS[0] = SS.back() <= SS[0];
            break;

        case Instruction::GT:
            SS[0] = SS.back() > SS[0];
            break;

        case Instruction::GE:
            SS[0] = SS.back() >= SS[0];
            break;

        case Instruction::EQ:
            SS[0] = SS.back() == SS[0];
            break;

        case Instruction::NE:
            SS[0] = SS.back() != SS[0];
            break;

        case Instruction::INPUT:
            scanf("%d", &SS[0]);
            break;

        case Instruction::OUTPUT:
            printf("%d\n", SS[0]);
            break;

        case Instruction::STOP:
            exit(0);
            break;
    }
}


}  // End namespace CMM
