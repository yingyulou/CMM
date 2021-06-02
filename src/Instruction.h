/*
    Instruction.h
    =============
        Enum class INSTRUCTION header.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Enum Class INSTRUCTION
////////////////////////////////////////////////////////////////////////////////

enum class INSTRUCTION
{
    // Load
    LDC,
    LD,
    ABSLD,

    // Save
    SAV,
    ABSSAV,
    SAVSP,

    // Push, Pop
    PUSH,
    POP,
    PUSHBP,
    POPBP,
    PUSHIP,
    POPIP,
    PUSHSP,

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
    INPUT,
    OUTPUT,

    // Stop
    STOP,
};


}  // End namespace CMM
