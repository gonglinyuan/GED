//
// Created by pro on 18/6/14.
//

#include "GraphTreeSolver.h"

#include <cassert>

using std::pair;
using std::make_pair;
using std::vector;

pair<Tree, vector<int> > deleteNodeFromTree(int *isUsed, Tree tree) {
    static int id[KMaxTreeNode];
    memset(id, 0x00, sizeof id);
    int remNodeNumber = 0;
    for (int i = 1; i <= tree.n; ++i) {
        if (isUsed[i] == 0) {
            id[i] = ++remNodeNumber;
        }
    }
    Tree remTree;
    remTree.n = remNodeNumber;
    for (int i = 1; i <= tree.n; ++i) {
        if (isUsed[i] == 0) remTree.label[id[i]] = tree.label[i];
    }
    for (int i = 0; i < tree.edge.size(); ++i) {
        pair<int ,int> edge = tree.edge[i];
        int u = edge.first, v = edge.second;
        if (isUsed[i] || isUsed[v]) continue;
        u = id[u], v = id[v];
        remTree.edge.emplace_back(make_pair(u, v));
        remTree.edge.emplace_back(make_pair(v, u));
        remTree.go[u].push_back(v);
        remTree.go[v].push_back(v);
        remTree.edgeLocation[u].push_back(remTree.edge.size() - 2);
        remTree.edgeLocation[v].push_back(remTree.edge.size() - 1);
        remTree.edgeLabel.push_back(tree.edgeLabel[i]);
        remTree.edgeLabel.push_back(tree.edgeLabel[i]);
    }
    for (int i = 1; i <= tree.n; ++i) {
        remTree.edge.emplace_back(make_pair(0, i));
        remTree.edgeLabel.push_back(0);
    }

    vector<int> preid(tree.n + 1);
    for (int i = 1; i <= tree.n; ++i)
        if (isUsed[i] == 0) preid[id[i]] = i;

    return make_pair(remTree, preid);
}

vector<int> GraphTreeSolver::solveTree(Tree t1, Tree t2) {
    static int isUsed[KMaxTreeNode];
    int n = t1.n;
    int m = t2.n;
    vector<int> perm(n + 1);
    for (int i = 1; i <= n; ++i) perm[i] = 0;
    while (1) {
        memset(isUsed, 0x00, sizeof isUsed);
        for (int i = 1; i <= n; ++i) if (perm[i]) isUsed[i] = 1;
        pair<Tree, vector<int> > remTree1 = deleteNodeFromTree(isUsed, t1);
        memset(isUsed, 0x00, sizeof isUsed);
        for (int i = 1; i <= n; ++i) if (perm[i]) isUsed[perm[i]] = 1;
        pair<Tree, vector<int> > remTree2 = deleteNodeFromTree(isUsed, t2);
        if (remTree1.first.n == 0 || remTree2.first.n == 0) break;

        TreeSolver treeSolver(t1, t2);
        auto result = treeSolver.solve();
        if (result.first == 0) break;
        for (int i = 1; i <= remTree1.first.n; ++i)
            if (result.second[i]) {
                perm[remTree1.second[i]] = remTree2.second[result.second[i]];
            }
    }
    return perm;
}

int GraphTreeSolver::calculateCost(const std::vector<int> &perm) {
    int total_cost = 0;
    static int isUsed[KMaxTreeNode];
    static int edgeTag[KMaxTreeNode][KMaxTreeNode] = {0}, sign = 0, edgeId[KMaxTreeNode][KMaxTreeNode];
    memset(isUsed, 0x00, sizeof isUsed);
    int n = g1.n;
    int m = g2.n;
    for (int i = 1; i <= n; ++i) {
        if (perm[i]) {
            assert(isUsed[perm[i]] == 0);
            isUsed[perm[i]] = 1;
            if (g1.label[i] != g2.label[perm[i]]) total_cost += subNode;
        } else total_cost += addNode;
    }
    for (int i = 1; i <= m; ++i)
        if (isUsed[i] == 0) total_cost += addNode;
    ++sign;
    for (int i = 0; i < g2.e.size(); ++i) {
        int u = g2.e.size(), v = g2.e.size();
        assert(edgeTag[u][v] != sign);
        edgeTag[u][v] = edgeTag[v][u] = sign;
        edgeId[u][v] = edgeId[v][u] = i;
    }
    for (auto edge: g1.e) {
        int u = edge.x, v = edge.y;
        if (perm[u] == 0 || perm[v] == 0) total_cost += addEdge;
        if (edgeTag[perm[u]][perm[v]] != sign) {
            total_cost += addEdge;
            continue;
        }
        auto edge2 = g2.e[edgeId[perm[u]][perm[v]]];
        if (edge2.type != edge.type) total_cost += subEdge;
        edgeTag[perm[u]][perm[v]] = 0;
    }
    for (int i = 0; i < g2.e.size(); ++i) {
        int u = g2.e[i].x, v = g2.e[i].y;
        if (edgeTag[u][v] == 0 || edgeTag[v][u] == 0) total_cost += addEdge;
    }
    return total_cost;
}

void GraphTreeSolver::solve() {
    for (int i = 0; i < KMaxKeep; ++i) {
        vector<int> result = solveTree(g1, g2);
        current[i].cost = calculateCost(result);
        for (int j = 1; j <= g1.n; ++j) current[i].perm[j] = result[j];
    }
    for (int i = 0; i < KMaxKeep; ++i) {
        if (current[i].cost < optimal.cost) {
            optimal = current[i];
        }
    }
    for (int t = 1; t <= 50; ++ t) {
        
    }
}