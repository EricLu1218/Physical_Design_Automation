#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct Cell;
struct Node
{
    Cell *cell;
    Node *next, *before;

    Node(Cell *cell) : cell(cell), next(nullptr), before(nullptr) {}
};

struct Net;
struct Cell
{
    std::string name;
    int size, set, gain;
    bool lock;
    std::vector<Net *> nets;
    Node *node;

    Cell(std::string name, int size)
        : name(name), size(size), set(2), gain(0), lock(false), node(new Node(this)) {}
};

struct Net
{
    std::string name;
    std::vector<int> groupCnt;
    std::vector<Cell *> cells;

    Net(std::string name) : name(name)
    {
        groupCnt.resize(2, 0);
    }
};

struct FMInput
{
    double balanceFactor;
    std::vector<Cell *> cells;
    std::vector<Net *> nets;

    FMInput(double balanceFactor, std::vector<Cell *> cells, std::vector<Net *> nets)
        : balanceFactor(balanceFactor), cells(cells), nets(nets) {}
};

struct Group
{
    int size, Pmax, bucketListCnt;
    std::unordered_map<int, Node *> bucketList;

    Group() : size(0), Pmax(0), bucketListCnt(0) {}
    void insertCell(Cell *cell);
    void removeCell(Cell *cell);
    void bulidBucketList();
    void insertNode(Cell *cell);
    void removeNode(Cell *cell);
    void moveNode(Cell *cell);
    Cell *getBaseCell();
};
