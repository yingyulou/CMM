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

void VM::__runInstruction(const pair<INSTRUCTION, int> &instructionPair)
{
    switch (instructionPair.first)
    {
        case INSTRUCTION::LDC:
            SS[0] = instructionPair.second;
            break;

        case INSTRUCTION::LD:
            SS[0] = SS[SS[1] - SS[0] - 1];
            break;

        case INSTRUCTION::ABSLD:
            SS[0] = SS[SS[0] + 2];
            break;

        case INSTRUCTION::SAV:
            SS[SS[1] - SS[0] - 1] = SS.back();
            break;

        case INSTRUCTION::ABSSAV:
            SS[SS[0] + 2] = SS.back();
            break;

        case INSTRUCTION::SAVSP:
            SS[1] = SS.size() - 1;
            break;

        case INSTRUCTION::PUSH:
            SS.push_back(SS[0]);
            break;

        case INSTRUCTION::POP:
            SS.pop_back();
            break;

        case INSTRUCTION::PUSHBP:
            SS.push_back(SS[1]);
            break;

        case INSTRUCTION::POPBP:
            SS[1] = SS.back();
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

            if (!SS[0])
            {
                IP += instructionPair.second - 1;
            }

            break;

        case INSTRUCTION::ADD:
            SS[0] = SS.back() + SS[0];
            break;

        case INSTRUCTION::SUB:
            SS[0] = SS.back() - SS[0];
            break;

        case INSTRUCTION::MUL:
            SS[0] = SS.back() * SS[0];
            break;

        case INSTRUCTION::DIV:
            SS[0] = SS.back() / SS[0];
            break;

        case INSTRUCTION::LT:
            SS[0] = SS.back() < SS[0];
            break;

        case INSTRUCTION::LE:
            SS[0] = SS.back() <= SS[0];
            break;

        case INSTRUCTION::GT:
            SS[0] = SS.back() > SS[0];
            break;

        case INSTRUCTION::GE:
            SS[0] = SS.back() >= SS[0];
            break;

        case INSTRUCTION::EQ:
            SS[0] = SS.back() == SS[0];
            break;

        case INSTRUCTION::NE:
            SS[0] = SS.back() != SS[0];
            break;

        case INSTRUCTION::INPUT:
            scanf("%d", &SS[0]);
            break;

        case INSTRUCTION::OUTPUT:
            printf("%d\n", SS[0]);
            break;

        case INSTRUCTION::STOP:
            exit(0);
            break;
    }
}


}  // End namespace CMM
