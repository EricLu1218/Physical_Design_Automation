#include "Legalizer.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stack>
#include <vector>

void Legalizer::divideRow()
{
    std::sort(input->blockages.begin(), input->blockages.end(), [](const Cell::ptr &a, const Cell::ptr &b)
              { return a->x < b->x; });

    for (const auto &blockage : input->blockages)
    {
        int blockageMinX = blockage->x;
        int blockageMaxX = blockage->x + blockage->width;
        int blockageMinY = blockage->y;
        int blockageMaxY = blockage->y + blockage->height;
        for (auto &row : input->rows)
        {
            if (row->y + row->height <= blockageMinY || blockageMaxY <= row->y)
                continue;

            auto lastSubRow = row->subRows.back().get();
            if (lastSubRow->minX < blockageMinX)
            {
                if (blockageMaxX < lastSubRow->maxX)
                    row->subRows.emplace_back(new SubRow(blockageMaxX, lastSubRow->maxX));
                lastSubRow->updateMinMax(lastSubRow->minX, blockageMinX);
            }
            else
            {
                if (blockageMaxX < lastSubRow->maxX)
                    lastSubRow->updateMinMax(blockageMaxX, lastSubRow->maxX);
                else
                    row->subRows.pop_back();
            }
        }
    }
}

int Legalizer::getRowIdx(const Cell *cell) const
{
    int rowIdx = -1;
    double minDisplacement = std::numeric_limits<double>::max();
    for (size_t i = 0; i < input->rows.size(); ++i)
    {
        double displacement = std::abs(cell->y - input->rows[i]->y);
        if (minDisplacement > displacement)
        {
            minDisplacement = displacement;
            rowIdx = i;
        }
        else
        {
            break;
        }
    }
    return rowIdx;
}

int Legalizer::getSubRowIdx(const Row *row, const Cell *cell) const
{
    const auto &subRows = row->subRows;
    std::vector<size_t> candidateSubRowIdx;
    for (size_t i = 0; i < subRows.size(); ++i)
        if (cell->width <= subRows[i]->freeWidth)
            candidateSubRowIdx.emplace_back(i);

    auto getMinDisplacement = [&](size_t idx) -> double
    {
        double displacement = 0;
        if (cell->x < subRows[idx]->minX)
            displacement = subRows[idx]->minX - cell->x;
        else if (cell->x + cell->width > subRows[idx]->maxX)
            displacement = cell->x + cell->width - subRows[idx]->maxX;
        return displacement;
    };

    int subRowIdx = -1;
    double minDisplacement = std::numeric_limits<double>::max();
    for (auto idx : candidateSubRowIdx)
    {
        auto displacement = getMinDisplacement(idx);
        if (minDisplacement > displacement)
        {
            minDisplacement = displacement;
            subRowIdx = idx;
        }
        else
        {
            break;
        }
    }
    return subRowIdx;
}

double Legalizer::getSiteX(double x, int minX, int siteWidth) const
{
    double shiftX = x - minX;
    return minX + std::round(shiftX / siteWidth) * siteWidth;
}

std::pair<int, double> Legalizer::placeRowTrial(const Row *row, Cell *cell, bool addPenalty)
{
    int subRowIdx = getSubRowIdx(row, cell);
    if (subRowIdx == -1)
        return {-1, std::numeric_limits<double>::max()};

    const auto &subRow = row->subRows[subRowIdx];
    double cellX = cell->x;
    if (cell->x < subRow->minX)
        cellX = subRow->minX;
    else if (cell->x > subRow->maxX - cell->width)
        cellX = subRow->maxX - cell->width;

    auto cluster = subRow->lastCluster;
    if (!cluster || cluster->x + cluster->width <= cellX)
    {
        cell->optimalX = cellX;
        cell->optimalY = row->y;
    }
    else
    {
        // add cell
        int clusterWeight = cluster->weight + cell->weight;
        double clusterQ = cluster->q + cell->weight * (cellX - cluster->width);
        int clusterWidth = cluster->width + cell->width;

        std::stack<Cluster::ptr> clusterStack;
        double clusterX = 0;
        // collapse cluster
        while (true)
        {
            clusterStack.push(cluster);
            clusterX = clusterQ / clusterWeight;
            if (clusterX < subRow->minX)
                clusterX = subRow->minX;
            if (clusterX > subRow->maxX - clusterWidth)
                clusterX = subRow->maxX - clusterWidth;

            const auto prevCluster = cluster->predecessor;
            if (prevCluster && prevCluster->x + prevCluster->width > clusterX)
            {
                // add cluster
                // (Update "q" then update "weight" since "weight" is used in both calculations.)
                clusterQ = prevCluster->q + clusterQ - clusterWeight * prevCluster->width;
                clusterWeight = prevCluster->weight + clusterWeight;
                clusterWidth = prevCluster->width + clusterWidth;

                cluster = prevCluster;
            }
            else
            {
                break;
            }
        }
        cell->optimalX = clusterX + clusterWidth - cell->width;
        cell->optimalY = row->y;

        if (addPenalty)
        {
            // check if violates the max displacement constraint after adding a cell to the cluster
            int x = getSiteX(clusterX, subRow->minX, row->siteWidth);
            while (!clusterStack.empty())
            {
                for (auto cell : clusterStack.top()->member)
                {
                    double copyX = cell->optimalX;
                    cell->optimalX = x;
                    if (cell->displacement() > maxDisplacementConstraint)
                    {
                        cell->optimalX = copyX;
                        return {-1, std::numeric_limits<double>::max()};
                    }
                    cell->optimalX = copyX;
                    x += cell->width;
                }
                clusterStack.pop();
            }
        }
    }
    return {subRowIdx, cell->displacement()};
}

void Legalizer::placeRowFinal(SubRow *subRow, Cell *cell)
{
    subRow->freeWidth -= cell->width;

    double cellX = cell->x;
    if (cell->x < subRow->minX)
        cellX = subRow->minX;
    else if (cell->x > subRow->maxX - cell->width)
        cellX = subRow->maxX - cell->width;

    auto cluster = subRow->lastCluster;
    if (!cluster || cluster->x + cluster->width <= cellX)
    {
        subRow->lastCluster = Cluster::ptr(new Cluster(cellX, cluster));
        cluster = subRow->lastCluster;

        // add cell
        cluster->member.emplace_back(cell);
        cluster->weight += cell->weight;
        cluster->q += cell->weight * (cellX - cluster->width);
        cluster->width += cell->width;
    }
    else
    {
        // add cell
        cluster->member.emplace_back(cell);
        cluster->weight += cell->weight;
        cluster->q += cell->weight * (cellX - cluster->width);
        cluster->width += cell->width;

        // collapse cluster
        while (true)
        {
            cluster->x = cluster->q / cluster->weight;
            if (cluster->x < subRow->minX)
                cluster->x = subRow->minX;
            if (cluster->x > subRow->maxX - cluster->width)
                cluster->x = subRow->maxX - cluster->width;

            auto prevCluster = cluster->predecessor;
            if (prevCluster && prevCluster->x + prevCluster->width > cluster->x)
            {
                // add cluster
                prevCluster->member.insert(prevCluster->member.end(), cluster->member.begin(), cluster->member.end());
                prevCluster->weight += cluster->weight;
                prevCluster->q += cluster->q - cluster->weight * prevCluster->width;
                prevCluster->width += cluster->width;

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

void Legalizer::abacusProcess()
{
    std::sort(input->cells.begin(), input->cells.end(), [](const Cell::ptr &a, const Cell::ptr &b)
              { return a->x < b->x; });

    for (const auto &cell : input->cells)
    {
        int baseRowIdx = getRowIdx(cell.get());
        int bestRowIdx = -1, bestSubRowIdx = -1;
        for (int addPenalty = 1; addPenalty >= 0; --addPenalty)
        {
            double bestCost = std::numeric_limits<double>::max();
            for (int rowIdx = baseRowIdx; rowIdx >= 0; --rowIdx)
            {
                if (std::abs(cell->y - input->rows[rowIdx]->y) >= bestCost)
                    break;

                auto [subRowIdx, cost] = placeRowTrial(input->rows[rowIdx].get(), cell.get(), addPenalty);
                if (cost < bestCost)
                {
                    bestCost = cost;
                    bestRowIdx = rowIdx;
                    bestSubRowIdx = subRowIdx;
                }
            }
            for (int rowIdx = baseRowIdx + 1; rowIdx < static_cast<int>(input->rows.size()); ++rowIdx)
            {
                if (std::abs(cell->y - input->rows[rowIdx]->y) >= bestCost)
                    break;

                auto [subRowIdx, cost] = placeRowTrial(input->rows[rowIdx].get(), cell.get(), addPenalty);
                if (cost < bestCost)
                {
                    bestCost = cost;
                    bestRowIdx = rowIdx;
                    bestSubRowIdx = subRowIdx;
                }
            }

            if (bestSubRowIdx != -1)
                break;
        }
        placeRowFinal(input->rows[bestRowIdx]->subRows[bestSubRowIdx].get(), cell.get());
    }
}

void Legalizer::determinePosition()
{
    for (const auto &row : input->rows)
    {
        for (const auto &subRow : row->subRows)
        {
            auto cluster = subRow->lastCluster;
            while (cluster)
            {
                int x = getSiteX(cluster->x, subRow->minX, row->siteWidth);
                for (auto cell : cluster->member)
                {
                    cell->optimalX = x;
                    cell->optimalY = row->y;
                    x += cell->width;
                }
                cluster = cluster->predecessor;
            }
        }
    }
}

std::pair<double, double> Legalizer::getTotalAndMaxDisplacement() const
{
    double totalDisplacement = 0, maxDisplacement = 0;
    for (const auto &cell : input->cells)
    {
        auto displacement = cell->displacement();
        totalDisplacement += displacement;
        if (maxDisplacement < displacement)
            maxDisplacement = displacement;
    }
    return {totalDisplacement, maxDisplacement};
}

Legalizer::Legalizer(Input *input)
    : input(input), maxDisplacementConstraint(input->maxDisplacementInSite * input->rows[0]->siteWidth)
{
    std::cout << "----- DESIGN INFORMATION -----\n"
              << "#row:                        " << input->rows.size() << "\n"
              << "#cell:                       " << input->cells.size() << "\n"
              << "#blockage:                   " << input->blockages.size() << "\n"
              << "Max displacement constraint: " << maxDisplacementConstraint << "\n"
              << "\n";
}

ResultWriter::ptr Legalizer::solve()
{
    divideRow();
    abacusProcess();
    determinePosition();
    auto [totalDisplacement, maxDisplacement] = getTotalAndMaxDisplacement();
    std::cout << "------- ABACUS RESULT --------\n"
              << "Total displacement:          " << totalDisplacement << "\n"
              << "Max displacement:            " << maxDisplacement << "\n"
              << "\n";

    auto result = new ResultWriter();
    for (const auto &cell : input->cells)
        result->addCell(cell.get());
    for (const auto &blockage : input->blockages)
        result->addBlockage(blockage.get());
    return std::unique_ptr<ResultWriter>(result);
}
