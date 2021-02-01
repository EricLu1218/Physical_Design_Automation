#include "Data.hpp"
#include <algorithm>
#include <limits>

void Hardblock::update(int const &_width, int const &_height, int const &_x, int const &_y)
{
    x = _x;
    y = _y;
    isRotated = !(width == _width && height == _height);
    pin->x = _x + _width / 2;
    pin->y = _y + _height / 2;
}

int Net::HPWL()
{
    int minX = std::numeric_limits<int>::max(), minY = std::numeric_limits<int>::max(),
        maxX = std::numeric_limits<int>::min(), maxY = std::numeric_limits<int>::min();
    for (auto pin : pins)
    {
        if (minX > pin->x)
            minX = pin->x;
        if (minY > pin->y)
            minY = pin->y;
        if (maxX < pin->x)
            maxX = pin->x;
        if (maxY < pin->y)
            maxY = pin->y;
    }
    return (maxX - minX) + (maxY - minY);
}

bool heightCmp(std::vector<int> const &a, std::vector<int> const &b)
{
    return a[1] > b[1];
}

void Node::updateRecord()
{
    record.clear();
    if (type == VERTICAL_CUT)
    {
        sort(lchild->record.begin(), lchild->record.end(), heightCmp);
        sort(rchild->record.begin(), rchild->record.end(), heightCmp);

        int l = 0, r = 0;
        while (l < static_cast<int>(lchild->record.size()) &&
               r < static_cast<int>(rchild->record.size()))
        {
            std::vector<int> row = {lchild->record[l][0] + rchild->record[r][0],
                                    std::max(lchild->record[l][1], rchild->record[r][1]),
                                    l, r};
            record.emplace_back(row);
            if (lchild->record[l][1] > rchild->record[r][1])
                l += 1;
            else if (lchild->record[l][1] < rchild->record[r][1])
                r += 1;
            else
            {
                l += 1;
                r += 1;
            }
        }
    }
    else
    {
        std::sort(lchild->record.begin(), lchild->record.end());
        std::sort(rchild->record.begin(), rchild->record.end());

        int l = lchild->record.size() - 1, r = rchild->record.size() - 1;
        while (l >= 0 && r >= 0)
        {
            std::vector<int> row = {std::max(lchild->record[l][0], rchild->record[r][0]),
                                    lchild->record[l][1] + rchild->record[r][1],
                                    l, r};
            record.emplace_back(row);
            if (lchild->record[l][0] > rchild->record[r][0])
                l -= 1;
            else if (lchild->record[l][0] < rchild->record[r][0])
                r -= 1;
            else
            {
                l -= 1;
                r -= 1;
            }
        }
    }
}
