//
// Created by jiry on 18-6-13.
//

#ifndef GED_TREESOLVER_H
#define GED_TREESOLVER_H

#endif //GED_TREESOLVER_H

#include <vector>

#include "Graph.h"

const int KMaxTreeNode = 100;

class Tree{
public:
    int n;
    std::vector<int> go[KMaxTreeNode];
    std::vector<int> edgeLocation[KMaxTreeNode];
    std::vector<std::pair<int,int> > edge;
    std::vector<int> edgeLabel;
    int label[KMaxTreeNode];

    Tree(Graph graph);
    Tree() {};
};

class TreeSolver {
    Tree t1, t2;
    int** dp;
    int n, m;

    int getValue(int loc1, int loc2);
    int getBestMatch(std::vector<int> childEdge1, std::vector<int> childEdge2);

public:
    TreeSolver(Tree _t1, Tree _t2);

    ~TreeSolver() {
        if (dp != nullptr) {
            for (int i = 0; i <= n; ++i) {
                if (dp[i] != nullptr) delete[](dp[i]);
            }
            delete dp;
        }
    }

    int solve();
};