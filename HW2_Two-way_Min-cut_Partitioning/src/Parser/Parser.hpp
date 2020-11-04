#pragma once
#include "../Structure/Data.hpp"
#include <fstream>
#include <string>
#include <vector>

class Parser
{
    int64_t totalSize;
    std::vector<Cell *> cells;
    std::vector<Net *> nets;

    void readCell(std::string filename);
    void readNet(std::string filename);

public:
    Parser() : totalSize(0) {}
    FMInput *parse(char *argv[]);
};