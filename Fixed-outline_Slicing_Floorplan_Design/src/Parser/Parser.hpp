#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    void readHardblock(Input *input, const std::string &filename);
    void readPl(Input *input, const std::string &filename);
    void readNet(Input *input, const std::string &filename);

public:
    Parser();
    Input::ptr parse(const std::string &hardblockFile, const std::string &plFile,
                     const std::string &netFile, double deadspaceRatio);
};
