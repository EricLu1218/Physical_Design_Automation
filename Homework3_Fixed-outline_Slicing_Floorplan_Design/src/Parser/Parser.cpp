#include "Parser.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>

std::unordered_map<std::string, Pin *> strToPin;

void Parser::readHardblock(std::string const &filename)
{
    std::ifstream fin(filename);
    std::string buff;
    while (std::getline(fin, buff))
    {
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string name;
        buffStream >> name;
        if (name == "NumHardRectilinearBlocks" ||
            name == "NumTerminals")
            continue;

        std::string identifier;
        buffStream >> identifier;
        if (identifier == "hardrectilinear")
        {
            std::string hardblockInfo;
            std::getline(buffStream, hardblockInfo);
            int x[4], y[4];
            std::sscanf(hardblockInfo.c_str(), " 4 (%d, %d) (%d, %d) (%d, %d) (%d, %d)",
                        x, y, x + 1, y + 1, x + 2, y + 2, x + 3, y + 3);
            auto width = *std::max_element(x, x + 4) - *std::min_element(x, x + 4);
            auto height = *std::max_element(y, y + 4) - *std::min_element(y, y + 4);
            hardblocks.emplace_back(new Hardblock(name, width, height));
            strToPin.emplace(name, hardblocks.back()->pin);
        }
    }
}

void Parser::readPl(std::string const &filename)
{
    std::ifstream fin(filename);
    std::string name;
    int x, y;
    while (fin >> name >> x >> y)
        strToPin.emplace(name, new Pin(name, x, y));
}

void Parser::readNet(std::string const &filename)
{
    std::ifstream fin(filename);
    std::string identifier;
    while (fin >> identifier)
    {
        if (identifier == "NetDegree")
        {
            nets.emplace_back(new Net());
            std::string temp;
            size_t pinNum;
            fin >> temp >> pinNum;
            for (size_t i = 0; i < pinNum; ++i)
            {
                std::string name;
                fin >> name;
                nets.back()->pins.emplace_back(strToPin.at(name));
            }
        }
    }
}

SAInput *Parser::parse(char *argv[])
{
    readHardblock(argv[1]);
    readPl(argv[3]);
    readNet(argv[2]);
    return new SAInput(hardblocks, nets, std::stod(argv[5]));
}
