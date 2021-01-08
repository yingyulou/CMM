/*
    Instruction.hpp
    ===============
        Enum class INSTRUCTION implementation.
*/

#ifndef __CMM_INSTRUCTION_HPP
#define __CMM_INSTRUCTION_HPP

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


#endif  // __CMM_INSTRUCTION_HPP
