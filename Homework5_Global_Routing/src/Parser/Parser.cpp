#include "Parser.hpp"
#include <fstream>
#include <sstream>

void Parser::readModifiedTxt(std::string const &filename)
{
    std::ifstream fin(filename);
    std::string buff;
    while (std::getline(fin, buff))
    {
        std::stringstream buffStream(buff);
        std::string identifier;
        buffStream >> identifier;

        if (identifier == "grid")
        {
            buffStream >> this->hGridCnt >> this->vGridCnt;
        }
        else if (identifier == "vertical")
        {
            std::string temp;
            buffStream >> temp >> this->vCapacity;
        }
        else if (identifier == "horizontal")
        {
            std::string temp;
            buffStream >> temp >> this->hCapacity;
        }
        else if (identifier.substr(0, 3) == "net")
        {
            int id = 0;
            buffStream >> id;

            std::getline(fin, buff);
            std::stringstream point1(buff);
            int x1 = 0, y1 = 0;
            point1 >> x1 >> y1;

            std::getline(fin, buff);
            std::stringstream point2(buff);
            int x2 = 0, y2 = 0;
            point2 >> x2 >> y2;

            this->nets.push_back(new Net(identifier, id, x1, y1, x2, y2));
        }
    }
}

RouterInput *Parser::parse(char *argv[])
{
    readModifiedTxt(argv[1]);
    return new RouterInput(hGridCnt, vGridCnt,
                           hCapacity, vCapacity, nets);
}
