#pragma once
#include "../Structure/Data.hpp"
#include <memory>
#include <string>
#include <vector>

class ResultWriter
{
    int cutSize;
    std::vector<std::vector<std::string>> groups;

public:
    using ptr = std::unique_ptr<ResultWriter>;

    ResultWriter();
    void setCutsize(int cutsize_);
    void addCell(Cell *cell);
    void write(const std::string &filename) const;
};
