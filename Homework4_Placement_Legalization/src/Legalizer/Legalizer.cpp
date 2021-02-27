#include "Legalizer.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>

bool cmp(Cell const *A, Cell const *B)
{
    return A->x < B->x;
}

void Legalizer::divideRow()
{
    std::sort(input->terminals.begin(), input->terminals.end(), cmp);
    for (auto const &terminal : input->terminals)
    {
        int terminalMinX = terminal->x, terminalMaxX = terminal->x + terminal->width;
        for (auto const &row : input->rows)
        {
            if (row->y < terminal->y || row->y >= terminal->y + terminal->height)
                continue;

            auto lastSubRow = row->subRows.back();
            if (lastSubRow->minX < terminalMinX)
            {
                if (lastSubRow->maxX > terminalMaxX)
                    row->subRows.emplace_back(new SubRow(terminalMaxX, lastSubRow->maxX));
                lastSubRow->updateMinMax(lastSubRow->minX, terminalMinX);
            }
            else
            {
                if (lastSubRow->maxX > terminalMaxX)
                {
                    lastSubRow->updateMinMax(terminalMaxX, lastSubRow->maxX);
                }
                else
                {
                    delete lastSubRow;
                    row->subRows.pop_back();
                }
            }
        }
    }
}

int Legalizer::getRowIdx(Cell const *cell)
{
    double minY = std::numeric_limits<double>::max();
    for (size_t i = 0; i < input->rows.size(); ++i)
    {
        if (minY > std::abs(cell->y - input->rows[i]->y))
            minY = std::abs(cell->y - input->rows[i]->y);
        else
            return i;
    }
    return input->rows.size() - 1;
}

int Legalizer::getSubRowIdx(Row const *row, Cell const *cell)
{
    auto const &subRows = row->subRows;
    if (subRows.empty() == true)
        return -1;

    if (cell->x >= row->subRows.back()->maxX)
    {
        if (cell->width <= row->subRows.back()->freeWidth)
            return subRows.size() - 1;
    }
    else
    {
        for (size_t i = 0; i < subRows.size(); ++i)
        {
            auto subRow = row->subRows[i];
            if (cell->x >= subRow->maxX)
                continue;

            if (cell->x >= subRow->minX)
            {
                if (cell->width <= subRow->freeWidth)
                    return i;
                if (i + 1 < row->subRows.size() && cell->width <= row->subRows[i + 1]->freeWidth)
                    return i + 1;
            }
            else
            {
                if (i > 0)
                {
                    if (std::abs(cell->x + cell->width - row->subRows[i - 1]->maxX) < std::abs(cell->x - subRow->minX))
                        if (cell->width <= row->subRows[i - 1]->freeWidth)
                            return i - 1;
                    if (cell->width <= subRow->freeWidth)
                        return i;
                }
                else
                {
                    if (cell->width <= subRow->freeWidth)
                        return 0;
                }
            }
        }
    }
    return -1;
}

int Legalizer::placeRowTrail(int const &rowIdx, Cell *cell)
{
    auto const &row = input->rows.at(rowIdx);
    int subRowIdx = getSubRowIdx(row, cell);
    if (subRowIdx == -1)
    {
        cell->optimalX = cell->optimalY = std::numeric_limits<double>::max();
        return -1;
    }

    auto const &subRow = row->subRows[subRowIdx];
    double optimalX = cell->x;
    if (cell->x < subRow->minX)
        optimalX = subRow->minX;
    else if (cell->x > subRow->maxX - cell->width)
        optimalX = subRow->maxX - cell->width;

    auto cluster = subRow->lastCluster;
    if (cluster == nullptr || cluster->x + cluster->width <= optimalX)
    {
        cell->optimalX = optimalX;
    }
    else
    {
        int trialWeight = cluster->weight + cell->weight;
        double trialQ = cluster->q + cell->weight * (optimalX - cluster->width);
        int trialWidth = cluster->width + cell->width;

        double trialX = 0;
        while (1)
        {
            trialX = trialQ / trialWeight;

            if (trialX < subRow->minX)
                trialX = subRow->minX;
            if (trialX > subRow->maxX - trialWidth)
                trialX = subRow->maxX - trialWidth;

            auto const &prevCluster = cluster->predecessor;
            if (prevCluster != nullptr && prevCluster->x + prevCluster->width > trialX)
            {
                trialQ = prevCluster->q + trialQ - trialWeight * prevCluster->width;
                trialWeight = prevCluster->weight + trialWeight;
                trialWidth = prevCluster->width + trialWidth;

                cluster = prevCluster;
            }
            else
            {
                break;
            }
        }
        cell->optimalX = trialX + trialWidth - cell->width;
    }
    cell->optimalY = row->y;
    return subRowIdx;
}

void Legalizer::placeRowFinal(int const &rowIdx, int const &subRowIdx, Cell *cell)
{
    auto subRow = input->rows[rowIdx]->subRows[subRowIdx];
    subRow->freeWidth -= cell->width;

    double optimalX = cell->x;
    if (cell->x < subRow->minX)
        optimalX = subRow->minX;
    else if (cell->x > subRow->maxX - cell->width)
        optimalX = subRow->maxX - cell->width;

    auto cluster = subRow->lastCluster;
    if (cluster == nullptr || cluster->x + cluster->width <= optimalX)
    {
        cluster = new Cluster(optimalX,
                              subRow->lastCluster,
                              cell->weight,
                              cell->weight * optimalX,
                              cell->width);
        subRow->lastCluster = cluster;
        cluster->member.emplace_back(cell);
    }
    else
    {
        cluster->member.emplace_back(cell);
        cluster->weight += cell->weight;
        cluster->q += cell->weight * (optimalX - cluster->width);
        cluster->width += cell->width;

        while (true)
        {
            cluster->x = cluster->q / cluster->weight;

            if (cluster->x < subRow->minX)
                cluster->x = subRow->minX;
            if (cluster->x > subRow->maxX - cluster->width)
                cluster->x = subRow->maxX - cluster->width;

            auto prevCluster = cluster->predecessor;
            if (prevCluster != nullptr && prevCluster->x + prevCluster->width > cluster->x)
            {
                prevCluster->member.insert(prevCluster->member.end(), cluster->member.begin(), cluster->member.end());
                prevCluster->weight += cluster->weight;
                prevCluster->q += cluster->q - cluster->weight * prevCluster->width;
                prevCluster->width += cluster->width;

                delete cluster;
                cluster = prevCluster;
            }
            else
            {
                break;
            }
        }
        subRow->lastCluster = cluster;
    }
}

double Legalizer::calCost(Cell const *cell)
{
    if (cell->optimalX == std::numeric_limits<double>::max() ||
        cell->optimalY == std::numeric_limits<double>::max())
        return std::numeric_limits<double>::max();

    double x = cell->optimalX - cell->x,
           y = cell->optimalY - cell->y;
    return std::sqrt(x * x + y * y);
}

void Legalizer::determinePosition()
{
    for (auto const &row : input->rows)
    {
        int siteWidth = row->width;
        for (auto const &subRow : row->subRows)
        {
            auto cluster = subRow->lastCluster;
            while (cluster != nullptr)
            {
                double shiftX = cluster->x - subRow->minX;
                if (shiftX - std::floor(shiftX / siteWidth) * siteWidth <= siteWidth / 2.0)
                    cluster->x = std::floor(shiftX / siteWidth) * siteWidth + subRow->minX;
                else
                    cluster->x = std::ceil(shiftX / siteWidth) * siteWidth + subRow->minX;

                int optimalX = cluster->x;
                for (auto &cell : cluster->member)
                {
                    cell->optimalX = optimalX;
                    cell->optimalY = row->y;
                    optimalX += cell->width;
                }
                cluster = cluster->predecessor;
            }
        }
    }
}

void Legalizer::abacusProcess()
{
    std::sort(input->cells.begin(), input->cells.end(), cmp);
    for (auto const &cell : input->cells)
    {
        int bestRowIdx = getRowIdx(cell),
            bestSubRowIdx = placeRowTrail(bestRowIdx, cell);
        double bestCost = calCost(cell);
        size_t downFinder = bestRowIdx, upFinder = bestRowIdx;

        while (downFinder > 0 && std::abs(cell->y - input->rows[downFinder]->y) < bestCost)
        {
            downFinder -= 1;
            int subRowIdx = placeRowTrail(downFinder, cell);
            double cost = calCost(cell);
            if (cost < bestCost)
            {
                bestRowIdx = downFinder;
                bestSubRowIdx = subRowIdx;
                bestCost = cost;
            }
        }

        while (upFinder < input->rows.size() - 1 && std::abs(cell->y - input->rows[upFinder]->y) < bestCost)
        {
            upFinder += 1;
            int subRowIdx = placeRowTrail(upFinder, cell);
            double cost = calCost(cell);
            if (cost < bestCost)
            {
                bestRowIdx = upFinder;
                bestSubRowIdx = subRowIdx;
                bestCost = cost;
            }
        }

        placeRowFinal(bestRowIdx, bestSubRowIdx, cell);
    }
    determinePosition();
}

void Legalizer::calDisplacement()
{
    double sum = 0, max = 0;
    for (auto const &cell : input->cells)
    {
        double x = cell->optimalX - cell->x,
               y = cell->optimalY - cell->y,
               dis = std::sqrt(x * x + y * y);
        sum += dis;
        if (max < dis)
            max = dis;
    }
    printf("total displacement: %.2f\n", sum);
    printf("max displacement:   %.2f\n", max);
}

ResultWriter *Legalizer::solve()
{
    divideRow();
    abacusProcess();
    calDisplacement();

    return new ResultWriter(input);
}
