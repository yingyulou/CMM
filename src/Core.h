/*
    Core.h
    ======
        Class Core header.
*/

#ifndef __CMM_CORE_H
#define __CMM_CORE_H

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
    string __outputFilePath;
    string __instructionFilePath;


    // Input Arguments
    void __inputArguments();


    // Run Code Generator
    void __runCodeGenerator() const;


    // Run Executer
    void __runExecuter() const;
};


}  // End namespace CMM


#endif  // __CMM_CORE_H
