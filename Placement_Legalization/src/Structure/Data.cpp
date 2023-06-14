#include "Data.hpp"

Cell::Cell() : width(0), height(0), x(0), y(0), weight(0), optimalX(0), optimalY(0) {}

Cell::Cell(const std::string &name, int width, int height)
    : name(name), width(width), height(height), x(0), y(0), weight(width), optimalX(0), optimalY(0) {}

Cluster::Cluster()
    : x(0), weight(0), q(x * weight), width(0), predecessor(nullptr) {}

Cluster::Cluster(double x, Cell *cell, Cluster *predecessor)
    : x(x), weight(cell->weight), q(x * weight), width(cell->width), predecessor(predecessor)
{
    member.emplace_back(cell);
}

SubRow::SubRow() : minX(0), maxX(0), freeWidth(maxX - minX), lastCluster(nullptr) {}

SubRow::SubRow(int minX, int maxX) : minX(minX), maxX(maxX), freeWidth(maxX - minX), lastCluster(nullptr) {}

void SubRow::updateMinMax(int minX_, int maxX_)
{
    minX = minX_;
    maxX = maxX_;
    freeWidth = maxX_ - minX_;
}

Row::Row() : y(0), height(0), siteWidth(0) {}

Row::Row(int y, int height, int siteWidth) : y(y), height(height), siteWidth(siteWidth) {}

Input::Input() {}
