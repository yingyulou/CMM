/*
    LexerStage.hpp
    ==============
        Enum class __LexerStage implementation.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum Class __LexerStage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class __LexerStage
{
    // __Start
    __Start,

    // abc...
    //  ^^^^^
    __InId,

    // 123...
    //  ^^^^^
    __InNumber,

    // /?
    //  ^
    __InDivide,

    // /* ...
    //    ^^^
    __InComment,

    // ... */
    //      ^
    __EndComment,

    // <?
    //  ^
    __InLess,

    // >?
    //  ^
    __InGreater,

    // =?
    //  ^
    __InAssign,

    // !?
    //  ^
    __InNot,

    // Done
    __Done,
};


}  // End namespace CMM
