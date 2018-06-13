//
// Created by pro on 18/6/14.
//

#ifndef GED_GRAPHTREESOLVER_H
#define GED_GRAPHTREESOLVER_H

#include <cstring>
#include <vector>
#include <set>

#include "TreeSolver.h"

const int KMaxKeep = 50;
class GraphTreeSolver {
    Graph g1, g2;
    int addEdge, subEdge, addNode, subNode;

    class CandidateWay {
    public:
        int cost;
        int perm[KMaxTreeNode];

        CandidateWay() {
            cost = 1000000000;
            memset(perm, 0x00, sizeof perm);
        }
    } optimal, current[KMaxKeep];

    std::vector<int> solveTree(Tree t1, Tree t2);
    int calculateCost(const std::vector<int> &perm);

public:
    GraphTreeSolver(Graph _g1, Graph _g2, int _addEdge, int _subEdge, int _addNode, int _subNode):
            g1(_g1), g2(_g2), addEdge(_addEdge), subEdge(_subEdge), addNode(_addNode), subNode(_subNode) {}

    void solve();
};


#endif //GED_GRAPHTREESOLVER_H
