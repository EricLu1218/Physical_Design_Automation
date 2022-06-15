#pragma once
#include <string>
#include <vector>

struct Cell
{
    std::string name;
    int width, height, weight;
    double x, y, optimalX, optimalY;

    Cell(std::string const &name, int const &width, int const &height)
        : name(name), width(width), height(height), weight(width), x(0), y(0), optimalX(0), optimalY(0) {}
};

struct Cluster
{
    double x, q;
    int width, weight;
    std::vector<Cell *> member;
    Cluster *predecessor;

    Cluster(double const &x, Cluster *predecessor, int const &weight, double const &q, int const &width)
        : x(x), q(q), width(width), weight(weight), predecessor(predecessor) {}
};

struct SubRow
{
    int minX, maxX, freeWidth;
    Cluster *lastCluster;

    SubRow(int const &minX, int const &maxX)
        : minX(minX), maxX(maxX), freeWidth(maxX - minX), lastCluster(nullptr) {}
    inline void updateMinMax(int const &_minX, int const &_maxX)
    {
        this->minX = _minX;
        this->maxX = _maxX;
        this->freeWidth = _maxX - _minX;
    }
};

struct Row
{
    int width, height, y;
    std::vector<SubRow *> subRows;

    Row(int const &width, int const &height, int const &y)
        : width(width), height(height), y(y) {}
};

struct LegalizerInput
{
    int maxDisplacement;
    std::vector<Cell *> cells, terminals;
    std::vector<Row *> rows;

    LegalizerInput(int const &maxDisplacement, std::vector<Cell *> const &cells,
                std::vector<Cell *> const &terminals, std::vector<Row *> const &rows)
        : maxDisplacement(maxDisplacement), cells(cells), terminals(terminals), rows(rows) {}
};
