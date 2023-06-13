#include "ResultWriter.hpp"
#include <fstream>
#include <iostream>

ResultWriter::ResultWriter() {}

void ResultWriter::assignWirelength(int wirelength_)
{
    wirelength = wirelength_;
}

void ResultWriter::addHardblock(const Hardblock *hardblock)
{
    hardblocks.emplace_back(hardblock->name, hardblock->x, hardblock->y, hardblock->isRotated);
}

void ResultWriter::write(const std::string &filename) const
{
    std::ofstream fout(filename);
    if (!fout)
    {
        std::cerr << "[Error] Cannot open \"" << filename << "\".\n";
        exit(EXIT_FAILURE);
    }

    fout << "Wirelength " << wirelength << "\n";

    std::string name;
    int x, y;
    bool isRotated;
    fout << "Blocks\n";
    for (const auto &hardblock : hardblocks)
    {
        std::tie(name, x, y, isRotated) = hardblock;
        fout << name << " " << x << " " << y << " " << isRotated << "\n";
    }
}
