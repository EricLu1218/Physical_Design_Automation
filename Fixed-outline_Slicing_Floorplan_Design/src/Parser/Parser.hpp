#pragma once
#include "../Structure/Data.hpp"
#include <string>
#include <vector>

class Parser
{
    std::vector<Hardblock *> hardblocks;
    std::vector<Net *> nets;

    void readHardblock(std::string const &filename);
    void readPl(std::string const &filename);
    void readNet(std::string const &filename);

public:
    Parser() {}
    SAInput *parse(char *argv[]);
};
