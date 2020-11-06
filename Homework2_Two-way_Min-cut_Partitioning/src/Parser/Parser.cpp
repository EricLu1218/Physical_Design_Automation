#include "Parser.hpp"
#include <unordered_map>

std::unordered_map<std::string, Cell *> strToCell;

void Parser::readCell(std::string filename)
{
    std::ifstream fin(filename);
    std::string name;
    int64_t size;
    while (fin >> name >> size)
    {
        auto cell = new Cell(name, size);
        cells.emplace_back(cell);
        strToCell.emplace(name, cell);
        totalSize += size;
    }
}

void Parser::readNet(std::string filename)
{
    std::ifstream fin(filename);
    std::string name, temp;
    while (fin >> temp >> name >> temp)
    {
        auto net = new Net(name);
        nets.emplace_back(net);
        while (fin >> name && name[0] != '}')
        {
            auto cell = strToCell.at(name);
            net->cells.push_back(cell);
            cell->nets.push_back(net);
        }
    }
}

FMInput *Parser::parse(char *argv[])
{
    readCell(argv[2]);
    readNet(argv[1]);
    return new FMInput(totalSize / 10.0, cells, nets);
}