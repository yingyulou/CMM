/*
    VM.hpp
    ======
        Class __VM implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::vector;
using std::ifstream;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class __VM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class __VM
{
public:

    // Constructor
    explicit __VM(const string &inputFilePath):
        __inputFilePath(inputFilePath) {}


    // operator()
    void operator()()
    {
        __main();
    }


private:

    // Attribute
    string __inputFilePath;
    vector<string> __CS;
    size_t __IP;
    vector<int32_t> __SS;
    int32_t __AX;
    int32_t __BP;


    // Construct __CS
    void __constructCS()
    {
        ifstream fdIn(__inputFilePath);

        if (!fdIn)
        {
            throw runtime_error("Invalid " + __inputFilePath);
        }

        for (string line; getline(fdIn, line); __CS.push_back(line));
    }


    // Exec Code
    void __execCode()
    {
        for (__IP = 0; __IP < __CS.size(); __IP++)
        {
            if (!__CS[__IP].compare(0, 4, "ldc "))
            {
                __AX = stoll(__CS[__IP].substr(4));
            }
            else if (__CS[__IP] == "ld")
            {
                __AX = __SS[__BP - __AX];
            }
            else if (__CS[__IP] == "ald")
            {
                __AX = __SS[__AX];
            }
            else if (__CS[__IP] == "st")
            {
                __SS[__BP - __AX] = __SS.back();
            }
            else if (__CS[__IP] == "ast")
            {
                __SS[__AX] = __SS.back();
            }
            else if (__CS[__IP] == "push")
            {
                __SS.push_back(__AX);
            }
            else if (__CS[__IP] == "pop")
            {
                __SS.pop_back();
            }
            else if (!__CS[__IP].compare(0, 4, "jmp "))
            {
                __IP += stoll(__CS[__IP].substr(4)) - 1;
            }
            else if (!__CS[__IP].compare(0, 3, "jz "))
            {
                if (!__AX)
                {
                    __IP += stoll(__CS[__IP].substr(3)) - 1;
                }
            }
            else if (__CS[__IP] == "add")
            {
                __AX = __SS.back() + __AX;
            }
            else if (__CS[__IP] == "sub")
            {
                __AX = __SS.back() - __AX;
            }
            else if (__CS[__IP] == "mul")
            {
                __AX = __SS.back() * __AX;
            }
            else if (__CS[__IP] == "div")
            {
                __AX = __SS.back() / __AX;
            }
            else if (__CS[__IP] == "lt")
            {
                __AX = __SS.back() < __AX;
            }
            else if (__CS[__IP] == "le")
            {
                __AX = __SS.back() <= __AX;
            }
            else if (__CS[__IP] == "gt")
            {
                __AX = __SS.back() > __AX;
            }
            else if (__CS[__IP] == "ge")
            {
                __AX = __SS.back() >= __AX;
            }
            else if (__CS[__IP] == "eq")
            {
                __AX = __SS.back() == __AX;
            }
            else if (__CS[__IP] == "ne")
            {
                __AX = __SS.back() != __AX;
            }
            else if (__CS[__IP] == "in")
            {
                scanf("%d", &__AX);
            }
            else if (__CS[__IP] == "out")
            {
                printf("%d\n", __AX);
            }
            else if (!__CS[__IP].compare(0, 5, "addr "))
            {
                __AX = __SS.size() - stoll(__CS[__IP].substr(5));
            }
            else if (!__CS[__IP].compare(0, 5, "call "))
            {
                __SS.push_back(__BP);
                __BP = __SS.size() - 2;
                __SS.push_back(__IP);
                __IP += stoll(__CS[__IP].substr(5)) - 1;
            }
            else if (__CS[__IP] == "ret")
            {
                __IP = __SS.back();
                __SS.pop_back();
                __BP = __SS.back();
                __SS.pop_back();
            }
            else
            {
                throw runtime_error("Invalid instruction");
            }
        }
    }


    // Main
    void __main()
    {
        if (__inputFilePath.empty())
        {
            return;
        }

        __constructCS();
        __execCode();
    }
};


}  // End namespace CMM
