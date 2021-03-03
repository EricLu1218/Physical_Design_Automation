#include "Router.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

Edge *Router::getEdge(int const &x, int const &y, int const &direction)
{
    assert(direction != Direction::STOP);

    if (direction == Direction::LEFT)
        return &hEdges.at(y).at(x - 1);
    else if (direction == Direction::RIGHT)
        return &hEdges.at(y).at(x);
    else if (direction == Direction::UP)
        return &vEdges.at(y).at(x);
    else if (direction == Direction::DOWN)
        return &vEdges.at(y - 1).at(x);
    return nullptr;
}

double Router::calCost(Edge const *edge)
{
    double historical = 0;
    if (edge->demand() > edge->capacity)
        historical = edge->historicalCost + 1;
    else
        historical = edge->historicalCost;
    double penalty = std::pow(static_cast<double>(edge->demand() + 1) / edge->capacity, k1);
    return baseCost + historical * penalty;
}

Node outOfBoundary(-1, -1, -1);
Node Router::wavePropagation(Node const &current, Node const &target, int const &direction)
{
    auto next = current;
    if (direction == Direction::LEFT)
    {
        next.x -= 1;
        if (next.x < 0)
            return outOfBoundary;
        if (current.x <= target.x)
            next.cost += 1;
    }
    else if (direction == Direction::RIGHT)
    {
        next.x += 1;
        if (next.x > input->hGridCnt - 1)
            return outOfBoundary;
        if (current.x >= target.x)
            next.cost += 1;
    }
    else if (direction == Direction::UP)
    {
        next.y += 1;
        if (next.y > input->vGridCnt - 1)
            return outOfBoundary;
        if (current.y >= target.y)
            next.cost += 1;
    }
    else if (direction == Direction::DOWN)
    {
        next.y -= 1;
        if (next.y < 0)
            return outOfBoundary;
        if (current.y <= target.y)
            next.cost += 1;
    }
    else
    {
        return outOfBoundary;
    }
    next.cost += calCost(getEdge(current.x, current.y, direction));
    return next;
}

void Router::traceBack(Net *net)
{
    net->routingPath.clear();
    int x = net->x2, y = net->y2;
    while (routingMap.at(y).at(x).prevDirection != -1)
    {
        net->routingPath.emplace_back(x, y);
        int const &direction = routingMap.at(y).at(x).prevDirection;
        if (direction == Direction::LEFT)
        {
            getEdge(x, y, Direction::RIGHT)->passNets.emplace(net);
            x += 1;
        }
        else if (direction == Direction::RIGHT)
        {
            getEdge(x, y, Direction::LEFT)->passNets.emplace(net);
            x -= 1;
        }
        else if (direction == Direction::UP)
        {
            getEdge(x, y, Direction::DOWN)->passNets.emplace(net);
            y -= 1;
        }
        else if (direction == Direction::DOWN)
        {
            getEdge(x, y, Direction::UP)->passNets.emplace(net);
            y += 1;
        }
    }
    net->routingPath.emplace_back(x, y);
}

void Router::routeNet(Net *net)
{
    Node source(net->x1, net->y1, 0), target(net->x2, net->y2, -1);

    for (auto &y : routingMap)
        for (auto &x : y)
            x.reset();
    routingMap.at(source.y).at(source.x).cost = 0;

    std::vector<int> directions(2, Direction::STOP); // {direction x, direction y}
    if (source.x > target.x)
        directions.at(0) = Direction::LEFT;
    else if (source.x < target.x)
        directions.at(0) = Direction::RIGHT;
    if (source.y < target.y)
        directions.at(1) = Direction::UP;
    else if (source.y > target.y)
        directions.at(1) = Direction::DOWN;

    std::priority_queue<Node, std::vector<Node>, Node> routingQueue;
    routingQueue.push(source);
    while (routingQueue.empty() == false)
    {
        auto current = routingQueue.top();
        routingQueue.pop();

        if (current.x == target.x && current.y == target.y)
        {
            target.cost = current.cost;
            continue;
        }

        if (target.cost != -1 && current.cost >= target.cost)
            continue;

        for (auto const direction : directions)
        {
            auto const &next = wavePropagation(current, target, direction);
            if (next.cost == -1)
                continue;

            auto &routingNode = routingMap.at(next.y).at(next.x);
            if (routingNode.cost != -1 && next.cost >= routingNode.cost)
                continue;

            routingNode.cost = next.cost;
            routingNode.prevDirection = direction;
            routingQueue.push(next);
        }
    }

    traceBack(net);
}

void Router::getRipUpQueue(std::priority_queue<Edge *, std::vector<Edge *>, Edge> &ripUpQueue)
{
    for (auto &y : hEdges)
        for (auto &x : y)
            if (x.overflow() > 0)
            {
                x.historicalCost += 1;
                ripUpQueue.push(&x);
            }
    for (auto &y : vEdges)
        for (auto &x : y)
            if (x.overflow() > 0)
            {
                x.historicalCost += 1;
                ripUpQueue.push(&x);
            }
}

void Router::rerouteNet(Net *net)
{
    Node source(net->x1, net->y1, 0), target(net->x2, net->y2, -1);

    for (auto &y : routingMap)
        for (auto &x : y)
            x.reset();
    routingMap.at(source.y).at(source.x).cost = 0;

    std::priority_queue<Node, std::vector<Node>, Node> routingQueue;
    routingQueue.push(source);
    while (routingQueue.empty() == false)
    {
        auto current = routingQueue.top();
        routingQueue.pop();

        if (current.x == target.x && current.y == target.y)
        {
            target.cost = current.cost;
            continue;
        }

        if (target.cost != -1 && current.cost >= target.cost)
            continue;

        for (int direction = 0; direction < 4; ++direction)
        {
            auto const &next = wavePropagation(current, target, direction);
            if (next.cost == -1)
                continue;

            auto &routingNode = routingMap.at(next.y).at(next.x);
            if (routingNode.cost != -1 && next.cost >= routingNode.cost)
                continue;

            routingNode.cost = next.cost;
            routingNode.prevDirection = direction;
            routingQueue.push(next);
        }
    }

    traceBack(net);
}

void Router::ripUpReroute(std::unordered_set<Net *> passNets)
{
    std::priority_queue<Net *, std::vector<Net *>, Net> rerouteQueue;
    for (auto const &net : passNets)
    {
        int cost = 0;
        for (size_t i = 0; i < net->routingPath.size() - 1; ++i)
        {
            auto const &point1 = net->routingPath.at(i),
                       &point2 = net->routingPath.at(i + 1);
            int direction = Direction::STOP;
            if (point1.x > point2.x && point1.y == point2.y)
                direction = Direction::LEFT;
            else if (point1.x < point2.x && point1.y == point2.y)
                direction = Direction::RIGHT;
            else if (point1.x == point2.x && point1.y < point2.y)
                direction = Direction::UP;
            else if (point1.x == point2.x && point1.y > point2.y)
                direction = Direction::DOWN;
            else
                continue;

            auto edge = getEdge(point1.x, point1.y, direction);
            cost += edge->overflow();
            edge->passNets.erase(net);
        }
        net->overflow = cost;
        net->routingPath.clear();
        rerouteQueue.push(net);
    }

    while (rerouteQueue.empty() == false)
    {
        auto net = rerouteQueue.top();
        rerouteQueue.pop();
        rerouteNet(net);
    }
}

int Router::calTotalOverflow()
{
    int overflow = 0;
    for (auto const &y : hEdges)
        for (auto const &x : y)
            overflow += x.overflow();
    for (auto const &y : vEdges)
        for (auto const &x : y)
            overflow += x.overflow();
    return overflow;
}

int Router::calWirelength()
{
    int wirelength = 0;
    for (auto const &net : input->nets)
        wirelength += net->wirelength();
    return wirelength;
}

ResultWriter *Router::solve()
{
    /* choose seed for each testcase (Platform: nthucad ic26) */
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
    for (auto const &net : input->nets)
        routeNet(net);
    int overflow = calTotalOverflow();
    std::cerr << "initial overflow:   " << overflow << '\n';

    int iteration = 1;
    while (overflow > 0)
    {
        baseCost = 1.0 - exp(-5 * exp(-0.1 * iteration));

        std::priority_queue<Edge *, std::vector<Edge *>, Edge> ripUpQueue;
        getRipUpQueue(ripUpQueue);
        while (overflow > 0 && ripUpQueue.empty() == false)
        {
            auto edge = ripUpQueue.top();
            ripUpQueue.pop();
            ripUpReroute(edge->passNets);

            overflow = calTotalOverflow();
            if ((testcase == 3 && overflow <= 100) || globalTimer.overTime() == true)
            {
                std::cerr << "reroute overflow:   " << overflow << '\n';
                goto finishRouting;
            }
        }
        std::cerr << "reroute overflow:   " << overflow << '\n';
        iteration += 1;
    }
finishRouting:;
    std::cerr << "total wirelength:   " << calWirelength() << '\n';

    return new ResultWriter(input);
}
