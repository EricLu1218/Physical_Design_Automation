#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    std::string nodeFilename, plFilename, sclFilename;

    void readAux(Input *input, const std::string &filepath);
    void readNode(Input *input, const std::string &filepath);
    void readPl(Input *input, const std::string &filepath);
    void readScl(Input *input, const std::string &filepath);

public:
    Parser();
    Input::ptr parse(const std::string &filepath);
};
