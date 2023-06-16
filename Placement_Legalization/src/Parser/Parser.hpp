#pragma once
#include "../Structure/Data.hpp"
#include <string>

class Parser
{
    std::string nodeFile, plFile, sclFile;

    void readAux(Input *input, const std::string &filename);
    void readNode(Input *input, const std::string &filename);
    void readPl(Input *input, const std::string &filename);
    void readScl(Input *input, const std::string &filename);

public:
    Parser();
    Input::ptr parse(const std::string &filename);
};
