/*
    CMM
    ===
        CMM (C Minus Minus) Language Compiler And VM.
*/

#include <string>
#include "CMM.hpp"

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using namespace CMM;


////////////////////////////////////////////////////////////////////////////////
// Main Program Define
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    string cmmFilePath, outputFilePath, instructionFilePath;

    __inputArguments(argc, argv, cmmFilePath, outputFilePath, instructionFilePath);

    if (!cmmFilePath.empty())
    {
        CodeGenerator(Parser(cmmFilePath).root(), outputFilePath).GenerateCode();
    }

    if (!instructionFilePath.empty())
    {
        Executer(instructionFilePath).Execute();
    }
}
