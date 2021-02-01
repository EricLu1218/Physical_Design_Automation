#pragma once
#include "../Structure/Data.hpp"
#include <string>
#include <vector>

class Parser
{
    int totalSize;
    std::vector<Cell *> cells;
    std::vector<Net *> nets;

    void readCell(std::string const &filename);
    void readNet(std::string const &filename);

public:
    Parser() : totalSize(0) {}
    FMInput *parse(char *argv[]);
};
