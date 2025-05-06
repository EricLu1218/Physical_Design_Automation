#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include <utility>

class Legalizer
{
    Input *input;
    double maxDisplacementConstraint;

    int getRowIdx(const Cell *cell) const;
    int getSubRowIdx(const Row *row, const Cell *cell) const;
    std::pair<double, double> getTotalAndMaxDisplacement() const;

    void divideRow();
    std::pair<int, double> placeRowTrial(const Row *row, Cell *cell, bool addPenalty);
    void placeRowFinal(SubRow *subRow, Cell *cell);
    void abacusProcess();
    void determinePosition();

public:
    Legalizer(Input *input);
    ResultWriter::ptr solve();
};
