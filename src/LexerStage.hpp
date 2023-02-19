/*
    LexerStage.hpp
    ==============
        Enum class __LexerStage implementation.
*/

#pragma once

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// Enum Class LexerStage
=======
// Enum Class __LexerStage
>>>>>>> 0c0e907012a412af040951cada6b8da33e61e29a
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
