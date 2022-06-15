#pragma once
#include "../Structure/Data.hpp"
#include <string>
#include <vector>

class Parser
{
    int hGridCnt, vGridCnt, hCapacity, vCapacity;
    std::vector<Net *> nets;

    void readModifiedTxt(std::string const &filename);

public:
    Parser() : hGridCnt(0), vGridCnt(0),
               hCapacity(0), vCapacity(0) {}
    RouterInput *parse(char *argv[]);
};
