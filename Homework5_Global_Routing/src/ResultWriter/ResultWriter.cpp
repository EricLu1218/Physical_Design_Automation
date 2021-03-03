#include "ResultWriter.hpp"
#include <algorithm>
#include <fstream>

bool netIdCmp(Net const *A, Net const *B)
{
    return A->id < B->id;
}

void ResultWriter::write(std::string const &filename)
{
    std::ofstream fout(filename);
    std::sort(input->nets.begin(), input->nets.end(), netIdCmp);
    for (auto const &net : input->nets)
    {
        if (net->routingPath.empty() == true)
            continue;

        fout << net->name << ' ' << net->id << '\n';
        for (size_t i = 0; i < net->routingPath.size() - 1; ++i)
        {
            auto const &point1 = net->routingPath[i],
                       &point2 = net->routingPath[i + 1];
            fout << '(' << point1.x << ", " << point1.y << ", 1)-"
                 << '(' << point2.x << ", " << point2.y << ", 1)\n";
        }
        fout << "!\n";
    }
}
