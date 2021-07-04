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
#include "CodeGenerator.h"
#include "Decoder.h"
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
    __inputArguments();
    __generateCode();
    __execCode();
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

        (",c", po::value<string>(&__cmmFilePath),
            "Input CMM File Path")

        (",s", po::value<string>(&__outputASMCodeFilePath),
            "Output ASM File Path")

        (",o", po::value<string>(&__outputByteCodeFilePath)->default_value("a.out"),
            "Output Byte Code File Path")

        (",r", po::value<string>(&__inputByteCodeFilePath),
            "Input Byte Code File Path (For Running)");

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
        CodeGenerator(Parser(__cmmFilePath).root(), __outputASMCodeFilePath,
            __outputByteCodeFilePath).GenerateCode();
    }
}


////////////////////////////////////////////////////////////////////////////////
// Exec Code
////////////////////////////////////////////////////////////////////////////////

void Core::__execCode() const
{
    if (!__inputByteCodeFilePath.empty())
    {
        VM(Decoder(__inputByteCodeFilePath).instructionList()).Start();
    }
}


}  // End namespace CMM
