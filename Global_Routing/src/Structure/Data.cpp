#include "Data.hpp"

Point::Point() : x(0), y(0) {}

Point::Point(int x, int y) : x(x), y(y) {}

Net::Net() : id(0), x1(0), y1(0), x2(0), y2(0), hpwl(0), overflow(0) {}

Net::Net(const std::string &name, int id, int x1, int y1, int x2, int y2)
    : name(name), id(id), x1(x1), y1(y1), x2(x2), y2(y2),
      hpwl(std::abs(x1 - x2) + std::abs(y1 - y2)), overflow(0) {}

bool Net::operator()(const Net *a, const Net *b) const
{
    if (a->overflow == b->overflow)
        return a->hpwl > b->hpwl;
    else
        return a->overflow < b->overflow;
}

int Net::wirelength() const
{
    int length = 0;
    for (size_t i = 1; i < routingPath.size(); ++i)
        length += std::abs(routingPath[i - 1].x - routingPath[i].x) +
                  std::abs(routingPath[i - 1].y - routingPath[i].y);
    return length;
}

Input::Input() : hGridCnt(0), vGridCnt(0), hCapacity(0), vCapacity(0) {}

Edge::Edge() : capacity(0), historicalCost(0) {}

Edge::Edge(int capacity) : capacity(capacity), historicalCost(1) {}

bool Edge::operator()(const Edge *a, const Edge *b) const
{
    return a->overflow() < b->overflow();
}

int Edge::demand() const
{
    return passNets.size();
}
int Edge::overflow() const
{
    return std::max(demand() - capacity, 0);
}

GridNode::GridNode() : prevDirection(-1), cost(-1) {}

void GridNode::reset()
{
    prevDirection = cost = -1;
}

RoutingNode::RoutingNode() : x(0), y(0) {}

RoutingNode::RoutingNode(int x, int y, double cost) : x(x), y(y), cost(cost) {}

bool RoutingNode::operator()(const RoutingNode &a, const RoutingNode &b) const
{
    return a.cost > b.cost;
}

Result::Result() : netId(0) {}

Result::Result(const std::string &netName, int netId, const std::vector<Point> &routingPath)
    : netName(netName), netId(netId), routingPath(routingPath) {}
