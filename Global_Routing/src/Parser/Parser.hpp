#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    void readInput(Input *input, const std::string &filename);

public:
    Parser();
    Input::ptr parse(const std::string &filename);
};
