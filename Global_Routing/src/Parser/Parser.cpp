#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

void Parser::readInput(Input *input, const std::string &filepath)
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
        std::stringstream buffStream(buff);
        std::string identifier, _;
        buffStream >> identifier;

        if (identifier == "grid")
        {
            buffStream >> input->hGridCnt >> input->vGridCnt;
        }
        else if (identifier == "horizontal")
        {
            buffStream >> _ >> input->hCapacity;
        }
        else if (identifier == "vertical")
        {
            buffStream >> _ >> input->vCapacity;
        }
        else if (identifier.substr(0, 3) == "net")
        {
            int id = 0;
            buffStream >> id;

            int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
            fin >> x1 >> y1 >> x2 >> y2;
            input->nets.emplace_back(new Net(identifier, id, x1, y1, x2, y2));
        }
    }
}

Parser::Parser() {}

Input::ptr Parser::parse(const std::string &filepath)
{
    Input *input = new Input();
    readInput(input, filepath);
    return std::unique_ptr<Input>(input);
}
