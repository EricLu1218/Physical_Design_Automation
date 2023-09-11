#include "Data.hpp"
#include <cmath>

Cell::Cell() : width(0), height(0), x(0), y(0), weight(0), optimalX(0), optimalY(0) {}

Cell::Cell(const std::string &name, int width, int height)
    : name(name), width(width), height(height), x(0), y(0), weight(1), optimalX(0), optimalY(0) {}

double Cell::displacement() const
{
    double diffX = optimalX - x;
    double diffY = optimalY - y;
    return std::sqrt(diffX * diffX + diffY * diffY);
}

Cluster::Cluster()
    : x(0), weight(0), q(0), width(0), predecessor(nullptr) {}

Cluster::Cluster(double x, Cluster *predecessor)
    : x(x), weight(0), q(0), width(0), predecessor(predecessor) {}

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
