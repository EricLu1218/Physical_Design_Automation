#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include <utility>

class Legalizer
{
    Input *input;
    double maxDisplacementConstraint;

    void divideRow();
    int getRowIdx(const Cell *cell) const;
    int getSubRowIdx(const Row *row, const Cell *cell) const;
    double getSiteX(double x, int minX, int siteWidth) const;
    std::pair<int, double> placeRowTrial(const Row *row, Cell *cell, bool addPenalty);
    void placeRowFinal(SubRow *subRow, Cell *cell);
    void determinePosition();
    void abacusProcess();
    std::pair<double, double> getTotalAndMaxDisplacement() const;

public:
    Legalizer(Input *input);
    ResultWriter::ptr solve();
};
