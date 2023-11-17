#include "Data.hpp"
#include <algorithm>
#include <limits>

Pin::Pin() : x(0), y(0) {}

Pin::Pin(const std::string &name, int x, int y) : name(name), x(x), y(y) {}

Hardblock::Hardblock()
    : width(0), height(0), x(0), y(0), isRotated(false), pin(new Pin(name, x, y)) {}

Hardblock::Hardblock(const std::string &name, int width, int height)
    : name(name), width(width), height(height), x(0), y(0), isRotated(false), pin(new Pin(name, x, y)) {}

void Hardblock::update(int width_, int height_, int x_, int y_)
{
    x = x_;
    y = y_;
    isRotated = !(width == width_ || height == height_);
    pin->x = x_ + width_ / 2;
    pin->y = y_ + height_ / 2;
}

Net::Net() {}

int Net::wirelength() const
{
    int minX = std::numeric_limits<int>::max(), minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min(), maxY = std::numeric_limits<int>::min();
    for (const auto pin : pins)
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

Input::Input() : deadspaceRatio(0) {}

Node::Record::Record() : width(0), height(0), leftChoice(0), rightChoice(0) {}

Node::Record::Record(int width, int height, int leftChoice, int rightChoice)
    : width(width), height(height), leftChoice(leftChoice), rightChoice(rightChoice) {}

Node::Node() : type(HARDBLOCK), hardblock(nullptr), lchild(nullptr), rchild(nullptr) {}

Node::Node(int type, Hardblock *hardblock) : type(type), hardblock(hardblock), lchild(nullptr), rchild(nullptr)
{
    if (hardblock)
    {
        records.emplace_back(hardblock->width, hardblock->height, 0, 0);
        records.emplace_back(hardblock->height, hardblock->width, 1, 1);
    }
}

void Node::updateRecord()
{
    records.clear();
    if (type == HORIZONTAL_CUT)
    {
        auto cmp = [](const Record &a, const Record &b)
        {
            return a.width <= b.width;
        };
        std::sort(lchild->records.begin(), lchild->records.end(), cmp);
        std::sort(rchild->records.begin(), rchild->records.end(), cmp);

        int l = lchild->records.size() - 1, r = rchild->records.size() - 1;
        while (l >= 0 && r >= 0)
        {
            const auto &left = lchild->records[l];
            const auto &right = rchild->records[r];
            records.emplace_back(std::max(left.width, right.width), left.height + right.height, l, r);
            if (left.width >= right.width)
                --l;
            if (left.width <= right.width)
                --r;
        }
    }
    else if (type == VERTICAL_CUT)
    {
        auto cmp = [](const Record &a, const Record &b)
        {
            return a.height >= b.height;
        };
        std::sort(lchild->records.begin(), lchild->records.end(), cmp);
        std::sort(rchild->records.begin(), rchild->records.end(), cmp);

        size_t l = 0, r = 0;
        while (l < lchild->records.size() && r < rchild->records.size())
        {
            const auto &left = lchild->records[l];
            const auto &right = rchild->records[r];
            records.emplace_back(left.width + right.width, std::max(left.height, right.height), l, r);
            if (left.height >= right.height)
                ++l;
            if (left.height <= right.height)
                ++r;
        }
    }
}
