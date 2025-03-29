#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include "../Timer/Timer.hpp"
#include <utility>
#include <vector>

class Solver
{
    Input *input;
    Timer &timer;

    std::vector<Node::ptr> hardblockNodes, cutNodes;
    int outline;

    std::vector<int> getInitialExpression() const;
    bool isCut(int id) const; // for expression
    bool isSkewed(const std::vector<int> &expression, size_t idx) const;
    bool satisfyBallot(const std::vector<int> &expression, size_t idx) const;
    std::vector<int> perturb(std::vector<int> expression, int type) const;

    Node *buildSlicingTree(const std::vector<int> &expression);                            // Stockmeyer process (bottom up part)
    void setPosition(Node *node, size_t choice, int x, int y);                             // Stockmeyer process (top down part)
    std::pair<bool, int> getCost(const std::vector<int> &expression, bool withWirelength); // Stockmeyer process for cost calculation
    int getWirelength(const std::vector<int> &expression);                                 // Stockmeyer process for wirelength
    std::pair<std::vector<int>, int> simulatedAnnealing(std::vector<int> expression, bool withWirelength,
                                                        double initTemperature, double minTemperature,
                                                        double coolingCoefficient, int tryingTimes,
                                                        double maxRejectRatio);

public:
    Solver(Input *input, Timer &timer);
    ResultWriter::ptr solve();
};
