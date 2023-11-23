#include "ResultWriter.hpp"
#include <fstream>
#include <iostream>

ResultWriter::ResultWriter() : cutSize(0), groups(2) {}

void ResultWriter::setCutsize(int cutsize_)
{
    cutSize = cutsize_;
}

void ResultWriter::addCell(Cell *cell)
{
    groups[cell->groupIdx].emplace_back(cell->name);
}

void ResultWriter::write(std::string const &filepath) const
{
    std::ofstream fout(filepath);
    if (!fout.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    fout << "cut_size " << cutSize << "\n";

    fout << "A " << groups[0].size() << "\n";
    for (const auto &cellName : groups[0])
        fout << cellName << "\n";

    fout << "B " << groups[1].size() << "\n";
    for (const auto &cellName : groups[1])
        fout << cellName << "\n";
}
