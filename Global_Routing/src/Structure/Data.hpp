#pragma once
#include <cmath>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

struct Point
{
    int x, y;

    Point();
    Point(int x, int y);
};

struct Net
{
    using ptr = std::unique_ptr<Net>;

    std::string name;
    int id, x1, y1, x2, y2;

    int hpwl, overflow;
    std::vector<Point> routingPath;

    Net();
    Net(const std::string &name, int id, int x1, int y1, int x2, int y2);
    bool operator()(const Net *a, const Net *b) const;
    int wirelength() const;
};

struct Input
{
    using ptr = std::unique_ptr<Input>;

    int hGridCnt, vGridCnt, hCapacity, vCapacity;
    std::vector<Net::ptr> nets;

    Input();
};

struct Edge
{
    int capacity;
    double historicalCost;
    std::unordered_set<Net *> passNets;

    Edge();
    Edge(int capacity);
    bool operator()(const Edge *a, const Edge *b) const;
    int demand() const;
    int overflow() const;
};

enum struct Direction : int
{
    STOP = -1,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct GridNode
{
    Direction prevDirection;
    double cost;

    GridNode();
    void reset();
};

struct RoutingNode
{
    int x, y;
    double cost;

    RoutingNode();
    RoutingNode(int x, int y, double cost);
    bool operator()(const RoutingNode &a, const RoutingNode &b) const;
};

struct Result
{
    std::string netName;
    int netId;
    std::vector<Point> routingPath;

    Result();
    Result(const std::string &netName, int netId, const std::vector<Point> &routingPath);
};
