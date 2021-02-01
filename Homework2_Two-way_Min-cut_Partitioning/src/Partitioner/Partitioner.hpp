#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"

class FM_Partitioner
{
    FMInput *input;

    int bestStep;
    std::vector<Group> group;

    void initPartition();
    void calNetGroup();
    size_t getCutSize();
    void calInitGain();
    void bulidBucketList();
    int update_gain(Cell *baseCell);
    int fmProcess();

public:
    FM_Partitioner(FMInput *input);
    ResultWriter *solve();
};
