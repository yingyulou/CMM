#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include "Compiler.hpp"
#include "VM.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::cout;
using std::endl;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __Kernel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __Kernel
{
public:

    // Constructor
    explicit __Kernel(int argc, char **argv):
        __Argc(argc),
        __Argv(argv) {}


    // operator()
    void operator()()
    {
        __main();
    }


private:

    // Data
    int __Argc;
    char **__Argv;
    string __inputFilePath;
    string __outputFilePath;
    string __asmFilePath;


    // Construct Argument
    void __constructArgument()
    {
        namespace po = boost::program_options;

        po::options_description desc;

        desc.add_options()

            ("help,h", "Show this help message and exit")

            ("input-file-path,", po::value<string>(&__inputFilePath),
                "Input cmm file path")

            ("output-file-path,", po::value<string>(&__outputFilePath),
                "Output asm file path")

            ("asm-file-path,", po::value<string>(&__asmFilePath),
                "Input asm file path for running");

        po::variables_map vm;
        po::store(po::parse_command_line(__Argc, __Argv, desc), vm);

        if (vm.count("help"))
        {
            cout << desc << endl;
            exit(1);
        }

        po::notify(vm);
    }


    // Main
    void __main()
    {
        __constructArgument();
        __Compiler(__inputFilePath, __outputFilePath)();
        (__VM(__asmFilePath))();
    }
};


}  // End namespace CMM
