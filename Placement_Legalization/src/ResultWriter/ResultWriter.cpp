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
    if (!fout.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filename << "\".\n";
        exit(EXIT_FAILURE);
    }

    for (const auto &[name, x, y] : cells)
        fout << name << " " << x << " " << y << " : N\n";
    for (const auto &[name, x, y] : blockages)
        fout << name << " " << x << " " << y << " : N /FIXED\n";
}
