#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"

class Legalizer
{
    LegalizerInput *input;

    void divideRow();
    int getRowIdx(Cell const *cell);
    int getSubRowIdx(Row const *row, Cell const *cell);
    int placeRowTrail(int const &rowIdx, Cell *cell);
    void placeRowFinal(int const &rowIdx, int const &subRowIdx, Cell *cell);
    double calCost(Cell const *cell);
    void determinePosition();
    void abacusProcess();
    void calDisplacement();

public:
    Legalizer(LegalizerInput *input) : input(input) {}
    ResultWriter *solve();
};
