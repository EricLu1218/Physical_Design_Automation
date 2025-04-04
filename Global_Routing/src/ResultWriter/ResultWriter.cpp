#include "ResultWriter.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

ResultWriter::ResultWriter() {}

void ResultWriter::addResult(const Net *net)
{
    results.emplace_back(net->name, net->id, net->routingPath);
}

void ResultWriter::write(const std::string &filepath)
{
    std::ofstream fout(filepath);
    if (!fout.is_open())
    {
        std::cerr << "[Error] Cannot open \"" << filepath << "\".\n";
        exit(EXIT_FAILURE);
    }

    std::sort(results.begin(), results.end(), [](const Result &a, const Result &b) -> bool
              { return a.netId <= b.netId; });
    for (const Result &result : results)
    {
        if (result.routingPath.empty())
            continue;

        fout << result.netName << " " << result.netId << "\n";
        for (size_t i = 1; i < result.routingPath.size(); ++i)
        {
            const Point &point1 = result.routingPath[i - 1];
            const Point &point2 = result.routingPath[i];
            fout << "(" << point1.x << ", " << point1.y << ", 1)-(" << point2.x << ", " << point2.y << ", 1)\n";
        }
        fout << "!\n";
    }
}
