#pragma once
#include "../GlobalTimer/GlobalTimer.hpp"
#include "../ResultWriter/ResultWriter.hpp"
#include "../Structure/Data.hpp"
#include <vector>

class SA_Solver
{
    SAInput *input;
    GlobalTimer &globalTimer;
    std::vector<Node *> hardblockNodes, cutNodes;
    double outline;

    void getInitialNPE(std::vector<int> &npe);
    void invertCut(std::vector<int> &npe, size_t pos);
    bool checkSkewed(std::vector<int> const &npe, size_t const &pos) const;
    bool checkBallot(std::vector<int> const &npe, size_t const &pos) const;
    std::vector<int> perturbNPE(std::vector<int> npe, int const &type);
    Node *buildSlicingTree(std::vector<int> const &npe);
    void setPosition(Node *node, size_t const &choice, int const &x, int const &y);
    int calWirelength() const;
    int calCost(std::vector<int> const &npe, bool const &focusWirelength);
    std::vector<int> saProcess(double const &c, double const &r, int const &k,
                                        std::vector<int> const &NPE, bool const &focusWirelength);

public:
    SA_Solver(SAInput *input, GlobalTimer &globalTimer);
    ResultWriter *solve();
};
