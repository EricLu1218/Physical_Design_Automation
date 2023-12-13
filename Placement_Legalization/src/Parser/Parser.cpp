#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

inline std::string strip(std::string input, std::string chars = " \t\r\n")
{
    input.erase(0, input.find_first_not_of(chars));
    input.erase(input.find_last_not_of(chars) + 1);
    return input;
}

void Parser::readAux(Input *input, const std::string &filepath)
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
        std::string identifier, _;
        buffStream >> identifier;
        if (identifier == "RowBasedPlacement")
        {
            buffStream >> _ >> nodeFilename >> plFilename >> sclFilename;
        }
        else if (identifier == "MaxDisplacement")
        {
            buffStream >> _ >> input->maxDisplacementInSite;
        }
    }
}

void Parser::readNode(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    size_t nodeNum = 0, blockageNum = 0;
    std::string buff;
    while (std::getline(fin, buff))
    {
        buff = strip(buff);
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string identifier, _;
        buffStream >> identifier;
        if (identifier == "UCLA" || identifier[0] == '#')
        {
            continue;
        }
        else if (identifier == "NumNodes")
        {
            buffStream >> _ >> nodeNum;
        }
        else if (identifier == "NumTerminals")
        {
            buffStream >> _ >> blockageNum;
            break;
        }
    }

    size_t cellNum = nodeNum - blockageNum;
    while (std::getline(fin, buff))
    {
        buff = strip(buff);
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string name;
        int width = 0, height = 0;
        buffStream >> name >> width >> height;
        Cell *cell = new Cell(name, width, height);
        if (input->cells.size() < cellNum)
            input->cells.emplace_back(cell);
        else
            input->blockages.emplace_back(cell);
        input->strToCell.emplace(name, cell);
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

    std::string buff;
    while (std::getline(fin, buff))
    {
        buff = strip(buff);
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string name;
        buffStream >> name;
        if (name == "UCLA")
            continue;

        double x = 0, y = 0;
        buffStream >> x >> y;
        input->strToCell[name]->x = x;
        input->strToCell[name]->y = y;
    }
}

void Parser::readScl(Input *input, const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    size_t rowNum = 0;
    std::string buff;
    while (std::getline(fin, buff))
    {
        buff = strip(buff);
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string identifier, _;
        buffStream >> identifier;
        if (identifier == "UCLA" || identifier[0] == '#')
        {
            continue;
        }
        else if (identifier == "NumRows")
        {
            buffStream >> _ >> rowNum;
            break;
        }
    }

    for (size_t i = 0; i < rowNum; ++i)
    {
        int y = 0, height = 0, siteWidth = 0, x = 0, siteNum = 0;
        while (std::getline(fin, buff))
        {
            buff = strip(buff);
            if (buff.empty())
                continue;

            std::stringstream buffStream(buff);
            std::string identifier, _;
            buffStream >> identifier;
            if (identifier == "Coordinate")
            {
                buffStream >> _ >> y;
            }
            else if (identifier == "Height")
            {
                buffStream >> _ >> height;
            }
            else if (identifier == "Sitewidth")
            {
                buffStream >> _ >> siteWidth;
            }
            else if (identifier == "SubrowOrigin")
            {
                buffStream >> _ >> x >> _ >> _ >> siteNum;
            }
            else if (identifier == "End")
            {
                Row *row = new Row(y, height, siteWidth);
                row->subRows.emplace_back(new SubRow(x, x + siteWidth * siteNum));
                input->rows.emplace_back(row);
                break;
            }
        }
    }
}

Parser::Parser() {}

Input::ptr Parser::parse(const std::string &filepath)
{
    Input *input = new Input();
    readAux(input, filepath);

    std::string dir = filepath.substr(0, filepath.find_last_of('/')) + "/";
    readNode(input, dir + nodeFilename);
    readPl(input, dir + plFilename);
    readScl(input, dir + sclFilename);
    return std::unique_ptr<Input>(input);
}
