#pragma once
#include "../Structure/Data.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Parser
{
    std::string nodeFile, plFile, sclFile;
    int maxDisplacement;
    std::vector<Cell *> cells, terminals;
    std::vector<Row *> rows;

    void readAux(std::string const &filename);
    void readNode(std::string const &filename);
    void readPl(std::string const &filename);
    void readScl(std::string const &filename);

public:
    Parser() : maxDisplacement(0) {}
    LegalizerInput *parse(char argv[]);
};
