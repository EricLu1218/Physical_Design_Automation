#pragma once
#include "../Structure/Data.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class ResultWriter
{
    std::vector<std::tuple<std::string, int, int>> cells, blockages;

public:
    using ptr = std::unique_ptr<ResultWriter>;

    ResultWriter();
    void addCell(const Cell *cell);
    void addBlockage(const Cell *blockage);
    void write(const std::string &filename) const;
};
