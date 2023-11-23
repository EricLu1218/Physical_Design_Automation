#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    void readHardblock(Input *input, const std::string &filepath);
    void readPl(Input *input, const std::string &filepath);
    void readNet(Input *input, const std::string &filepath);

public:
    Parser();
    Input::ptr parse(const std::string &hardblockFilepath, const std::string &plFilepath,
                     const std::string &netFilepath, double deadspaceRatio);
};
