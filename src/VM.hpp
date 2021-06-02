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

VM::VM(const vector<pair<INSTRUCTION, int>> &instructionList):
    AX(0),
    IP(0),
    BP(0),
    CS(instructionList) {}


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

void VM::__runInstruction(const pair<INSTRUCTION, int> &instructionPair)
{
    switch (instructionPair.first)
    {
        case INSTRUCTION::LDC:
            AX = instructionPair.second;
            break;

        case INSTRUCTION::LD:
            AX = SS[BP - AX - 1];
            break;

        case INSTRUCTION::ABSLD:
            AX = SS[AX];
            break;

        case INSTRUCTION::SAV:
            SS[BP - AX - 1] = SS.back();
            break;

        case INSTRUCTION::ABSSAV:
            SS[AX] = SS.back();
            break;

        case INSTRUCTION::SAVSP:
            BP = SS.size() - 1;
            break;

        case INSTRUCTION::PUSH:
            SS.push_back(AX);
            break;

        case INSTRUCTION::POP:
            SS.pop_back();
            break;

        case INSTRUCTION::PUSHBP:
            SS.push_back(BP);
            break;

        case INSTRUCTION::POPBP:
            BP = SS.back();
            SS.pop_back();
            break;

        case INSTRUCTION::PUSHIP:
            SS.push_back(IP);
            break;

        case INSTRUCTION::POPIP:
            IP = SS.back() + 1;
            SS.pop_back();
            break;

        case INSTRUCTION::PUSHSP:
            SS.push_back(SS.size() - 1);
            break;

        case INSTRUCTION::JMP:
            IP += instructionPair.second - 1;
            break;

        case INSTRUCTION::JZ:

            if (!AX)
            {
                IP += instructionPair.second - 1;
            }

            break;

        case INSTRUCTION::ADD:
            AX = SS.back() + AX;
            break;

        case INSTRUCTION::SUB:
            AX = SS.back() - AX;
            break;

        case INSTRUCTION::MUL:
            AX = SS.back() * AX;
            break;

        case INSTRUCTION::DIV:
            AX = SS.back() / AX;
            break;

        case INSTRUCTION::LT:
            AX = SS.back() < AX;
            break;

        case INSTRUCTION::LE:
            AX = SS.back() <= AX;
            break;

        case INSTRUCTION::GT:
            AX = SS.back() > AX;
            break;

        case INSTRUCTION::GE:
            AX = SS.back() >= AX;
            break;

        case INSTRUCTION::EQ:
            AX = SS.back() == AX;
            break;

        case INSTRUCTION::NE:
            AX = SS.back() != AX;
            break;

        case INSTRUCTION::INPUT:
            scanf("%d", &AX);
            break;

        case INSTRUCTION::OUTPUT:
            printf("%d\n", AX);
            break;

        case INSTRUCTION::STOP:
            exit(0);
            break;
    }
}


}  // End namespace CMM
