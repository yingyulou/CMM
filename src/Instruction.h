/*
    Instruction.h
    =============
        Enum class Instruction header.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Enum Class Instruction
////////////////////////////////////////////////////////////////////////////////

enum class Instruction: char
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

    // Call (Dummy Instruction)
    CALL,

    // Stop
    STOP,
};


}  // End namespace CMM
