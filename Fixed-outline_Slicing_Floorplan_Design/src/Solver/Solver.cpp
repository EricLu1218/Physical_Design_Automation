#include "Solver.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stack>

std::vector<int> Solver::getInitialExpression() const
{
    std::vector<int> expression;
    int rowWidth = 0, hCnt = 0, vCnt = 0;
    for (size_t i = 0; i < input->hardblocks.size(); ++i)
    {
        rowWidth += input->hardblocks[i]->width;
        if (rowWidth >= outline)
        {
            rowWidth = input->hardblocks[i]->width;
            vCnt = 0;
            if (++hCnt == 2)
            {
                expression.emplace_back(Node::HORIZONTAL_CUT);
                hCnt = 1;
            }
        }

        expression.emplace_back(i);
        if (++vCnt == 2)
        {
            expression.emplace_back(Node::VERTICAL_CUT);
            vCnt = 1;
        }
    }
    if (++hCnt == 2)
        expression.emplace_back(Node::HORIZONTAL_CUT);
    return expression;
}

bool Solver::isCut(int id) const
{
    return id < Node::HARDBLOCK;
}

bool Solver::isSkewed(const std::vector<int> &expression, size_t idx) const
{
    if (isCut(expression[idx]))
    {
        if (idx + 2 < expression.size() && expression[idx] == expression[idx + 2])
            return false;
    }
    else if (isCut(expression[idx + 1]))
    {
        if (idx > 0 && expression[idx - 1] == expression[idx + 1])
            return false;
    }
    return true;
}

bool Solver::satisfyBallot(const std::vector<int> &expression, size_t idx) const
{
    if (isCut(expression[idx + 1]))
    {
        size_t cutCnt = 1;
        for (size_t i = 0; i < idx + 1; ++i)
            if (isCut(expression[i]))
                ++cutCnt;
        if (2 * cutCnt >= idx + 1)
            return false;
    }
    return true;
}

std::vector<int> Solver::perturb(std::vector<int> expression, int type) const
{
    if (type == 0)
    {
        std::vector<size_t> indices;
        for (size_t i = 0; i < expression.size(); ++i)
            if (!isCut(expression[i]))
                indices.emplace_back(i);

        size_t r1 = rand() % indices.size();
        size_t r2 = rand() % indices.size();
        while (r1 == r2)
            r2 = rand() % indices.size();
        std::swap(expression[indices[r1]], expression[indices[r2]]);
    }
    else if (type == 1)
    {
        std::vector<size_t> indices;
        for (size_t i = 1; i < expression.size(); ++i)
            if (!isCut(expression[i - 1]) && isCut(expression[i]))
                indices.emplace_back(i);

        size_t r = rand() % indices.size();
        for (size_t i = indices[r]; i < expression.size(); ++i)
        {
            if (!isCut(expression[i]))
                break;
            expression[i] = (expression[i] == Node::HORIZONTAL_CUT) ? Node::VERTICAL_CUT : Node::HORIZONTAL_CUT;
        }
    }
    else if (type == 2)
    {
        std::vector<size_t> indices;
        for (size_t i = 0; i + 1 < expression.size(); ++i)
            if ((!isCut(expression[i]) && isCut(expression[i + 1])) ||
                (isCut(expression[i]) && !isCut(expression[i + 1])))
                indices.emplace_back(i);

        while (!indices.empty())
        {
            size_t r = rand() % indices.size();
            if (isSkewed(expression, indices[r]) && satisfyBallot(expression, indices[r]))
            {
                std::swap(expression[indices[r]], expression[indices[r] + 1]);
                break;
            }
            else
            {
                std::swap(indices[r], indices.back());
                indices.pop_back();
            }
        }
    }
    return expression;
}

Node *Solver::buildSlicingTree(const std::vector<int> &expression)
{
    size_t cutIdx = 0;
    std::stack<Node *> nodeStack;
    for (int id : expression)
    {
        if (!isCut(id))
        {
            nodeStack.emplace(hardblockNodes[id].get());
        }
        else
        {
            Node::ptr &cutNode = cutNodes[cutIdx++];
            cutNode->type = id;
            cutNode->rchild = nodeStack.top();
            nodeStack.pop();
            cutNode->lchild = nodeStack.top();
            nodeStack.pop();
            cutNode->updateRecord();
            nodeStack.emplace(cutNode.get());
        }
    }
    return nodeStack.top();
}

void Solver::setPosition(Node *node, size_t choice, int x, int y)
{
    const Node::Record &record = node->records[choice];
    if (node->type == Node::HARDBLOCK)
    {
        node->hardblock->update(record.width, record.height, x, y);
    }
    else
    {
        setPosition(node->lchild, record.leftChoice, x, y);
        if (node->type == Node::HORIZONTAL_CUT)
            y += node->lchild->records[record.leftChoice].height;
        else
            x += node->lchild->records[record.leftChoice].width;
        setPosition(node->rchild, record.rightChoice, x, y);
    }
}

int Solver::getCost(const std::vector<int> &expression, bool withWirelength)
{
    Node *root = buildSlicingTree(expression);
    int minAreaCost = std::numeric_limits<int>::max();
    size_t choice = 0;
    for (size_t i = 0; i < root->records.size(); ++i)
    {
        int areaCost = 0;
        Node::Record &record = root->records[i];
        if (record.width > outline && record.height > outline)
            areaCost = record.width * record.height - outline * outline;
        else if (record.width > outline)
            areaCost = (record.width - outline) * outline;
        else if (record.height > outline)
            areaCost = outline * (record.height - outline);

        if (minAreaCost > areaCost)
        {
            minAreaCost = areaCost;
            choice = i;
        }
    }

    int wirelength = 0;
    if (withWirelength)
    {
        setPosition(root, choice, 0, 0);
        for (const Net::ptr &net : input->nets)
            wirelength += net->wirelength();
    }
    return minAreaCost * 10 + wirelength;
}

int Solver::getWirelength(const std::vector<int> &expression)
{
    Node *root = buildSlicingTree(expression);
    size_t choice = 0;
    for (size_t i = 0; i < root->records.size(); ++i)
    {
        const Node::Record &record = root->records[i];
        if (record.width <= outline && record.height <= outline)
        {
            choice = i;
            break;
        }
    }

    setPosition(root, choice, 0, 0);
    int wirelength = 0;
    for (const Net::ptr &net : input->nets)
        wirelength += net->wirelength();
    return wirelength;
}

// refer to the pseudo-code of the class lecture
std::pair<std::vector<int>, int> Solver::simulatedAnnealing(std::vector<int> expression, bool withWirelength,
                                                            double initTemperature, double minTemperature,
                                                            double coolingCoefficient, int tryingTimes,
                                                            double maxRejectRatio)
{
    int cost = getCost(expression, withWirelength);
    std::vector<int> bestExpression = expression;
    int bestCost = cost;
    if (bestCost == 0)
        return {bestExpression, bestCost};

    double temperature = initTemperature;
    int tryingCnt = 0, uphillCnt = 0, rejectCnt = 0;
    int maxTryingCnt = tryingTimes * input->hardblocks.size();
    do
    {
        tryingCnt = uphillCnt = rejectCnt = 0;
        do
        {
            if (timer.overTime())
                return {bestExpression, bestCost};

            int type = (withWirelength) ? 0 : rand() % 3;
            std::vector<int> newExpression = perturb(expression, type);

            ++tryingCnt;
            int newCost = getCost(newExpression, withWirelength);
            int deltaCost = newCost - cost;
            if (deltaCost < 0 || static_cast<double>(rand()) / RAND_MAX < exp(-deltaCost / temperature))
            {
                if (deltaCost > 0)
                    ++uphillCnt;

                expression = newExpression;
                cost = newCost;
                if (cost < bestCost)
                {
                    bestExpression = expression;
                    bestCost = cost;
                    if (bestCost == 0)
                        return {bestExpression, bestCost};
                }
            }
            else
            {
                ++rejectCnt;
            }
        } while (uphillCnt <= maxTryingCnt && tryingCnt <= 2 * maxTryingCnt);
        temperature *= coolingCoefficient;
    } while (static_cast<double>(rejectCnt) / tryingCnt <= maxRejectRatio && temperature >= minTemperature);
    return {bestExpression, bestCost};
}

Solver::Solver(Input *input, Timer &timer) : input(input), timer(timer)
{
    int totalArea = 0;
    for (size_t i = 0; i < input->hardblocks.size(); ++i)
    {
        const Hardblock::ptr &hardblock = input->hardblocks[i];
        hardblockNodes.emplace_back(new Node(Node::HARDBLOCK, hardblock.get()));
        totalArea += hardblock->width * hardblock->height;
    }
    for (size_t i = 1; i < input->hardblocks.size(); ++i)
        cutNodes.emplace_back(new Node(Node::HORIZONTAL_CUT));

    outline = sqrt(totalArea * (1 + input->deadspaceRatio));
    std::cout << "----- DESIGN INFORMATION -----\n"
              << "#hardblock:              " << input->hardblocks.size() << "\n"
              << "#fixed-pin:              " << input->fixedPins.size() << "\n"
              << "#net:                    " << input->nets.size() << "\n"
              << "Deadspace ratio:         " << input->deadspaceRatio << "\n"
              << "Floorplan width(height): " << outline << "\n"
              << "\n";
}

ResultWriter::ptr Solver::solve()
{
    // can set the random seed for different testcases
    int seed = 0;
    srand(seed);

    std::vector<int> expression = getInitialExpression();
    int cost = getCost(expression, false);
    std::cout << "-------- SA FOR AREA --------\n";
    while (cost != 0)
        std::tie(expression, cost) = simulatedAnnealing(expression, false, 1000, 0.1, 0.9, 10, 1);
    int wirelength = getWirelength(expression);
    std::cout << "A feasible solution is found!\n"
              << "Wirelength: " << wirelength << "\n"
              << "\n";

    std::cout << "----- SA FOR WIRELENGTH -----\n";
    std::tie(expression, cost) = simulatedAnnealing(expression, true, 1000, 1, 0.95, 5, 1);
    wirelength = getWirelength(expression);
    std::cout << "A minimum wirelength solution is found!\n"
              << "Wirelength: " << wirelength << "\n"
              << "\n";

    ResultWriter *result = new ResultWriter();
    result->assignWirelength(wirelength);
    for (const Hardblock::ptr &hardblock : input->hardblocks)
        result->addHardblock(hardblock.get());
    return std::unique_ptr<ResultWriter>(result);
}
