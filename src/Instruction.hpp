/*
    Instruction.hpp
    ===============
        Enum class __Instruction implementation.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum Class __Instruction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class __Instruction
{
    // Load
    __LDC,
    __LD,
    __ALD,

    // Store
    __ST,
    __AST,

    // Push, Pop
    __PUSH,
    __POP,

    // Jump
    __JMP,
    __JZ,

    // Arithmetic
    __ADD,
    __SUB,
    __MUL,
    __DIV,

    // Relationship
    __LT,
    __LE,
    __GT,
    __GE,
    __EQ,
    __NE,

    // I/O
    __IN,
    __OUT,

    // Address
    __ADDR,

    // Functional
    __CALL,
    __RET,
};


}  // End namespace CMM
