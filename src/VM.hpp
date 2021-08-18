/*
    VM.hpp
    ======
        Class VM implementation.
*/

#pragma once

#include <vector>
#include <utility>
#include <stdexcept>
#include <cstdio>
#include "VM.h"
#include "Instruction.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::pair;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

VM::VM(const vector<pair<Instruction, int>> &instructionList):
    __CS(instructionList),
    __IP(0),
    __SS(2) {}


////////////////////////////////////////////////////////////////////////////////
// Start
////////////////////////////////////////////////////////////////////////////////

void VM::Start()
{
    for (; __IP < __CS.size(); __execInstruction(__CS[__IP]), __IP++);
}


////////////////////////////////////////////////////////////////////////////////
// Exec Instruction
////////////////////////////////////////////////////////////////////////////////

void VM::__execInstruction(const pair<Instruction, int> &instructionPair)
{
    switch (instructionPair.first)
    {
        case Instruction::LDC:
            __SS[0] = instructionPair.second;
            break;

        case Instruction::LD:
            __SS[0] = __SS[__SS[1] - __SS[0]];
            break;

        case Instruction::ALD:
            __SS[0] = __SS[__SS[0] + 2];
            break;

        case Instruction::ST:
            __SS[__SS[1] - __SS[0]] = __SS.back();
            break;

        case Instruction::AST:
            __SS[__SS[0] + 2] = __SS.back();
            break;

        case Instruction::PUSH:
            __SS.push_back(__SS[0]);
            break;

        case Instruction::POP:
            __SS.pop_back();
            break;

        case Instruction::JMP:
            __IP += instructionPair.second - 1;
            break;

        case Instruction::JZ:

            if (!__SS[0])
            {
                __IP += instructionPair.second - 1;
            }

            break;

        case Instruction::ADD:
            __SS[0] = __SS.back() + __SS[0];
            break;

        case Instruction::SUB:
            __SS[0] = __SS.back() - __SS[0];
            break;

        case Instruction::MUL:
            __SS[0] = __SS.back() * __SS[0];
            break;

        case Instruction::DIV:
            __SS[0] = __SS.back() / __SS[0];
            break;

        case Instruction::LT:
            __SS[0] = __SS.back() < __SS[0];
            break;

        case Instruction::LE:
            __SS[0] = __SS.back() <= __SS[0];
            break;

        case Instruction::GT:
            __SS[0] = __SS.back() > __SS[0];
            break;

        case Instruction::GE:
            __SS[0] = __SS.back() >= __SS[0];
            break;

        case Instruction::EQ:
            __SS[0] = __SS.back() == __SS[0];
            break;

        case Instruction::NE:
            __SS[0] = __SS.back() != __SS[0];
            break;

        case Instruction::IN:
            scanf("%d", &__SS[0]);
            break;

        case Instruction::OUT:
            printf("%d\n", __SS[0]);
            break;

        case Instruction::ADDR:
            __SS[0] = __SS.size() - instructionPair.second;
            break;

        case Instruction::CALL:
            __SS.push_back(__SS[1]);
            __SS[1] = __SS.size() - 2;
            __SS.push_back(__IP);
            __IP += instructionPair.second - 1;
            break;

        case Instruction::RET:
            __IP = __SS.back();
            __SS.pop_back();
            __SS[1] = __SS.back();
            __SS.pop_back();
            break;

        default:
            throw runtime_error("Invalid Instruction");
    }
}


}  // End namespace CMM
