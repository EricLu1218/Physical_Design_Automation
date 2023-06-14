#include "ResultWriter.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

ResultWriter::ResultWriter() {}

void ResultWriter::addResult(const Net *net)
{
    results.emplace_back(net->name, net->id, net->routingPath);
}

void ResultWriter::write(const std::string &filename)
{
    std::ofstream fout(filename);
    if (!fout)
    {
        std::cerr << "[Error] Cannot open \"" << filename << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::sort(results.begin(), results.end(), [](const Result &a, const Result &b)
              { return a.netId <= b.netId; });
    for (const auto &result : results)
    {
        if (result.routingPath.empty())
            continue;

        fout << result.netName << " " << result.netId << "\n";
        for (size_t i = 1; i < result.routingPath.size(); ++i)
        {
            const auto &point1 = result.routingPath[i - 1];
            const auto &point2 = result.routingPath[i];
            fout << "(" << point1.x << ", " << point1.y << ", 1)-(" << point2.x << ", " << point2.y << ", 1)\n";
        }
        fout << "!\n";
    }
}
