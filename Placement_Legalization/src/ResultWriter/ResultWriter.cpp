#include "ResultWriter.hpp"
#include <fstream>
#include <iostream>

ResultWriter::ResultWriter() {}

void ResultWriter::addCell(const Cell *cell)
{
    cells.emplace_back(cell->name, cell->optimalX, cell->optimalY);
}

void ResultWriter::addBlockage(const Cell *blockage)
{
    blockages.emplace_back(blockage->name, blockage->x, blockage->y);
}

void ResultWriter::write(const std::string &filename) const
{
    std::ofstream fout(filename);
    if (!fout)
    {
        std::cerr << "[Error] Cannot open \"" << filename << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::string name;
    int x = 0, y = 0;
    for (const auto &cell : cells)
    {
        std::tie(name, x, y) = cell;
        fout << name << " " << x << " " << y << " : N\n";
    }
    for (const auto &blockage : blockages)
    {
        std::tie(name, x, y) = blockage;
        fout << name << " " << x << " " << y << " : N /FIXED\n";
    }
}
