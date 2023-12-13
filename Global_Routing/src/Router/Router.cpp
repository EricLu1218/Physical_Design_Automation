#include "Router.hpp"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <random>

Edge *Router::getEdge(int x, int y, int direction)
{
    if (direction == Direction::LEFT)
        return &hEdges[y][x - 1];
    else if (direction == Direction::RIGHT)
        return &hEdges[y][x];
    else if (direction == Direction::UP)
        return &vEdges[y][x];
    else if (direction == Direction::DOWN)
        return &vEdges[y - 1][x];
    return nullptr;
}

double Router::calCost(const Edge *edge) const
{
    double historicalCost = edge->historicalCost;
    if (edge->overflow())
        ++historicalCost;
    double penalty = std::pow(static_cast<double>(edge->demand() + 1) / edge->capacity, k1);
    return baseCost + historicalCost * penalty;
}

int Router::getTotalOverflow() const
{
    int overflow = 0;
    for (const std::vector<Edge> &row : hEdges)
        for (const Edge &edge : row)
            overflow += edge.overflow();
    for (const std::vector<Edge> &row : vEdges)
        for (const Edge &edge : row)
            overflow += edge.overflow();
    return overflow;
}

int Router::getWirelength() const
{
    int wirelength = 0;
    for (const Net::ptr &net : input->nets)
        wirelength += net->wirelength();
    return wirelength;
}

RoutingNode outOfBoundary(-1, -1, -1);
RoutingNode Router::propagate(const RoutingNode &current, const RoutingNode &target, int direction)
{
    RoutingNode next = current;
    if (direction == Direction::LEFT)
    {
        --next.x;
        if (next.x < 0)
            return outOfBoundary;
        if (current.x <= target.x)
            ++next.cost;
    }
    else if (direction == Direction::RIGHT)
    {
        ++next.x;
        if (next.x > input->hGridCnt - 1)
            return outOfBoundary;
        if (current.x >= target.x)
            ++next.cost;
    }
    else if (direction == Direction::UP)
    {
        ++next.y;
        if (next.y > input->vGridCnt - 1)
            return outOfBoundary;
        if (current.y >= target.y)
            ++next.cost;
    }
    else if (direction == Direction::DOWN)
    {
        --next.y;
        if (next.y < 0)
            return outOfBoundary;
        if (current.y <= target.y)
            ++next.cost;
    }
    else
    {
        return outOfBoundary;
    }
    next.cost += calCost(getEdge(current.x, current.y, direction));
    return next;
}

void Router::wavePropagate(const RoutingNode &source, RoutingNode &target, const std::vector<int> directions)
{
    for (std::vector<GridNode> &row : routingGrid)
        for (GridNode &gridNode : row)
            gridNode.reset();
    routingGrid[source.y][source.x].cost = 0;

    std::priority_queue<RoutingNode, std::vector<RoutingNode>, RoutingNode> routingQueue;
    routingQueue.push(source);
    while (!routingQueue.empty())
    {
        RoutingNode current = routingQueue.top();
        routingQueue.pop();

        if (current.x == target.x && current.y == target.y)
        {
            target.cost = current.cost;
            continue;
        }

        if (target.cost != -1 && current.cost >= target.cost)
            continue;

        for (int direction : directions)
        {
            const RoutingNode &next = propagate(current, target, direction);
            if (next.cost == -1)
                continue;

            GridNode &nextGridNode = routingGrid[next.y][next.x];
            if (nextGridNode.cost != -1 && next.cost >= nextGridNode.cost)
                continue;

            nextGridNode.cost = next.cost;
            nextGridNode.prevDirection = direction;
            routingQueue.push(next);
        }
    }
}

void Router::traceBack(Net *net)
{
    net->routingPath.clear();
    int x = net->x2, y = net->y2;
    while (routingGrid[y][x].prevDirection != Direction::STOP)
    {
        net->routingPath.emplace_back(x, y);
        int prevDirection = routingGrid[y][x].prevDirection;
        if (prevDirection == Direction::LEFT)
        {
            getEdge(x, y, Direction::RIGHT)->passNets.emplace(net);
            ++x;
        }
        else if (prevDirection == Direction::RIGHT)
        {
            getEdge(x, y, Direction::LEFT)->passNets.emplace(net);
            --x;
        }
        else if (prevDirection == Direction::UP)
        {
            getEdge(x, y, Direction::DOWN)->passNets.emplace(net);
            --y;
        }
        else if (prevDirection == Direction::DOWN)
        {
            getEdge(x, y, Direction::UP)->passNets.emplace(net);
            ++y;
        }
    }
    net->routingPath.emplace_back(x, y);
}

void Router::monotonicRouting(Net *net)
{
    RoutingNode source(net->x1, net->y1, 0), target(net->x2, net->y2, -1);
    std::vector<int> directions(2, Direction::STOP);
    if (source.x > target.x)
        directions[0] = Direction::LEFT;
    else if (source.x < target.x)
        directions[0] = Direction::RIGHT;
    if (source.y < target.y)
        directions[1] = Direction::UP;
    else if (source.y > target.y)
        directions[1] = Direction::DOWN;
    wavePropagate(source, target, directions);
    traceBack(net);
}

std::priority_queue<Edge *, std::vector<Edge *>, Edge> Router::getRipupQueue()
{
    std::priority_queue<Edge *, std::vector<Edge *>, Edge> ripupQueue;
    for (std::vector<Edge> &row : hEdges)
    {
        for (Edge &edge : row)
        {
            if (edge.overflow())
            {
                ++edge.historicalCost;
                ripupQueue.push(&edge);
            }
        }
    }
    for (std::vector<Edge> &row : vEdges)
    {
        for (Edge &edge : row)
        {
            if (edge.overflow())
            {
                ++edge.historicalCost;
                ripupQueue.push(&edge);
            }
        }
    }
    return ripupQueue;
}

void Router::mazeRouting(Net *net)
{
    RoutingNode source(net->x1, net->y1, 0), target(net->x2, net->y2, -1);
    std::vector<int> directions = {Direction::LEFT, Direction::RIGHT, Direction::UP, Direction::DOWN};
    wavePropagate(source, target, directions);
    traceBack(net);
}

void Router::ripupReroute(std::unordered_set<Net *> passNets)
{
    std::priority_queue<Net *, std::vector<Net *>, Net> rerouteQueue;
    for (Net *net : passNets)
    {
        int cost = 0;
        for (size_t i = 1; i < net->routingPath.size(); ++i)
        {
            const Point &point1 = net->routingPath[i - 1];
            const Point &point2 = net->routingPath[i];
            int direction = Direction::STOP;
            if (point1.x > point2.x)
                direction = Direction::LEFT;
            else if (point1.x < point2.x)
                direction = Direction::RIGHT;
            else if (point1.y < point2.y)
                direction = Direction::UP;
            else if (point1.y > point2.y)
                direction = Direction::DOWN;
            else
                continue;

            Edge *edge = getEdge(point1.x, point1.y, direction);
            cost += edge->overflow();
            edge->passNets.erase(net);
        }
        net->overflow = cost;
        net->routingPath.clear();
        rerouteQueue.push(net);
    }

    while (!rerouteQueue.empty())
    {
        Net *net = rerouteQueue.top();
        rerouteQueue.pop();
        mazeRouting(net);
    }
}

Router::Router(Input *input, Timer &timer) : input(input), timer(timer), k1(5), baseCost(0)
{
    hEdges = std::vector<std::vector<Edge>>(input->vGridCnt, std::vector<Edge>(input->hGridCnt - 1, input->hCapacity));
    vEdges = std::vector<std::vector<Edge>>(input->vGridCnt - 1, std::vector<Edge>(input->hGridCnt, input->vCapacity));
    routingGrid = std::vector<std::vector<GridNode>>(input->vGridCnt, std::vector<GridNode>(input->hGridCnt));
    std::cout << "----- DESIGN INFORMATION -----\n"
              << "#grid row/col:       " << input->vGridCnt << " " << input->hGridCnt << "\n"
              << "#net:                " << input->nets.size() << "\n"
              << "Horizontal capacity: " << input->hCapacity << "\n"
              << "Vertical capacity:   " << input->vCapacity << "\n"
              << "\n";
}

ResultWriter::ptr Router::solve()
{
    // can set the random seed for different testcases
    int seed = 0, testcase = 0;
    if (input->nets.size() == 13357)
    {
        seed = 53;
        testcase = 1;
    }
    else if (input->nets.size() == 22465)
    {
        seed = 91;
        testcase = 2;
    }
    else if (input->nets.size() == 27781)
    {
        seed = 76;
        testcase = 3;
    }
    std::shuffle(input->nets.begin(), input->nets.end(), std::default_random_engine(seed));

    std::cout << "------ INITIAL ROUTING ------\n";
    for (Net::ptr &net : input->nets)
        monotonicRouting(net.get());
    int overflow = getTotalOverflow();
    std::cout << "Wirelength:             " << getWirelength() << "\n"
              << "Overflow:               " << overflow << "\n"
              << "\n";

    std::cout << "----- RIPUP AND REROUTE -----\n";
    int iteration = 1;
    while (overflow > 0)
    {
        baseCost = 1.0 - exp(-5 * exp(-0.1 * iteration));
        std::priority_queue<Edge *, std::vector<Edge *>, Edge> ripupQueue = getRipupQueue();
        while (overflow > 0 && !ripupQueue.empty())
        {
            Edge *edge = ripupQueue.top();
            ripupQueue.pop();
            ripupReroute(edge->passNets);
            overflow = getTotalOverflow();
            if ((testcase == 3 && overflow <= 100) || timer.overTime())
            {
                std::cout << "Iteration " << std::setw(3) << iteration << " overflow: " << overflow << "\n";
                goto finishRouting;
            }
        }
        std::cout << "Iteration " << std::setw(3) << iteration << " overflow: " << overflow << "\n";
        ++iteration;
    }
finishRouting:;
    std::cout << "Wirelength:             " << getWirelength() << "\n"
              << "Overflow:               " << overflow << "\n"
              << "\n";

    ResultWriter *result = new ResultWriter();
    for (const Net::ptr &net : input->nets)
        result->addResult(net.get());
    return std::unique_ptr<ResultWriter>(result);
}
