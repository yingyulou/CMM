/*
    LexerStage.hpp
    ==============
        Enum class LEXER_STAGE implementation.
*/

#ifndef __CMM_LEXER_STAGE_HPP
#define __CMM_LEXER_STAGE_HPP

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Enum Class LEXER_STAGE
////////////////////////////////////////////////////////////////////////////////

enum class LEXER_STAGE
{
    // Start
    START,

    // abc...
    //  ^^^^^
    IN_ID,

    // 123...
    //  ^^^^^
    IN_NUMBER,

    // /?
    //  ^
    IN_DIVIDE,

    // /* ...
    //    ^^^
    IN_COMMENT,

    // ... */
    //      ^
    END_COMMENT,

    // <?
    //  ^
    IN_LESS,

    // >?
    //  ^
    IN_GREATER,

    // =?
    //  ^
    IN_ASSIGN,

    // !?
    //  ^
    IN_NOT,

    // Done
    DONE,
};


}  // End namespace CMM


#endif  // __CMM_LEXER_STAGE_HPP
