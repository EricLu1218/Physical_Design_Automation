#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"

class FM_Partitioner
{
    FMInput input;

    int64_t bestStep;
    std::vector<Group> group;

    void initPartition();
    void calNetGroup();
    size_t getCutSize();
    void calInitGain();
    void bulidBucketList();
    int64_t update_gain(Cell *maxGainCell);
    int64_t fmProcess();

public:
    FM_Partitioner(FMInput *input);

    ResultWriter *solve();
};