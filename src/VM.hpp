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

VM::VM(const vector<pair<Instruction, int>> &CS):
    __CS(CS),
    __IP(0) {}


////////////////////////////////////////////////////////////////////////////////
// Run
////////////////////////////////////////////////////////////////////////////////

void VM::run()
{
    __run();
}


////////////////////////////////////////////////////////////////////////////////
// Exec Instruction
////////////////////////////////////////////////////////////////////////////////

void VM::__execInstruction(const pair<Instruction, int> &instructionPair)
{
    switch (instructionPair.first)
    {
        case Instruction::LDC:
            __AX = instructionPair.second;
            break;

        case Instruction::LD:
            __AX = __SS[__BP - __AX];
            break;

        case Instruction::ALD:
            __AX = __SS[__AX];
            break;

        case Instruction::ST:
            __SS[__BP - __AX] = __SS.back();
            break;

        case Instruction::AST:
            __SS[__AX] = __SS.back();
            break;

        case Instruction::PUSH:
            __SS.push_back(__AX);
            break;

        case Instruction::POP:
            __SS.pop_back();
            break;

        case Instruction::JMP:
            __IP += instructionPair.second - 1;
            break;

        case Instruction::JZ:

            if (!__AX)
            {
                __IP += instructionPair.second - 1;
            }

            break;

        case Instruction::ADD:
            __AX = __SS.back() + __AX;
            break;

        case Instruction::SUB:
            __AX = __SS.back() - __AX;
            break;

        case Instruction::MUL:
            __AX = __SS.back() * __AX;
            break;

        case Instruction::DIV:
            __AX = __SS.back() / __AX;
            break;

        case Instruction::LT:
            __AX = __SS.back() < __AX;
            break;

        case Instruction::LE:
            __AX = __SS.back() <= __AX;
            break;

        case Instruction::GT:
            __AX = __SS.back() > __AX;
            break;

        case Instruction::GE:
            __AX = __SS.back() >= __AX;
            break;

        case Instruction::EQ:
            __AX = __SS.back() == __AX;
            break;

        case Instruction::NE:
            __AX = __SS.back() != __AX;
            break;

        case Instruction::IN:
            scanf("%d", &__AX);
            break;

        case Instruction::OUT:
            printf("%d\n", __AX);
            break;

        case Instruction::ADDR:
            __AX = __SS.size() - instructionPair.second;
            break;

        case Instruction::CALL:
            __SS.push_back(__BP);
            __BP = __SS.size() - 2;
            __SS.push_back(__IP);
            __IP += instructionPair.second - 1;
            break;

        case Instruction::RET:
            __IP = __SS.back();
            __SS.pop_back();
            __BP = __SS.back();
            __SS.pop_back();
            break;

        default:
            throw runtime_error("Invalid Instruction value");
    }
}


////////////////////////////////////////////////////////////////////////////////
// Run
////////////////////////////////////////////////////////////////////////////////

void VM::__run()
{
    for (__IP = 0; __IP < __CS.size(); __IP++)
    {
        __execInstruction(__CS[__IP]);
    }
}


}  // End namespace CMM
