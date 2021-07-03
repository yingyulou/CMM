/*
    LexerStage.h
    ============
        Enum class LexerStage header.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Enum Class LexerStage
////////////////////////////////////////////////////////////////////////////////

enum class LexerStage
{
    // Start
    Start,

    // abc...
    //  ^^^^^
    InId,

    // 123...
    //  ^^^^^
    InNumber,

    // /?
    //  ^
    InDivide,

    // /* ...
    //    ^^^
    InComment,

    // ... */
    //      ^
    EndComment,

    // <?
    //  ^
    InLess,

    // >?
    //  ^
    InGreater,

    // =?
    //  ^
    InAssign,

    // !?
    //  ^
    InNot,

    // Done
    Done,
};


}  // End namespace CMM
