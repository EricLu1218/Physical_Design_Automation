#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, Cell *> strToCell;

void Parser::readCell(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::string name;
    int size = 0;
    while (fin >> name >> size)
    {
        auto cell = new Cell(name, size);
        input->cells.emplace_back(cell);
        strToCell.emplace(name, cell);
    };
}

void Parser::readNet(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::string name, _;
    while (fin >> _ >> name >> _)
    {
        auto net = new Net(name);
        input->nets.emplace_back(net);
        while (fin >> name && name[0] != '}')
        {
            auto cell = strToCell[name];
            net->cells.emplace_back(cell);
            cell->nets.emplace_back(net);
        }
    }
}

Parser::Parser() {}

Input::ptr Parser::parse(const std::string &cellFilepath, const std::string &netFilepath)
{
    auto input = new Input();
    readCell(input, cellFilepath);
    readNet(input, netFilepath);
    int totalSize = 0;
    for (const auto &cell : input->cells)
        totalSize += cell->size;
    input->maxDiffSize = static_cast<double>(totalSize) / 10;
    return std::unique_ptr<Input>(input);
}
