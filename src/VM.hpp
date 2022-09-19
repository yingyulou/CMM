/*
    VM.hpp
    ======
        Class __VM implementation.
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::pair;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__VM::__VM(const vector<pair<__Instruction, int>> &CS):
    __CS(CS),
    __IP(0) {}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exec __Instruction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __VM::__execInstruction(const pair<__Instruction, int> &instructionPair)
{
    switch (instructionPair.first)
    {
        case __Instruction::__LDC:
            __AX = instructionPair.second;
            break;

        case __Instruction::__LD:
            __AX = __SS[__BP - __AX];
            break;

        case __Instruction::__ALD:
            __AX = __SS[__AX];
            break;

        case __Instruction::__ST:
            __SS[__BP - __AX] = __SS.back();
            break;

        case __Instruction::__AST:
            __SS[__AX] = __SS.back();
            break;

        case __Instruction::__PUSH:
            __SS.push_back(__AX);
            break;

        case __Instruction::__POP:
            __SS.pop_back();
            break;

        case __Instruction::__JMP:
            __IP += instructionPair.second - 1;
            break;

        case __Instruction::__JZ:

            if (!__AX)
            {
                __IP += instructionPair.second - 1;
            }

            break;

        case __Instruction::__ADD:
            __AX = __SS.back() + __AX;
            break;

        case __Instruction::__SUB:
            __AX = __SS.back() - __AX;
            break;

        case __Instruction::__MUL:
            __AX = __SS.back() * __AX;
            break;

        case __Instruction::__DIV:
            __AX = __SS.back() / __AX;
            break;

        case __Instruction::__LT:
            __AX = __SS.back() < __AX;
            break;

        case __Instruction::__LE:
            __AX = __SS.back() <= __AX;
            break;

        case __Instruction::__GT:
            __AX = __SS.back() > __AX;
            break;

        case __Instruction::__GE:
            __AX = __SS.back() >= __AX;
            break;

        case __Instruction::__EQ:
            __AX = __SS.back() == __AX;
            break;

        case __Instruction::__NE:
            __AX = __SS.back() != __AX;
            break;

        case __Instruction::__IN:
            scanf("%d", &__AX);
            break;

        case __Instruction::__OUT:
            printf("%d\n", __AX);
            break;

        case __Instruction::__ADDR:
            __AX = (int)__SS.size() - instructionPair.second;
            break;

        case __Instruction::__CALL:
            __SS.push_back(__BP);
            __BP = (int)__SS.size() - 2;
            __SS.push_back(__IP);
            __IP += instructionPair.second - 1;
            break;

        case __Instruction::__RET:
            __IP = __SS.back();
            __SS.pop_back();
            __BP = __SS.back();
            __SS.pop_back();
            break;

        default:
            throw runtime_error("Invalid __Instruction value");
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Run
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __VM::__run()
{
    for (__IP = 0; __IP < (int)__CS.size(); __IP++)
    {
        __execInstruction(__CS[__IP]);
    }
}


}  // End namespace CMM
