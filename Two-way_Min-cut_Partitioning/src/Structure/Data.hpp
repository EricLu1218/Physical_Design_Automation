#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Node
{
    Node *prev, *next;

    Node();
};

struct Net;
struct Cell : Node
{
    using ptr = std::unique_ptr<Cell>;

    std::string name;
    int size;
    std::vector<Net *> nets;

    int groupIdx, gain;
    bool lock;

    Cell();
    Cell(const std::string &name, int size);
};

struct Net
{
    using ptr = std::unique_ptr<Net>;

    std::string name;
    std::vector<Cell *> cells;

    std::vector<int> numCellInGroup;

    Net();
    Net(const std::string &name);
    void updateNumCellInGroup();
};

struct Input
{
    using ptr = std::unique_ptr<Input>;

    double maxDiffSize;
    std::vector<Cell::ptr> cells;
    std::vector<Net::ptr> nets;

    Input();
};

struct Group
{
    int size, pMax, numCellInBucketList;
    std::unordered_map<int, Node> bucketList;

    Group();
    void initBucketList();
    void insertNode(Cell *cell);
    void removeNode(Cell *cell);
    void moveNode(Cell *cell);
    Cell *getBaseCell() const;
};
