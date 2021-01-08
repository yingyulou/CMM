/*
    ArgumentParser.hpp
    ==================
        Command line argument parser functions implementation.
*/

#ifndef __CMM_ARGUMENT_PARSER_HPP
#define __CMM_ARGUMENT_PARSER_HPP

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "Constants.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::cout;
using std::endl;
using std::string;


////////////////////////////////////////////////////////////////////////////////
// Input Arguments
////////////////////////////////////////////////////////////////////////////////

void __inputArguments(int argc, char **argv, string &cmmFilePath,
    string &outputFilePath, string &instructionFilePath)
{
    namespace po = boost::program_options;

    po::options_description desc(DESCRIPTION_STR);

    desc.add_options()

        ("help,h", "show this help message and exit")

        (",c", po::value<string>(&cmmFilePath),
            "Input CMM File Path")

        (",o", po::value<string>(&outputFilePath)->default_value("a.out"),
            "Output Instruction File Path")

        (",r", po::value<string>(&instructionFilePath),
            "Input Instruction File Path (For Running)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
    {
        cout << desc << endl;
        exit(1);
    }

    po::notify(vm);
}


}  // End namespace CMM


#endif  // __CMM_ARGUMENT_PARSER_HPP
