#include "ResultWriter.hpp"
#include <fstream>

void ResultWriter::write(std::string const &filename)
{
    size_t cnt = 0;
    for (auto cell : input->cells)
        if (cell->set == 0)
            ++cnt;

    std::ofstream fout(filename);
    fout << "cut_size " << cutSize << '\n';
    fout << "A " << cnt << '\n';
    for (auto cell : input->cells)
        if (cell->set == 0)
            fout << cell->name << '\n';

    fout << "B " << input->cells.size() - cnt << '\n';
    for (auto cell : input->cells)
        if (cell->set == 1)
            fout << cell->name << '\n';
}
