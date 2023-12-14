#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include "../Timer/Timer.hpp"
#include <queue>
#include <unordered_set>

class Router
{
    Input *input;
    Timer &timer;

    int k1;
    double baseCost;
    std::vector<std::vector<Edge>> hEdges, vEdges;
    std::vector<std::vector<GridNode>> routingGrid;

    Edge *getEdge(int x, int y, Direction direction);
    double calCost(const Edge *edge) const;
    int getTotalOverflow() const;
    int getWirelength() const;

    RoutingNode propagate(const RoutingNode &current, const RoutingNode &target, Direction direction);
    void wavePropagate(const RoutingNode &source, RoutingNode &target, const std::vector<Direction> directions);
    void traceBack(Net *net);
    void monotonicRouting(Net *net);

    std::priority_queue<Edge *, std::vector<Edge *>, Edge> getRipupQueue();
    void mazeRouting(Net *net);
    void ripupReroute(std::unordered_set<Net *> passNets);

public:
    Router(Input *input, Timer &timer);
    ResultWriter::ptr solve();
};
