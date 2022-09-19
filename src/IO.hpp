/*
    IO.h
    ====
        Class __IO implementation.
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include "IO.h"
#include "Instruction.hpp"

namespace CMM
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::getline;
using std::stoi;
using std::vector;
using std::pair;
using std::ifstream;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parse __Instruction File
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<__Instruction, int>> __IO::__parseInstructionFile(const string &instructionFilePath)
{
    vector<pair<__Instruction, int>> codeList;

    ifstream f(instructionFilePath);
    string line;

    while (getline(f, line))
    {
        if (line.substr(0, 4) == "LDC ")
        {
            codeList.emplace_back(__Instruction::__LDC, stoi(line.substr(4)));
        }
        else if (line == "LD")
        {
            codeList.emplace_back(__Instruction::__LD, 0);
        }
        else if (line == "ALD")
        {
            codeList.emplace_back(__Instruction::__ALD, 0);
        }
        else if (line == "ST")
        {
            codeList.emplace_back(__Instruction::__ST, 0);
        }
        else if (line == "__AST")
        {
            codeList.emplace_back(__Instruction::__AST, 0);
        }
        else if (line == "PUSH")
        {
            codeList.emplace_back(__Instruction::__PUSH, 0);
        }
        else if (line == "POP")
        {
            codeList.emplace_back(__Instruction::__POP, 0);
        }
        else if (line.substr(0, 4) == "JMP ")
        {
            codeList.emplace_back(__Instruction::__JMP, stoi(line.substr(4)));
        }
        else if (line.substr(0, 3) == "JZ ")
        {
            codeList.emplace_back(__Instruction::__JZ, stoi(line.substr(3)));
        }
        else if (line == "ADD")
        {
            codeList.emplace_back(__Instruction::__ADD, 0);
        }
        else if (line == "SUB")
        {
            codeList.emplace_back(__Instruction::__SUB, 0);
        }
        else if (line == "MUL")
        {
            codeList.emplace_back(__Instruction::__MUL, 0);
        }
        else if (line == "DIV")
        {
            codeList.emplace_back(__Instruction::__DIV, 0);
        }
        else if (line == "LT")
        {
            codeList.emplace_back(__Instruction::__LT, 0);
        }
        else if (line == "LE")
        {
            codeList.emplace_back(__Instruction::__LE, 0);
        }
        else if (line == "GT")
        {
            codeList.emplace_back(__Instruction::__GT, 0);
        }
        else if (line == "GE")
        {
            codeList.emplace_back(__Instruction::__GE, 0);
        }
        else if (line == "EQ")
        {
            codeList.emplace_back(__Instruction::__EQ, 0);
        }
        else if (line == "NE")
        {
            codeList.emplace_back(__Instruction::__NE, 0);
        }
        else if (line == "IN")
        {
            codeList.emplace_back(__Instruction::__IN, 0);
        }
        else if (line == "OUT")
        {
            codeList.emplace_back(__Instruction::__OUT, 0);
        }
        else if (line.substr(0, 5) == "ADDR ")
        {
            codeList.emplace_back(__Instruction::__ADDR, stoi(line.substr(5)));
        }
        else if (line.substr(0, 5) == "CALL ")
        {
            codeList.emplace_back(__Instruction::__CALL, stoi(line.substr(5)));
        }
        else if (line == "RET")
        {
            codeList.emplace_back(__Instruction::__RET, 0);
        }
        else
        {
            throw runtime_error("Invalid instruction");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output __Instruction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __IO::__outputInstruction(const string &instructionFilePath, const vector<pair<__Instruction, string>> &codeList)
{
    FILE *fo = fopen(instructionFilePath.c_str(), "w");

    for (auto &[codeEnum, codeValStr]: codeList)
    {
        switch (codeEnum)
        {
            case __Instruction::__LDC:
                fprintf(fo, "LDC %s\n", codeValStr.c_str());
                break;

            case __Instruction::__LD:
                fprintf(fo, "LD\n");
                break;

            case __Instruction::__ALD:
                fprintf(fo, "ALD\n");
                break;

            case __Instruction::__ST:
                fprintf(fo, "ST\n");
                break;

            case __Instruction::__AST:
                fprintf(fo, "__AST\n");
                break;

            case __Instruction::__PUSH:
                fprintf(fo, "PUSH\n");
                break;

            case __Instruction::__POP:
                fprintf(fo, "POP\n");
                break;

            case __Instruction::__JMP:
                fprintf(fo, "JMP %s\n", codeValStr.c_str());
                break;

            case __Instruction::__JZ:
                fprintf(fo, "JZ %s\n", codeValStr.c_str());
                break;

            case __Instruction::__ADD:
                fprintf(fo, "ADD\n");
                break;

            case __Instruction::__SUB:
                fprintf(fo, "SUB\n");
                break;

            case __Instruction::__MUL:
                fprintf(fo, "MUL\n");
                break;

            case __Instruction::__DIV:
                fprintf(fo, "DIV\n");
                break;

            case __Instruction::__LT:
                fprintf(fo, "LT\n");
                break;

            case __Instruction::__LE:
                fprintf(fo, "LE\n");
                break;

            case __Instruction::__GT:
                fprintf(fo, "GT\n");
                break;

            case __Instruction::__GE:
                fprintf(fo, "GE\n");
                break;

            case __Instruction::__EQ:
                fprintf(fo, "EQ\n");
                break;

            case __Instruction::__NE:
                fprintf(fo, "NE\n");
                break;

            case __Instruction::__IN:
                fprintf(fo, "IN\n");
                break;

            case __Instruction::__OUT:
                fprintf(fo, "OUT\n");
                break;

            case __Instruction::__ADDR:
                fprintf(fo, "ADDR %s\n", codeValStr.c_str());
                break;

            case __Instruction::__CALL:
                fprintf(fo, "CALL %s\n", codeValStr.c_str());
                break;

            case __Instruction::__RET:
                fprintf(fo, "RET\n");
                break;

            default:
                throw runtime_error("Invalid instruction");
        }
    }

    fclose(fo);
}


}  // End namespace CMM
