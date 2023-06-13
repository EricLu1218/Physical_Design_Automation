#pragma once
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include "../Timer/Timer.hpp"
#include <vector>

class Solver
{
    Input *input;
    Timer &timer;

    std::vector<Node::ptr> hardblockNodes, cutNodes;
    int outline;

    std::vector<int> generateInitialNpe() const;
    bool isCut(int id) const; // for NPE
    bool isSkewed(const std::vector<int> &npe, size_t pos) const;
    bool satisfyBallot(const std::vector<int> &npe, size_t pos) const;
    std::vector<int> perturbNpe(std::vector<int> npe, int type) const;

    Node *buildSlicingTree(const std::vector<int> &npe);       // Stockmeyer process (bottom up)
    void setPosition(Node *node, size_t choice, int x, int y); // Stockmeyer process (top down)
    int getWirelength() const;
    int getCost(const std::vector<int> &npe, bool focusWirelength);
    std::vector<int> saProcess(double c, double r, int k, std::vector<int> E, bool focusWirelength);

public:
    Solver(Input *input, Timer &timer);
    ResultWriter::ptr solve();
};
