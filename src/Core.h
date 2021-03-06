/*
    Core.h
    ======
        Class Core header.
*/

#pragma once

#include <string>

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;


////////////////////////////////////////////////////////////////////////////////
// Class Core
////////////////////////////////////////////////////////////////////////////////

class Core
{
public:

    // Constructor
    explicit Core(int argc, char **argv);


    // Main
    void main();


private:

    // Attribute
    int __ARGC;
    char **__ARGV;
    string __cmmFilePath;
    string __outputASMCodeFilePath;
    string __outputByteCodeFilePath;
    string __inputByteCodeFilePath;


    // Input Arguments
    void __inputArguments();


    // Generate Code
    void __generateCode() const;


    // Exec Code
    void __execCode() const;
};


}  // End namespace CMM
