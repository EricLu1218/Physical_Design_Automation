#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Cell
{
    using ptr = std::unique_ptr<Cell>;

    std::string name;
    int width, height;
    double x, y;

    int weight;
    double optimalX, optimalY;

    Cell();
    Cell(const std::string &name, int width, int height);
    double displacement() const;
};

struct Cluster
{
    using ptr = std::shared_ptr<Cluster>;

    double x;
    int weight;
    double q;
    int width;
    Cluster::ptr predecessor;
    std::vector<Cell *> member;

    Cluster();
    Cluster(double x, Cluster::ptr predecessor);
};

struct SubRow
{
    using ptr = std::unique_ptr<SubRow>;

    int minX, maxX, freeWidth;

    Cluster::ptr lastCluster;

    SubRow();
    SubRow(int minX, int maxX);
    void updateMinMax(int minX_, int maxX_);
};

struct Row
{
    using ptr = std::unique_ptr<Row>;

    int y, height, siteWidth;
    std::vector<SubRow::ptr> subRows;

    Row();
    Row(int y, int height, int siteWidth);
};

struct Input
{
    using ptr = std::unique_ptr<Input>;

    int maxDisplacementInSite;
    std::vector<Cell::ptr> cells, blockages;
    std::vector<Row::ptr> rows;

    std::unordered_map<std::string, Cell *> strToCell;

    Input();
};
