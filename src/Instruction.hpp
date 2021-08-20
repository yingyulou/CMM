/*
    Instruction.hpp
    ===============
        Enum class Instruction implementation.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Enum Class Instruction
////////////////////////////////////////////////////////////////////////////////

enum class Instruction
{
    // Load
    LDC,
    LD,
    ALD,

    // Store
    ST,
    AST,

    // Push, Pop
    PUSH,
    POP,

    // Jump
    JMP,
    JZ,

    // Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,

    // Relationship
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,

    // I/O
    IN,
    OUT,

    // Address
    ADDR,

    // Functional
    CALL,
    RET,
};


}  // End namespace CMM
