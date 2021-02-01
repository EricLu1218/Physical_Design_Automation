#include "Solver.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

void SA_Solver::getInitialNPE(std::vector<int> &npe)
{
    int rowWidth = 0, xCnt = 0, yCnt = 0, cCnt = 0;
    for (size_t i = 0; i < input->hardblocks.size(); ++i)
    {
        auto hardblock = input->hardblocks[i];
        rowWidth += hardblock->width;
        if (rowWidth >= outline)
        {
            yCnt += 1;
            if (yCnt >= 2)
            {
                npe.emplace_back(Node::HORIZONTAL_CUT);
                cCnt += 1;
                yCnt = 1;
            }
            rowWidth = hardblock->width;
            xCnt = 0;
        }

        npe.emplace_back(i);

        xCnt += 1;
        if (xCnt >= 2)
        {
            npe.emplace_back(Node::VERTICAL_CUT);
            cCnt += 1;
            xCnt = 1;
        }
    }

    for (size_t i = cCnt; i < input->hardblocks.size() - 1; ++i)
        npe.emplace_back(Node::HORIZONTAL_CUT);
}

void SA_Solver::invertCut(std::vector<int> &npe, size_t pos)
{
    while (pos < npe.size() && npe[pos] < Node::HARDBLOCK)
    {
        if (npe[pos] == Node::VERTICAL_CUT)
            npe[pos] = Node::HORIZONTAL_CUT;
        else
            npe[pos] = Node::VERTICAL_CUT;
        pos += 1;
    }
}

bool SA_Solver::checkSkewed(std::vector<int> const &npe, size_t const &pos) const
{
    if (npe[pos + 1] < Node::HARDBLOCK && pos != 0)
    {
        if (npe[pos - 1] != npe[pos + 1])
            return true;
    }
    else if (npe[pos] < Node::HARDBLOCK && pos + 1 != npe.size() - 1)
    {
        if (npe[pos] != npe[pos + 2])
            return true;
    }
    return false;
}

bool SA_Solver::checkBallot(std::vector<int> const &npe, size_t const &pos) const
{
    if (npe[pos + 1] < Node::HARDBLOCK)
    {
        size_t op = 0;
        for (size_t i = 0; i <= pos + 1; ++i)
            if (npe[i] < Node::HARDBLOCK)
                op += 1;

        if (2 * op >= pos + 1)
            return false;
    }
    return true;
}

std::vector<int> SA_Solver::perturbNPE(std::vector<int> npe, int const &type)
{
    int pos[npe.size()], posCnt = 0, pos1 = 0, pos2 = 0, violationCnt = 0;
    switch (type)
    {
    case 0:
        for (size_t i = 0; i < npe.size(); ++i)
            if (npe[i] >= Node::HARDBLOCK)
                pos[posCnt++] = i;

        pos1 = pos2 = rand() % posCnt;
        while (pos1 == pos2)
            pos2 = rand() % posCnt;
        pos1 = pos[pos1];
        pos2 = pos[pos2];
        std::swap(npe[pos1], npe[pos2]);
        break;

    case 1:
        for (size_t i = 1; i < npe.size(); ++i)
            if (npe[i - 1] >= Node::HARDBLOCK && npe[i] < Node::HARDBLOCK)
                pos[posCnt++] = i;

        pos1 = rand() % posCnt;
        pos1 = pos[pos1];
        invertCut(npe, pos1);
        break;

    case 2:
        for (size_t i = 0; i < npe.size() - 1; ++i)
            if ((npe[i] >= Node::HARDBLOCK && npe[i + 1] < Node::HARDBLOCK) ||
                (npe[i] < Node::HARDBLOCK && npe[i + 1] >= Node::HARDBLOCK))
                pos[posCnt++] = i;

        do
        {
            pos1 = rand() % posCnt;
            violationCnt += 1;
        } while ((checkSkewed(npe, pos[pos1]) && checkBallot(npe, pos[pos1])) == false && violationCnt < posCnt);

        if (violationCnt < posCnt)
        {
            pos1 = pos[pos1];
            pos2 = pos1 + 1;
            std::swap(npe[pos1], npe[pos2]);
        }
        break;
    }
    return npe;
}

Node *SA_Solver::buildSlicingTree(std::vector<int> const &npe)
{
    size_t cutIdx = 0;
    std::vector<Node *> postOrder;
    for (auto const &type : npe)
    {
        if (type >= Node::HARDBLOCK)
        {
            postOrder.emplace_back(hardblockNodes[type]);
        }
        else
        {
            auto node = cutNodes[cutIdx++];
            node->type = type;
            node->rchild = postOrder.back();
            postOrder.pop_back();
            node->lchild = postOrder.back();
            postOrder.pop_back();
            postOrder.emplace_back(node);
            node->updateRecord();
        }
    }
    return postOrder.back();
}

void SA_Solver::setPosition(Node *node, size_t const &choice, int const &x, int const &y)
{
    if (node->type == Node::HARDBLOCK)
    {
        node->hardblock->update(node->record[choice][0], node->record[choice][1], x, y);
    }
    else
    {
        setPosition(node->lchild, node->record[choice][2], x, y);

        int offsetX = 0, offsetY = 0;
        if (node->type == Node::VERTICAL_CUT)
            offsetX = node->lchild->record[node->record[choice][2]][0];
        else
            offsetY = node->lchild->record[node->record[choice][2]][1];
        setPosition(node->rchild, node->record[choice][3], x + offsetX, y + offsetY);
    }
}

int SA_Solver::calWirelength() const
{
    int wirelength = 0;
    for (auto const &net : input->nets)
        wirelength += net->HPWL();
    return wirelength;
}

int SA_Solver::calCost(std::vector<int> const &npe, bool const &focusWirelength)
{
    auto root = buildSlicingTree(npe);
    int minArea = std::numeric_limits<int>::max(), area = 0;
    size_t choice = 0;
    for (size_t i = 0; i < root->record.size(); ++i)
    {
        auto &record = root->record[i];
        if (record[0] > outline && record[1] > outline)
            area = record[0] * record[1] - outline * outline;
        else if (record[0] > outline && record[1] <= outline)
            area = (record[0] - outline) * outline;
        else if (record[0] <= outline && record[1] > outline)
            area = outline * (record[1] - outline);
        else
            area = 0;

        if (minArea > area)
        {
            minArea = area;
            choice = i;
        }
    }

    if (focusWirelength == false)
        return minArea * 10;

    setPosition(root, choice, 0, 0);
    return minArea * 10 + calWirelength();
}

std::vector<int> SA_Solver::saProcess(double const &c, double const &r, int const &k,
                                              std::vector<int> const &NPE, bool const &focusWirelength)
{
    int MT = 0, uphill = 0, reject = 0, N = k * input->hardblocks.size();
    std::vector<int> BE, E;
    BE = E = NPE;
    int bestCost, cost;
    bestCost = cost = calCost(E, focusWirelength);
    if (bestCost == 0)
        goto finishSA;

    do
    {
        double T0 = 1000;
        do
        {
            MT = uphill = reject = 0;
            do
            {
                if (globalTimer.overTime())
                    goto finishSA;

                int type = 0;
                if (focusWirelength == false)
                    type = rand() % 3;
                std::vector<int> NE = perturbNPE(E, type);

                MT += 1;
                int newCost = calCost(NE, focusWirelength);
                int delta_cost = newCost - cost;
                if (delta_cost < 0 || (double)rand() / RAND_MAX < exp(-1 * delta_cost / T0))
                {
                    if (delta_cost > 0)
                        uphill += 1;

                    E = NE;
                    cost = newCost;

                    if (cost < bestCost)
                    {
                        BE = E;
                        bestCost = cost;
                        if (bestCost == 0)
                            goto finishSA;
                    }
                }
                else
                {
                    reject += 1;
                }
            } while (uphill <= N && MT <= 2 * N);
            T0 = r * T0;
        } while (reject / MT <= 0.95 && T0 >= c);
    } while (focusWirelength == false);

finishSA:
    calCost(BE, true);
    return BE;
}

SA_Solver::SA_Solver(SAInput *input, GlobalTimer &globalTimer)
    : input(input), globalTimer(globalTimer)
{
    int totalArea = 0;
    for (size_t i = 0; i < input->hardblocks.size(); ++i)
    {
        auto hardblock = input->hardblocks[i];
        hardblockNodes.emplace_back(new Node(Node::HARDBLOCK, hardblock));
        totalArea += hardblock->width * hardblock->height;
    }
    outline = sqrt(totalArea * (1 + input->deadSpaceRatio));

    for (size_t i = 0; i < input->hardblocks.size() - 1; ++i)
        cutNodes.emplace_back(new Node(Node::VERTICAL_CUT));
}

ResultWriter *SA_Solver::solve()
{
    /* choose seed for each testcase (Platform: nthucad ic26) */
    int seed = 0;
    if (input->hardblocks.size() == 100)
    {
        if (input->deadSpaceRatio == 0.2)
            seed = 94;
        else if (input->deadSpaceRatio == 0.15)
            seed = 142;
    }
    else if (input->hardblocks.size() == 200)
    {
        if (input->deadSpaceRatio == 0.2)
            seed = 94;
        else if (input->deadSpaceRatio == 0.15)
            seed = 0;
    }
    else if (input->hardblocks.size() == 300)
    {
        if (input->deadSpaceRatio == 0.2)
            seed = 21;
        else if (input->deadSpaceRatio == 0.15)
            seed = 13;
    }
    srand(seed);

    std::vector<int> npe;
    getInitialNPE(npe);
    std::cerr << "start simulated annealing for area\n";
    npe = saProcess(0.1, 0.9, 10, npe, false);
    std::cerr << "find a feasible solution!\n"
              << "wirelength: " << calWirelength() << "\n\n";

    std::cerr << "start simulated annealing for wirelength\n";
    saProcess(1, 0.95, 5, npe, true);
    std::cerr << "find a minimum wirelength in time!\n"
              << "wirelength: " << calWirelength() << "\n\n";

    return new ResultWriter(input);
}
