/*
    IO.h
    ====
        Class IO implementation.
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

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using std::string;
using std::getline;
using std::stoi;
using std::vector;
using std::pair;
using std::ifstream;
using std::runtime_error;


////////////////////////////////////////////////////////////////////////////////
// Parse Instruction File
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, int>> IO::parseInstructionFile(
    const string &instructionFilePath)
{
    return __parseInstructionFile(instructionFilePath);
}


////////////////////////////////////////////////////////////////////////////////
// Output Instruction
////////////////////////////////////////////////////////////////////////////////

void IO::outputInstruction(const string &instructionFilePath,
    const vector<pair<Instruction, string>> &codeList)
{
    __outputInstruction(instructionFilePath, codeList);
}


////////////////////////////////////////////////////////////////////////////////
// Parse Instruction File
////////////////////////////////////////////////////////////////////////////////

vector<pair<Instruction, int>> IO::__parseInstructionFile(
    const string &instructionFilePath)
{
    vector<pair<Instruction, int>> codeList;

    ifstream f(instructionFilePath);
    string line;

    while (getline(f, line))
    {
        if (line.substr(0, 4) == "LDC ")
        {
            codeList.emplace_back(Instruction::LDC, stoi(line.substr(4)));
        }
        else if (line == "LD")
        {
            codeList.emplace_back(Instruction::LD, 0);
        }
        else if (line == "ALD")
        {
            codeList.emplace_back(Instruction::ALD, 0);
        }
        else if (line == "ST")
        {
            codeList.emplace_back(Instruction::ST, 0);
        }
        else if (line == "AST")
        {
            codeList.emplace_back(Instruction::AST, 0);
        }
        else if (line == "PUSH")
        {
            codeList.emplace_back(Instruction::PUSH, 0);
        }
        else if (line == "POP")
        {
            codeList.emplace_back(Instruction::POP, 0);
        }
        else if (line.substr(0, 4) == "JMP ")
        {
            codeList.emplace_back(Instruction::JMP, stoi(line.substr(4)));
        }
        else if (line.substr(0, 3) == "JZ ")
        {
            codeList.emplace_back(Instruction::JZ, stoi(line.substr(3)));
        }
        else if (line == "ADD")
        {
            codeList.emplace_back(Instruction::ADD, 0);
        }
        else if (line == "SUB")
        {
            codeList.emplace_back(Instruction::SUB, 0);
        }
        else if (line == "MUL")
        {
            codeList.emplace_back(Instruction::MUL, 0);
        }
        else if (line == "DIV")
        {
            codeList.emplace_back(Instruction::DIV, 0);
        }
        else if (line == "LT")
        {
            codeList.emplace_back(Instruction::LT, 0);
        }
        else if (line == "LE")
        {
            codeList.emplace_back(Instruction::LE, 0);
        }
        else if (line == "GT")
        {
            codeList.emplace_back(Instruction::GT, 0);
        }
        else if (line == "GE")
        {
            codeList.emplace_back(Instruction::GE, 0);
        }
        else if (line == "EQ")
        {
            codeList.emplace_back(Instruction::EQ, 0);
        }
        else if (line == "NE")
        {
            codeList.emplace_back(Instruction::NE, 0);
        }
        else if (line == "IN")
        {
            codeList.emplace_back(Instruction::IN, 0);
        }
        else if (line == "OUT")
        {
            codeList.emplace_back(Instruction::OUT, 0);
        }
        else if (line.substr(0, 5) == "ADDR ")
        {
            codeList.emplace_back(Instruction::ADDR, stoi(line.substr(5)));
        }
        else if (line.substr(0, 5) == "CALL ")
        {
            codeList.emplace_back(Instruction::CALL, stoi(line.substr(5)));
        }
        else if (line == "RET")
        {
            codeList.emplace_back(Instruction::RET, 0);
        }
        else
        {
            throw runtime_error("Invalid instruction");
        }
    }

    return codeList;
}


////////////////////////////////////////////////////////////////////////////////
// Output Instruction
////////////////////////////////////////////////////////////////////////////////

void IO::__outputInstruction(const string &instructionFilePath,
    const vector<pair<Instruction, string>> &codeList)
{
    FILE *fo = fopen(instructionFilePath.c_str(), "w");

    for (auto &[codeEnum, codeValStr]: codeList)
    {
        switch (codeEnum)
        {
            case Instruction::LDC:
                fprintf(fo, "LDC %s\n", codeValStr.c_str());
                break;

            case Instruction::LD:
                fprintf(fo, "LD\n");
                break;

            case Instruction::ALD:
                fprintf(fo, "ALD\n");
                break;

            case Instruction::ST:
                fprintf(fo, "ST\n");
                break;

            case Instruction::AST:
                fprintf(fo, "AST\n");
                break;

            case Instruction::PUSH:
                fprintf(fo, "PUSH\n");
                break;

            case Instruction::POP:
                fprintf(fo, "POP\n");
                break;

            case Instruction::JMP:
                fprintf(fo, "JMP %s\n", codeValStr.c_str());
                break;

            case Instruction::JZ:
                fprintf(fo, "JZ %s\n", codeValStr.c_str());
                break;

            case Instruction::ADD:
                fprintf(fo, "ADD\n");
                break;

            case Instruction::SUB:
                fprintf(fo, "SUB\n");
                break;

            case Instruction::MUL:
                fprintf(fo, "MUL\n");
                break;

            case Instruction::DIV:
                fprintf(fo, "DIV\n");
                break;

            case Instruction::LT:
                fprintf(fo, "LT\n");
                break;

            case Instruction::LE:
                fprintf(fo, "LE\n");
                break;

            case Instruction::GT:
                fprintf(fo, "GT\n");
                break;

            case Instruction::GE:
                fprintf(fo, "GE\n");
                break;

            case Instruction::EQ:
                fprintf(fo, "EQ\n");
                break;

            case Instruction::NE:
                fprintf(fo, "NE\n");
                break;

            case Instruction::IN:
                fprintf(fo, "IN\n");
                break;

            case Instruction::OUT:
                fprintf(fo, "OUT\n");
                break;

            case Instruction::ADDR:
                fprintf(fo, "ADDR %s\n", codeValStr.c_str());
                break;

            case Instruction::CALL:
                fprintf(fo, "CALL %s\n", codeValStr.c_str());
                break;

            case Instruction::RET:
                fprintf(fo, "RET\n");
                break;

            default:
                throw runtime_error("Invalid instruction");
        }
    }

    fclose(fo);
}


}  // End namespace CMM
