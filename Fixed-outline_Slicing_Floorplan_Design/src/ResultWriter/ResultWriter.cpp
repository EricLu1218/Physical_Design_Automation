#include "ResultWriter.hpp"
#include <fstream>

void ResultWriter::write(std::string const &filename)
{
    std::ofstream fout(filename);
    int wirelength = 0;
    for (auto const &net : input->nets)
        wirelength += net->HPWL();
    fout << "Wirelength " << wirelength << '\n'
         << "Blocks\n";
    for (auto const &hardblock : input->hardblocks)
        fout << hardblock->name << ' '
             << hardblock->x << ' '
             << hardblock->y << ' '
             << hardblock->isRotated << '\n';
}
