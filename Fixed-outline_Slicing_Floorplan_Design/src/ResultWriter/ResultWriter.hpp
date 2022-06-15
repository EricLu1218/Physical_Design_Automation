#pragma once
#include "../Structure/Data.hpp"
#include <string>

class ResultWriter
{
    SAInput *input;

public:
    ResultWriter(SAInput *input) : input(input) {}
    void write(std::string const &filename);
};
