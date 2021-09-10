/*
    Core.hpp
    ========
        Class Core implementation.
*/

#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include "Core.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "CodeGenerator.h"
#include "IO.h"
#include "VM.h"
#include "Constants.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::cout;
using std::endl;


////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

Core::Core(int argc, char **argv):
    __ARGC(argc),
    __ARGV(argv) {}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

void Core::main()
{
    __main();
}


////////////////////////////////////////////////////////////////////////////////
// Input Arguments
////////////////////////////////////////////////////////////////////////////////

void Core::__inputArguments()
{
    namespace po = boost::program_options;

    po::options_description desc(DESCRIPTION_STR);

    desc.add_options()

        ("help,h", "show this help message and exit")

        (",i", po::value<string>(&__cmmFilePath),
            "Input CMM File Path")

        (",o", po::value<string>(&__outputFilePath)->default_value("a.out"),
            "Output ASM File Path")

        (",r", po::value<string>(&__asmFilePath),
            "Input ASM File Path (For Running)");

    po::variables_map vm;
    po::store(po::parse_command_line(__ARGC, __ARGV, desc), vm);

    if (vm.count("help"))
    {
        cout << desc << endl;
        exit(1);
    }

    po::notify(vm);
}


////////////////////////////////////////////////////////////////////////////////
// Generate Code
////////////////////////////////////////////////////////////////////////////////

void Core::__generateCode() const
{
    if (!__cmmFilePath.empty())
    {
        LexicalAnalyzer lexicalAnalyzer(__cmmFilePath);

        auto tokenList = lexicalAnalyzer.lexicalAnalysis();

        SyntaxAnalyzer syntaxAnalyzer(tokenList);

        auto root = syntaxAnalyzer.syntaxAnalysis();

        SemanticAnalyzer semanticAnalyzer(root);

        auto symbolTable = semanticAnalyzer.semanticAnalysis();

        CodeGenerator codeGenerator(root, symbolTable);

        auto codeList = codeGenerator.generateCode();

        IO::outputInstruction(__outputFilePath, codeList);

        delete root;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Exec Code
////////////////////////////////////////////////////////////////////////////////

void Core::__execCode() const
{
    if (!__asmFilePath.empty())
    {
        auto codeList = IO::parseInstructionFile(__asmFilePath);

        VM vm(codeList);

        vm.run();
    }
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

void Core::__main()
{
    __inputArguments();
    __generateCode();
    __execCode();
}


}  // End namespace CMM
