#include "Parser.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

inline std::string strip(std::string input, std::string chars = " \t\r\n")
{
    input.erase(0, input.find_first_not_of(chars));
    input.erase(input.find_last_not_of(chars) + 1);
    return input;
}

std::unordered_map<std::string, Pin *> strToPin;

void Parser::readHardblock(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::string buff;
    while (std::getline(fin, buff))
    {
        buff = strip(buff);
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string name, identifier;
        buffStream >> name >> identifier;
        if (identifier == "hardrectilinear")
        {
            std::string hardblockInfo;
            std::getline(buffStream, hardblockInfo);
            int x[4] = {0}, y[4] = {0};
            std::sscanf(hardblockInfo.c_str(), " 4 (%d, %d) (%d, %d) (%d, %d) (%d, %d)",
                        x, y, x + 1, y + 1, x + 2, y + 2, x + 3, y + 3);
            auto width = *std::max_element(x, x + 4) - *std::min_element(x, x + 4);
            auto height = *std::max_element(y, y + 4) - *std::min_element(y, y + 4);

            auto hardblock = new Hardblock(name, width, height);
            input->hardblocks.emplace_back(hardblock);
            strToPin.emplace(name, hardblock->pin.get());
        }
    }
}

void Parser::readPl(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::string name;
    int x = 0, y = 0;
    while (fin >> name >> x >> y)
    {
        auto pin = new Pin(name, x, y);
        input->fixedPins.emplace_back(pin);
        strToPin.emplace(name, pin);
    }
}

void Parser::readNet(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::string identifier;
    while (fin >> identifier)
    {
        if (identifier == "NetDegree")
        {
            auto net = new Net();
            input->nets.emplace_back(net);

            std::string _, name;
            size_t pinNum = 0;
            fin >> _ >> pinNum;
            for (size_t i = 0; i < pinNum; ++i)
            {
                fin >> name;
                net->pins.emplace_back(strToPin[name]);
            }
        }
    }
}

Parser::Parser() {}

Input::ptr Parser::parse(const std::string &hardblockFilepath, const std::string &plFilepath,
                         const std::string &netFilepath, double deadspaceRatio)
{
    auto input = new Input();
    readHardblock(input, hardblockFilepath);
    readPl(input, plFilepath);
    readNet(input, netFilepath);
    input->deadspaceRatio = deadspaceRatio;
    return std::unique_ptr<Input>(input);
}
