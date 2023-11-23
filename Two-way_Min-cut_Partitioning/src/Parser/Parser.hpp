#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    void readCell(Input *input, const std::string &filepath);
    void readNet(Input *input, const std::string &filepath);

public:
    Parser();
    Input::ptr parse(const std::string &cellFilepath, const std::string &netFilepath);
};
