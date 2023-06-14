#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"

class Legalizer
{
    Input *input;

    void divideRow();
    int getRowIdx(const Cell *cell) const;
    int getSubRowIdx(const Row *row, const Cell *cell) const;
    int placeRowTrail(const Row *row, Cell *cell);
    void placeRowFinal(SubRow *subRow, Cell *cell);
    double calCost(const Cell *cell) const;
    void determinePosition();
    void abacusProcess();
    std::pair<double, double> getTotalAndMaxDisplacement() const;

public:
    Legalizer(Input *input);
    ResultWriter::ptr solve();
};
