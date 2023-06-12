#pragma once
#include "../Structure/Data.hpp"
#include <string>
#include <vector>

class Parser
{
    void readCell(Input *input, const std::string &filename);
    void readNet(Input *input, const std::string &filename);

public:
    Parser();
    Input::ptr parse(const std::string &cellFile, const std::string &netFile);
};
