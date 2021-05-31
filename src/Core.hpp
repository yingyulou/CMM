/*
    Core.hpp
    ========
        Class Core implementation.
*/

#ifndef __CMM_CORE_HPP
#define __CMM_CORE_HPP

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include "Core.h"
#include "CodeGenerator.h"
#include "Executor.h"
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
    __runCodeGenerator();
    __runExecuter();
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

        (",o", po::value<string>(&__outputFilePath)->default_value("a.out"),
            "Output Instruction File Path")

        (",r", po::value<string>(&__instructionFilePath),
            "Input Instruction File Path (For Running)");

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
// Run Code Generator
////////////////////////////////////////////////////////////////////////////////

void Core::__runCodeGenerator() const
{
    if (!__cmmFilePath.empty())
    {
        CodeGenerator(Parser(__cmmFilePath).root(), __outputFilePath).GenerateCode();
    }
}


////////////////////////////////////////////////////////////////////////////////
// Run Executer
////////////////////////////////////////////////////////////////////////////////

void Core::__runExecuter() const
{
    if (!__instructionFilePath.empty())
    {
        Executer(__instructionFilePath).Execute();
    }
}


}  // End namespace CMM


#endif  // __CMM_CORE_HPP
