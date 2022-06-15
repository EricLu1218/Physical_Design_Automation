#include "Data.hpp"

void Group::insertCell(Cell *cell)
{
    size += cell->size;
}

void Group::removeCell(Cell *cell)
{
    size -= cell->size;
}

void Group::bulidBucketList()
{
    bucketListCnt = 0;
    bucketList.clear();
    for (int i = -Pmax; i <= Pmax; ++i)
        bucketList.emplace(i, new Node(nullptr));
}

void Group::insertNode(Cell *cell)
{
    auto head = bucketList.at(cell->gain);
    auto node = cell->node;
    node->next = head->next;
    node->before = head;
    if (node->next != nullptr)
        node->next->before = node;
    head->next = node;
    ++bucketListCnt;
}

void Group::removeNode(Cell *cell)
{
    auto node = cell->node;
    if (node->next != nullptr)
        node->next->before = node->before;
    node->before->next = node->next;
    --bucketListCnt;
}

void Group::moveNode(Cell *cell)
{
    removeNode(cell);
    insertNode(cell);
}

Cell *Group::getBaseCell()
{
    for (int i = Pmax; i >= -Pmax; --i)
        if (bucketList.at(i)->next != nullptr)
            return bucketList.at(i)->next->cell;
    return nullptr;
}
