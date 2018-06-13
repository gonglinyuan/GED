//
// Created by jiry on 18-6-13.
//

#include <vector>
#include <cstdio>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

#include "TreeSolver.h"
#include "Graph.h"

using std::pair;
using std::make_pair;
using std::vector;
using std::max;
using std::min;
using std::swap;
using std::cout;
using std::endl;

int findFather(int *father, int u) {
    if (father[u] == u) return u;
    return father[u] = findFather(father, father[u]);
}

Tree::Tree(Graph graph) {
    static int father[KMaxTreeNode];
    n = graph.n;
    for (int i = 1; i <= n; ++i) father[i] = i;
    for (int i = 1; i <= n; ++i) {
        go[i].clear();
        edgeLocation[i].clear();
        label[i] = graph.label[i];
    }
    edge.clear();
    edgeLabel.clear();

    for (int i = 1; i < graph.e.size(); ++i) {
        swap(graph.e[i], graph.e[rand() % (i + 1)]);
    }
    for (int i = 1; i <= n; ++i) label[i] = graph.label[i];
    int total_edge = 0;
    for (const Graph::Edge current_edge: graph.e) {
        int u = current_edge.x, v = current_edge.y;
        if (findFather(father, u) != findFather(father, v)) {
            father[findFather(father, u)] = findFather(father, v);
            go[u].push_back(v);
            go[v].push_back(u);
            total_edge += 1;
            edge.push_back(make_pair(u, v));
            edgeLabel.push_back(current_edge.type);
            edgeLocation[u].push_back(edge.size() - 1);
            edge.push_back(make_pair(v, u));
            edgeLocation[v].push_back(edge.size() - 1);
            edgeLabel.push_back(current_edge.type);
        }
    }

    for (int i = 1; i <= n; ++i) {
        edge.push_back(make_pair(0, i));
        edgeLabel.push_back(0);
    }

    assert(total_edge == n - 1);
    for (int i = 1; i <= n; ++i) assert(findFather(father, i) != findFather(father, 1));
}

TreeSolver::TreeSolver(Tree _t1, Tree _t2): t1(_t1), t2(_t2) {
    n = t1.edge.size();
    m = t2.edge.size();
    dp = new int*[n + 5];
    for (int i = 0; i <= n + 2; ++i) dp[i] = new int[m + 5];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            dp[i][j] = -1;
}

int TreeSolver::getBestMatch(vector<int> childEdge1, vector<int> childEdge2) {
    const int KMaxD = 8;
    static int f[KMaxD + 3][1 << KMaxD];
    memset(f, 0x00, sizeof f);

    vector<vector<int> > value;
    for (int i = 0; i < childEdge1.size(); ++i) {
        vector<int> current_value;
        for (int j = 0; j < childEdge2.size(); ++j) current_value.push_back(getValue(childEdge1[i], childEdge2[j]));
        value.push_back(current_value);
    }
    assert(childEdge1.size() <= KMaxD && childEdge2.size() <= KMaxD);

    int n = childEdge1.size(), m = childEdge2.size();
    for (int i = 0; i < n; ++i) {
        memcpy(f[i + 1], f[i], sizeof f[i]);
        for (int j = 0; j < (1 << m); ++j) {
            for (int k = 0; k < m; ++k) {
                if (j & (1 << k)) continue;
                f[i + 1][j + (1 << k)] = max(f[i + 1][j + (1 << k)], f[i][j] + value[i][k]);
            }
        }
    }
    return f[n][(1 << m) - 1] + 1;
}

int TreeSolver::getValue(int loc1, int loc2) {
    if (dp[loc1][loc2] != -1) return dp[loc1][loc2];

    int node1 = t1.edge[loc1].second, father1 = t1.edge[loc1].first;
    int node2 = t2.edge[loc2].second, father2 = t2.edge[loc2].first;

    if (t1.label[node1] != t2.label[node2] || t1.edgeLabel[loc1] != t2.edgeLabel[loc2]) {
        return dp[loc1][loc2] = 0;
    }
    vector<int> childEdge1, childEdge2;
    for (int i = 0; i < t1.go[node1].size(); ++i) {
        if (t1.go[node1][i] != father1) {
            childEdge1.push_back(t1.edgeLocation[node1][i]);
        }
    }
    for (int i = 0; i < t2.go[node2].size(); ++i) {
        if (t2.go[node2][i] != father2) {
            childEdge2.push_back(t2.edgeLocation[node2][i]);
        }
    }

    dp[loc1][loc2] = getBestMatch(childEdge1, childEdge2);
    return dp[loc1][loc2];
}

int TreeSolver::solve() {
    int ans = 0;
    /*puts("t1");
    for (int i = 0; i < t1.edge.size(); ++i) printf("%d %d %d %d\n", i, t1.edge[i].first, t1.edge[i].second, t1.edgeLabel[i]);
    puts("t2");
    for (int i = 0; i < t2.edge.size(); ++i) printf("%d %d %d %d\n", i, t2.edge[i].first, t2.edge[i].second, t2.edgeLabel[i]);*/
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (t1.edge[i].first == 0 && t2.edge[j].first == 0) {
                ans = max(ans, getValue(i, j));
            }
        }
    }
    return ans;
}
