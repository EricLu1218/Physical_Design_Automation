#pragma once
#include "../Structure/Data.hpp"
#include <string>

class ResultWriter
{
    RouterInput *input;

public:
    ResultWriter(RouterInput *input) : input(input) {}
    void write(std::string const &filename);
};
