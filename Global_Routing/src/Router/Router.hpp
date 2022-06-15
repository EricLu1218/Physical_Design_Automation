#pragma once
#include "../GlobalTimer/GlobalTimer.hpp"
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include <queue>
#include <unordered_set>

class Router
{
    RouterInput *input;
    GlobalTimer &globalTimer;

    enum Direction
    {
        STOP = -1,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    int k1 = 5;
    double baseCost = 0;
    std::vector<std::vector<Edge>> hEdges, vEdges;
    std::vector<std::vector<RoutingNode>> routingMap;

    Edge *getEdge(int const &x, int const &y, int const &direction);
    double calCost(Edge const *edge);
    Node wavePropagation(Node const &current, Node const &target, int const &direction);
    void traceBack(Net *net);
    void routeNet(Net *net);
    void getRipUpQueue(std::priority_queue<Edge *, std::vector<Edge *>, Edge> &ripUpQueue);
    void rerouteNet(Net *net);
    void ripUpReroute(std::unordered_set<Net *> passNets);
    int calTotalOverflow();
    int calWirelength();

public:
    Router(RouterInput *input, GlobalTimer &globalTimer)
        : input(input), globalTimer(globalTimer)
    {
        hEdges = std::vector<std::vector<Edge>>(
            input->vGridCnt, std::vector<Edge>(input->hGridCnt - 1, input->hCapacity));
        vEdges = std::vector<std::vector<Edge>>(
            input->vGridCnt - 1, std::vector<Edge>(input->hGridCnt, input->vCapacity));
        routingMap = std::vector<std::vector<RoutingNode>>(
            input->vGridCnt, std::vector<RoutingNode>(input->hGridCnt));
    }
    ResultWriter *solve();
};
