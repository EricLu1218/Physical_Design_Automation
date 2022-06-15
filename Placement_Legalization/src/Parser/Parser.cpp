#include "Parser.hpp"
#include <unordered_map>

void Parser::readAux(std::string const &filename)
{
    std::ifstream fin(filename);
    std::string buff;
    while (std::getline(fin, buff))
    {
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string identifier, temp;
        buffStream >> identifier;
        if (identifier == "RowBasedPlacement")
        {
            buffStream >> temp >> this->nodeFile >> this->plFile >> this->sclFile;
        }
        else if (identifier == "MaxDisplacement")
        {
            buffStream >> temp >> this->maxDisplacement;
        }
    }
}

std::unordered_map<std::string, Cell *> strToCell;
void Parser::readNode(std::string const &filename)
{
    std::ifstream fin(filename);
    size_t nodeNum = 0, terminalNum = 0;
    std::string buff;
    while (std::getline(fin, buff))
    {
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string identifier, temp;
        buffStream >> identifier;
        if (identifier == "UCLA" || identifier.at(0) == '#')
        {
            continue;
        }
        else if (identifier == "NumNodes")
        {
            buffStream >> temp >> nodeNum;
        }
        else if (identifier == "NumTerminals")
        {
            buffStream >> temp >> terminalNum;
            break;
        }
    }
    std::getline(fin, buff);

    size_t const cellNum = nodeNum - terminalNum;
    for (size_t i = 0; i < nodeNum; ++i)
    {
        std::getline(fin, buff);
        std::stringstream buffStream(buff);
        std::string name;
        int width = 0, height = 0;
        buffStream >> name >> width >> height;
        auto cell = new Cell(name, width, height);
        if (i < cellNum)
            this->cells.emplace_back(cell);
        else
            this->terminals.emplace_back(cell);
        strToCell.emplace(name, cell);
    }
}

void Parser::readPl(std::string const &filename)
{
    std::ifstream fin(filename);
    std::string buff;
    std::getline(fin, buff);
    std::getline(fin, buff);

    for (size_t i = 0; i < this->cells.size() + this->terminals.size(); ++i)
    {
        std::getline(fin, buff);
        std::stringstream buffStream(buff);
        std::string name;
        double x = 0, y = 0;
        buffStream >> name >> x >> y;
        strToCell.at(name)->x = x;
        strToCell.at(name)->y = y;
    }
}

void Parser::readScl(std::string const &filename)
{
    std::ifstream fin(filename);
    size_t rowNum = 0;
    std::string buff;
    while (std::getline(fin, buff))
    {
        if (buff.empty())
            continue;

        std::stringstream buffStream(buff);
        std::string identifier, temp;
        buffStream >> identifier;
        if (identifier == "UCLA" || identifier.at(0) == '#')
        {
            continue;
        }
        else if (identifier == "NumRows")
        {
            buffStream >> temp >> rowNum;
            break;
        }
    }

    for (size_t i = 0; i < rowNum; ++i)
    {
        int y = 0, height = 0, siteWidth = 0, x = 0, siteNum = 0;
        while (std::getline(fin, buff))
        {
            if (buff.empty())
                continue;

            std::stringstream buffStream(buff);
            std::string identifier, temp;
            buffStream >> identifier;
            if (identifier == "Coordinate")
            {
                buffStream >> temp >> y;
            }
            else if (identifier == "Height")
            {
                buffStream >> temp >> height;
            }
            else if (identifier == "Sitewidth")
            {
                buffStream >> temp >> siteWidth;
            }
            else if (identifier == "SubrowOrigin")
            {
                buffStream >> temp >> x >> temp >> temp >> siteNum;
            }
            else if (identifier == "End")
            {
                auto row = new Row(siteWidth, height, y);
                row->subRows.emplace_back(new SubRow(x, x + siteWidth * siteNum));
                this->rows.emplace_back(row);
                break;
            }
        }
    }
}

LegalizerInput *Parser::parse(char argv[])
{
    std::string auxFilePath(argv);
    readAux(auxFilePath);

    std::string inputPath = auxFilePath.substr(0, auxFilePath.find_last_of('/'));
    readNode(inputPath + '/' + this->nodeFile);
    readPl(inputPath + '/' + this->plFile);
    readScl(inputPath + '/' + this->sclFile);

    return new LegalizerInput(this->maxDisplacement, this->cells, this->terminals, this->rows);
}
