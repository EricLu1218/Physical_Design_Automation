#include "ResultWriter.hpp"
#include <fstream>

void ResultWriter::write(char *argv[])
{
    std::string auxFilePath(argv[1]);
    auto filename = auxFilePath.substr(auxFilePath.find_last_of('/') + 1,
                                       auxFilePath.find_last_of('.') - auxFilePath.find_last_of('/') - 1);
    std::ofstream fout("../output/" + filename + ".result");
    for (auto const &cell : input->cells)
    {
        fout << cell->name << ' '
             << static_cast<int>(cell->optimalX) << ' '
             << static_cast<int>(cell->optimalY) << " : N\n";
    }
    for (auto const &terminal : input->terminals)
    {
        fout << terminal->name << ' '
             << static_cast<int>(terminal->x) << ' '
             << static_cast<int>(terminal->y) << " : N /FIXED\n";
    }
}
