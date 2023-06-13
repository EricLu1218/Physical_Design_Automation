#include "Solver.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

std::vector<int> Solver::generateInitialNpe() const
{
    std::vector<int> npe;
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
                npe.emplace_back(Node::HORIZONTAL_CUT);
                hCnt = 1;
            }
        }

        npe.emplace_back(i);
        if (++vCnt == 2)
        {
            npe.emplace_back(Node::VERTICAL_CUT);
            vCnt = 1;
        }
    }
    if (++hCnt == 2)
        npe.emplace_back(Node::HORIZONTAL_CUT);
    return npe;
}

bool Solver::isCut(int id) const
{
    return id < Node::HARDBLOCK;
}

bool Solver::isSkewed(const std::vector<int> &npe, size_t pos) const
{
    if (isCut(npe[pos]))
    {
        if (pos + 2 < npe.size() && npe[pos] == npe[pos + 2])
            return false;
    }
    else if (isCut(npe[pos + 1]))
    {
        if (pos > 0 && npe[pos - 1] == npe[pos + 1])
            return false;
    }
    return true;
}

bool Solver::satisfyBallot(const std::vector<int> &npe, size_t pos) const
{
    if (isCut(npe[pos + 1]))
    {
        size_t cutCnt = 0;
        for (size_t i = 0; i <= pos + 1; ++i)
            if (isCut(npe[i]))
                ++cutCnt;

        if (2 * cutCnt >= pos + 1)
            return false;
    }
    return true;
}

std::vector<int> Solver::perturbNpe(std::vector<int> npe, int type) const
{
    switch (type)
    {
    case 0:
    {
        std::vector<size_t> pos;
        for (size_t i = 0; i < npe.size(); ++i)
            if (!isCut(npe[i]))
                pos.emplace_back(i);

        size_t idx1 = rand() % pos.size();
        size_t idx2 = rand() % pos.size();
        while (idx1 == idx2)
            idx2 = rand() % pos.size();
        std::swap(npe[pos[idx1]], npe[pos[idx2]]);
        break;
    }
    case 1:
    {
        std::vector<size_t> pos;
        for (size_t i = 1; i < npe.size(); ++i)
            if (!isCut(npe[i - 1]) && isCut(npe[i]))
                pos.emplace_back(i);

        size_t i = pos[rand() % pos.size()];
        while (i < npe.size() && isCut(npe[i]))
        {
            if (npe[i] == Node::HORIZONTAL_CUT)
                npe[i] = Node::VERTICAL_CUT;
            else
                npe[i] = Node::HORIZONTAL_CUT;
            ++i;
        }
        break;
    }
    case 2:
    {
        std::vector<size_t> pos;
        for (size_t i = 0; i + 1 < npe.size(); ++i)
            if ((!isCut(npe[i]) && isCut(npe[i + 1])) || (isCut(npe[i]) && !isCut(npe[i + 1])))
                pos.emplace_back(i);

        size_t idx = rand() % pos.size(), violationCnt = 0;
        while (!isSkewed(npe, pos[idx]) || !satisfyBallot(npe, pos[idx]))
        {
            if (++violationCnt >= pos.size())
                break;

            idx = rand() % pos.size();
        }
        if (violationCnt < pos.size())
            std::swap(npe[pos[idx]], npe[pos[idx] + 1]);
        break;
    }
    }
    return npe;
}

Node *Solver::buildSlicingTree(const std::vector<int> &npe)
{
    size_t cutIdx = 0;
    std::vector<Node *> postOrder;
    for (const auto &id : npe)
    {
        if (!isCut(id))
        {
            postOrder.emplace_back(hardblockNodes[id].get());
        }
        else
        {
            auto &cutNode = cutNodes[cutIdx++];
            cutNode->type = id;
            cutNode->rchild = postOrder.back();
            postOrder.pop_back();
            cutNode->lchild = postOrder.back();
            postOrder.pop_back();
            cutNode->updateRecord();
            postOrder.emplace_back(cutNode.get());
        }
    }
    return postOrder.back();
}

void Solver::setPosition(Node *node, size_t choice, int x, int y)
{
    const auto &record = node->records[choice];
    if (node->type == Node::HARDBLOCK)
    {
        node->hardblock->update(record.width, record.height, x, y);
    }
    else
    {
        setPosition(node->lchild, record.leftChoice, x, y);

        int offsetX = 0, offsetY = 0;
        if (node->type == Node::HORIZONTAL_CUT)
            offsetY = node->lchild->records[record.leftChoice].height;
        else
            offsetX = node->lchild->records[record.leftChoice].width;
        setPosition(node->rchild, record.rightChoice, x + offsetX, y + offsetY);
    }
}

int Solver::getWirelength() const
{
    int wirelength = 0;
    for (const auto &net : input->nets)
        wirelength += net->wirelength();
    return wirelength;
}

int Solver::getCost(const std::vector<int> &npe, bool focusWirelength)
{
    auto root = buildSlicingTree(npe);
    int minArea = std::numeric_limits<int>::max();
    size_t choice = 0;
    for (size_t i = 0; i < root->records.size(); ++i)
    {
        int area = 0;
        auto &record = root->records[i];
        if (record.width > outline && record.height > outline)
            area = record.width * record.height - outline * outline;
        else if (record.width > outline && record.height <= outline)
            area = (record.width - outline) * outline;
        else if (record.width <= outline && record.height > outline)
            area = outline * (record.height - outline);
        else
            area = 0;

        if (minArea > area)
        {
            minArea = area;
            choice = i;
        }
    }

    int wirelength = 0;
    if (focusWirelength)
    {
        setPosition(root, choice, 0, 0);
        wirelength = getWirelength();
    }
    return minArea * 10 + wirelength;
}

std::vector<int> Solver::saProcess(double c, double r, int k, std::vector<int> E, bool focusWirelength)
{
    // refer to the pseudo-code of the class lecture
    std::vector<int> bestE = E;
    int bestCost = 0, cost = 0;
    bestCost = cost = getCost(E, focusWirelength);
    if (bestCost == 0)
        goto finishSA;

    do
    {
        int MT = 0, uphill = 0, reject = 0, N = k * input->hardblocks.size();
        double T = 1000;
        do
        {
            MT = uphill = reject = 0;
            do
            {
                if (timer.overTime())
                    goto finishSA;

                int type = 0;
                if (!focusWirelength)
                    type = rand() % 3;
                auto NE = perturbNpe(E, type);

                ++MT;
                int newCost = getCost(NE, focusWirelength);
                int deltaCost = newCost - cost;
                if (deltaCost < 0 || static_cast<double>(rand()) / RAND_MAX < exp(-deltaCost / T))
                {
                    if (deltaCost > 0)
                        ++uphill;

                    E = NE;
                    cost = newCost;
                    if (cost < bestCost)
                    {
                        bestE = E;
                        bestCost = cost;
                        if (bestCost == 0)
                            goto finishSA;
                    }
                }
                else
                {
                    ++reject;
                }
            } while (uphill <= N && MT <= 2 * N);
            T = r * T;
        } while (reject / MT <= 0.95 && T >= c);
    } while (!focusWirelength);

finishSA:
    getCost(bestE, true);
    return bestE;
}

Solver::Solver(Input *input, Timer &timer) : input(input), timer(timer)
{
    int totalArea = 0;
    for (size_t i = 0; i < input->hardblocks.size(); ++i)
    {
        const auto &hardblock = input->hardblocks[i];
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

    auto npe = generateInitialNpe();
    std::cout << "-------- SA FOR AREA --------\n";
    npe = saProcess(0.1, 0.9, 10, npe, false);
    std::cout << "A feasible solution is found!\n"
              << "Wirelength: " << getWirelength() << "\n"
              << "\n";

    std::cout << "----- SA FOR WIRELENGTH -----\n";
    saProcess(1, 0.95, 5, npe, true);
    std::cout << "A minimum wirelength solution is found!\n"
              << "Wirelength: " << getWirelength() << "\n"
              << "\n";

    auto result = new ResultWriter();
    result->assignWirelength(getWirelength());
    for (const auto &hardblock : input->hardblocks)
        result->addHardblock(hardblock.get());
    return std::unique_ptr<ResultWriter>(result);
}
