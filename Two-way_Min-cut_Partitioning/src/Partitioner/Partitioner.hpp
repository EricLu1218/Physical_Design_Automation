#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"

class Partitioner
{
    Input *input;

    std::vector<Group> groups;

    void generateInitialPartition();
    int getCutSize() const;
    void updateAllCellGain();
    void bulidBucketList();
    void updateGain(Cell *baseCell);
    int fmProcess();

public:
    Partitioner(Input *input);
    ResultWriter::ptr solve();
};
