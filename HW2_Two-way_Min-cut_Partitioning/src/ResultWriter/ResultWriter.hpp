#pragma once
#include "../Structure/Data.hpp"
#include <string>

class ResultWriter
{
    FMInput &input;

    size_t cutSize;

public:
    ResultWriter(FMInput *input, size_t cutSize) : input(*input), cutSize(cutSize) {}
    void write(std::string filename);
};