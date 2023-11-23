#pragma once
#include "../Structure/Data.hpp"
#include <memory>
#include <string>
#include <vector>

class ResultWriter
{
public:
    using ptr = std::unique_ptr<ResultWriter>;

    std::vector<Result> results;

    ResultWriter();
    void addResult(const Net *net);
    void write(const std::string &filepath);
};
