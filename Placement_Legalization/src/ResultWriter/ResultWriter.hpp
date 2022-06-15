#pragma once
#include "../Structure/Data.hpp"

class ResultWriter
{
    LegalizerInput *input;

public:
    ResultWriter(LegalizerInput *input) : input(input) {}
    void write(char *argv[]);
};
