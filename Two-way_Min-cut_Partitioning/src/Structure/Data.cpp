#include "Data.hpp"

Node::Node() : prev(nullptr), next(nullptr) {}

Cell::Cell() : size(0), groupIdx(-1), gain(0), lock(false) {}

Cell::Cell(const std::string &name, int size)
    : name(name), size(size), groupIdx(-1), gain(0), lock(false) {}

Net::Net() {}

Net::Net(const std::string &name) : name(name), numCellInGroup(2, 0) {}

void Net::updateNumCellInGroup()
{
    numCellInGroup[0] = numCellInGroup[1] = 0;
    for (const auto cell : cells)
        ++numCellInGroup[cell->groupIdx];
}

Input::Input() : maxDiffSize(0) {}

Group::Group() : size(0), pMax(0), numCellInBucketList(0) {}

void Group::initBucketList()
{
    numCellInBucketList = 0;
    bucketList.clear();
    for (int i = -pMax; i <= pMax; ++i)
        bucketList[i];
}

void Group::insertNode(Cell *cell)
{
    ++numCellInBucketList;
    auto head = &bucketList[cell->gain];
    cell->prev = head;
    cell->next = head->next;
    if (head->next)
        head->next->prev = cell;
    head->next = cell;
}

void Group::removeNode(Cell *cell)
{
    --numCellInBucketList;
    if (cell->next)
        cell->next->prev = cell->prev;
    cell->prev->next = cell->next;
    cell->prev = cell->next = nullptr;
}

void Group::moveNode(Cell *cell)
{
    removeNode(cell);
    insertNode(cell);
}

Cell *Group::getBaseCell() const
{
    for (int i = pMax; i >= -pMax; --i)
        if (bucketList.at(i).next)
            return static_cast<Cell *>(bucketList.at(i).next);
    return nullptr;
}
