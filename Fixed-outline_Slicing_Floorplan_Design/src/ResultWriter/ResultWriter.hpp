#pragma once
#include "../Structure/Data.hpp"
#include <memory>
#include <string>
#include <vector>

class ResultWriter
{
    int wirelength;
    std::vector<std::tuple<std::string, int, int, bool>> hardblocks; // (name, x, y, is rotated)

public:
    using ptr = std::unique_ptr<ResultWriter>;

    ResultWriter();
    void assignWirelength(int wirelength_);
    void addHardblock(const Hardblock *hardblock);
    void write(const std::string &filepath) const;
};
