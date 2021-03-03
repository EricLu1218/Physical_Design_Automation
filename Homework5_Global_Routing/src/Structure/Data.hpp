#pragma once
#include <cmath>
#include <string>
#include <unordered_set>
#include <vector>

struct Net
{
    struct Point
    {
        int x, y;

        Point(int const &x, int const &y) : x(x), y(y) {}
    };

    std::string name;
    int id, x1, y1, x2, y2, hpwl, overflow;
    std::vector<Point> routingPath;

    Net() {}
    Net(std::string name, int id, int x1, int y1, int x2, int y2)
        : name(name), id(id), x1(x1), y1(y1), x2(x2), y2(y2),
          hpwl(std::abs(x1 - x2) + std::abs(y1 - y2)), overflow(0) {}
    inline bool operator()(Net const *A, Net const *B) const
    {
        if (A->overflow == B->overflow)
            return A->hpwl > B->hpwl;
        else
            return A->overflow < B->overflow;
    }

    int wirelength() const
    {
        if (routingPath.empty() == true)
            return 0;

        int _wirelength = 0;
        for (size_t i = 0; i < routingPath.size() - 1; ++i)
        {
            auto const &point1 = routingPath[i],
                       &point2 = routingPath[i + 1];
            _wirelength += std::abs(point1.x - point2.x) + std::abs(point1.y - point2.y);
        }
        return _wirelength;
    }
};

struct RouterInput
{
    int hGridCnt, vGridCnt, hCapacity, vCapacity;
    std::vector<Net *> nets;

    RouterInput(int hGridCnt, int vGridCnt,
                int hCapacity, int vCapacity, std::vector<Net *> nets)
        : hGridCnt(hGridCnt), vGridCnt(vGridCnt),
          hCapacity(hCapacity), vCapacity(vCapacity), nets(nets) {}
};

struct Edge
{
    int capacity;
    double historicalCost;
    std::unordered_set<Net *> passNets;

    Edge() {}
    Edge(int const &capacity) : capacity(capacity), historicalCost(1) {}
    inline bool operator()(Edge const *A, Edge const *B) const
    {
        return A->overflow() < B->overflow();
    }

    inline int demand() const
    {
        return passNets.size();
    }
    inline int overflow() const
    {
        return std::max(demand() - capacity, 0);
    }
};

struct RoutingNode
{
    int prevDirection;
    double cost;

    RoutingNode() : prevDirection(-1), cost(-1) {}
    inline void reset()
    {
        prevDirection = cost = -1;
    }
};

struct Node
{
    int x, y;
    double cost;

    Node() {}
    Node(int x, int y, double cost) : x(x), y(y), cost(cost) {}
    inline bool operator()(Node const &A, Node const &B) const
    {
        return A.cost > B.cost;
    }
};
