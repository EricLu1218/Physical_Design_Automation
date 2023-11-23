#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    void readInput(Input *input, const std::string &filepath);

public:
    Parser();
    Input::ptr parse(const std::string &filepath);
};
