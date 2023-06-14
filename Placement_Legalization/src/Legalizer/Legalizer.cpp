#include "Legalizer.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
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
        if (cell->x + cell->width <= subRows[idx]->minX)
            displacement = subRows[idx]->minX - cell->x;
        else if (cell->x >= subRows[idx]->maxX)
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

int Legalizer::placeRowTrail(const Row *row, Cell *cell)
{
    int subRowIdx = getSubRowIdx(row, cell);
    if (subRowIdx == -1)
    {
        cell->optimalX = cell->optimalY = std::numeric_limits<double>::max();
        return -1;
    }

    const auto &subRow = row->subRows[subRowIdx];
    double optimalX = cell->x;
    if (cell->x < subRow->minX)
        optimalX = subRow->minX;
    else if (cell->x > subRow->maxX - cell->width)
        optimalX = subRow->maxX - cell->width;

    auto cluster = subRow->lastCluster;
    if (!cluster || cluster->x + cluster->width <= optimalX)
    {
        cell->optimalX = optimalX;
    }
    else
    {
        int trialWeight = cluster->weight + cell->weight;
        double trialQ = cluster->q + cell->weight * (optimalX - cluster->width);
        int trialWidth = cluster->width + cell->width;

        double trialX = 0;
        while (true)
        {
            trialX = trialQ / trialWeight;

            if (trialX < subRow->minX)
                trialX = subRow->minX;
            if (trialX > subRow->maxX - trialWidth)
                trialX = subRow->maxX - trialWidth;

            const auto prevCluster = cluster->predecessor;
            if (prevCluster && prevCluster->x + prevCluster->width > trialX)
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

void Legalizer::placeRowFinal(SubRow *subRow, Cell *cell)
{
    subRow->freeWidth -= cell->width;

    double optimalX = cell->x;
    if (cell->x < subRow->minX)
        optimalX = subRow->minX;
    else if (cell->x > subRow->maxX - cell->width)
        optimalX = subRow->maxX - cell->width;

    auto cluster = subRow->lastCluster;
    if (!cluster || cluster->x + cluster->width <= optimalX)
    {
        subRow->lastCluster = new Cluster(optimalX, cell, subRow->lastCluster);
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
            if (prevCluster && prevCluster->x + prevCluster->width > cluster->x)
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

double Legalizer::calCost(const Cell *cell) const
{
    if (cell->optimalX == std::numeric_limits<double>::max() ||
        cell->optimalY == std::numeric_limits<double>::max())
        return std::numeric_limits<double>::max();

    double x = cell->optimalX - cell->x;
    double y = cell->optimalY - cell->y;
    return std::sqrt(x * x + y * y);
}

void Legalizer::determinePosition()
{
    for (const auto &row : input->rows)
    {
        int siteWidth = row->siteWidth;
        for (const auto &subRow : row->subRows)
        {
            auto cluster = subRow->lastCluster;
            while (cluster)
            {
                double shiftX = cluster->x - subRow->minX;
                if (shiftX - std::floor(shiftX / siteWidth) * siteWidth <= siteWidth / 2.0)
                    cluster->x = std::floor(shiftX / siteWidth) * siteWidth + subRow->minX;
                else
                    cluster->x = std::ceil(shiftX / siteWidth) * siteWidth + subRow->minX;

                int optimalX = cluster->x;
                for (auto cell : cluster->member)
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
    std::sort(input->cells.begin(), input->cells.end(), [](const Cell::ptr &a, const Cell::ptr &b)
              { return a->x < b->x; });

    for (const auto &cell : input->cells)
    {
        int bestRowIdx = getRowIdx(cell.get());
        int bestSubRowIdx = placeRowTrail(input->rows[bestRowIdx].get(), cell.get());
        double bestCost = calCost(cell.get());
        size_t downFinder = bestRowIdx, upFinder = bestRowIdx;

        while (downFinder > 0 && std::abs(cell->y - input->rows[downFinder]->y) < bestCost)
        {
            --downFinder;
            int subRowIdx = placeRowTrail(input->rows[downFinder].get(), cell.get());
            double cost = calCost(cell.get());
            if (cost < bestCost)
            {
                bestRowIdx = downFinder;
                bestSubRowIdx = subRowIdx;
                bestCost = cost;
            }
        }

        while (upFinder < input->rows.size() - 1 && std::abs(cell->y - input->rows[upFinder]->y) < bestCost)
        {
            ++upFinder;
            int subRowIdx = placeRowTrail(input->rows[upFinder].get(), cell.get());
            double cost = calCost(cell.get());
            if (cost < bestCost)
            {
                bestRowIdx = upFinder;
                bestSubRowIdx = subRowIdx;
                bestCost = cost;
            }
        }

        placeRowFinal(input->rows[bestRowIdx]->subRows[bestSubRowIdx].get(), cell.get());
    }
    determinePosition();
}

std::pair<double, double> Legalizer::getTotalAndMaxDisplacement() const
{
    double totalDisplacement = 0, maxDisplacement = 0;
    for (const auto &cell : input->cells)
    {
        double x = cell->optimalX - cell->x;
        double y = cell->optimalY - cell->y;
        double displacement = std::sqrt(x * x + y * y);
        totalDisplacement += displacement;
        if (maxDisplacement < displacement)
            maxDisplacement = displacement;
    }
    return {totalDisplacement, maxDisplacement};
}

Legalizer::Legalizer(Input *input) : input(input)
{
    std::cout << "----- DESIGN INFORMATION -----\n"
              << "#row:               " << input->rows.size() << "\n"
              << "#cell:              " << input->cells.size() << "\n"
              << "#blockage:          " << input->blockages.size() << "\n"
              << "Max displacement:   " << input->maxDisplacement * input->rows[0]->siteWidth << "\n"
              << "\n";
}

ResultWriter::ptr Legalizer::solve()
{
    divideRow();
    abacusProcess();
    auto [totalDisplacement, maxDisplacement] = getTotalAndMaxDisplacement();
    std::cout << "------- ABACUS RESULT --------\n"
              << "Total displacement: " << totalDisplacement << "\n"
              << "Max displacement:   " << maxDisplacement << "\n"
              << "\n";

    auto result = new ResultWriter();
    for (const auto &cell : input->cells)
        result->addCell(cell.get());
    for (const auto &blockage : input->blockages)
        result->addBlockage(blockage.get());
    return std::unique_ptr<ResultWriter>(result);
}
