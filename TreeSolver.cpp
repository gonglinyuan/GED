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

Tree::Tree() {
    n = 0;
    memset(label, 0x00, sizeof label);
}

Tree::Tree(Graph graph) {
    int father[KMaxTreeNode];
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

    // assert(total_edge == n - 1);
    // for (int i = 1; i <= n; ++i) assert(findFather(father, i) != findFather(father, 1));
}

TreeSolver::TreeSolver(Tree _t1, Tree _t2) : t1(_t1), t2(_t2) {
    n = t1.edge.size();
    m = t2.edge.size();
    dp = new int *[n + 5];
    for (int i = 0; i <= n + 2; ++i) dp[i] = new int[m + 5];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            dp[i][j] = -1;
    memset(perm, 0x00, sizeof perm);
    memset(f, 0x00, sizeof f);
}

int TreeSolver::getBestMatch(const vector<int> &childEdge1, const vector<int> &childEdge2) {
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

void TreeSolver::getBestMatchWay(const vector<int> &childEdge1, const vector<int> &childEdge2) {
    getBestMatch(childEdge1, childEdge2);
    int n = childEdge1.size(), m = childEdge2.size();
    int now = (1 << m) - 1;
    vector<pair<int, int> > matchPair;
    for (int i = (n - 1); i >= 0; --i) {
        if (f[i][now] == f[i + 1][now]) {
            continue;
        }
        for (int j = 0; j < m; ++j) {
            if (now & (1 << j)) {
                int preS = now - (1 << j);
                if (f[i][preS] + getValue(childEdge1[i], childEdge2[j]) == f[i + 1][now]) {
                    now = preS;
                    matchPair.push_back(make_pair(childEdge1[i], childEdge2[j]));
                    break;
                }
            }
        }
    }
    for (auto pair: matchPair) getPermutation(pair.first, pair.second);
}

void TreeSolver::getPermutation(int loc1, int loc2) {
    assert(dp[loc1][loc2] != -1);
    if (dp[loc1][loc2] == 0) return;

    int node1 = t1.edge[loc1].second, father1 = t1.edge[loc1].first;
    int node2 = t2.edge[loc2].second, father2 = t2.edge[loc2].first;

    assert(t1.label[node1] == t2.label[node2] && t1.edgeLabel[loc1] == t2.edgeLabel[loc2]);
    assert(perm[node1] == 0);
    // printf("%d %d %d\n", node1, node2, dp[loc1][loc2]);
    perm[node1] = node2;

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
    getBestMatchWay(childEdge1, childEdge2);
}

pair<int, int *> TreeSolver::solve() {
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
    int flag = 0;
    for (int i = 0; i < n && flag == 0; ++i) {
        for (int j = 0; j < m && flag == 0; ++j) {
            if (t1.edge[i].first == 0 && t2.edge[j].first == 0 && dp[i][j] == ans) {
                getPermutation(i, j);
                flag = 1;
            }
        }
    }
    checkSolution(ans);
    return make_pair(ans, perm);
}

void TreeSolver::checkSolution(int ans) const {
    int n = t1.n;
    int m = t2.n;
    int flag = 0, total_edge = 0;
    for (int i = 0; i < t1.edge.size(); ++i) {
        pair<int, int> edge = t1.edge[i];
        int node1 = edge.second, father1 = edge.first;
        if (father1 == 0 || perm[node1] == 0 || perm[father1] == 0) continue;
        total_edge += 1;
        int node2 = perm[node1], father2 = perm[father1], isOccur = 0;
        for (int j = 0; j < t2.edge.size(); ++j) {
            pair<int, int> edge2 = t2.edge[j];
            if (edge2.second == node2 && edge2.first == father2 && t1.edgeLabel[i] == t2.edgeLabel[j])
                isOccur = 1;
        }
        if (isOccur == 0) {
            flag = 1;
            break;
        }
    }
    assert(flag == 0);
    assert(total_edge % 2 == 0);
    total_edge /= 2;
    int num = 0;
    for (int i = 1; i <= n; ++i) num += (perm[i] > 0);
    assert(total_edge == num - 1 || num == 0);
    assert(num == ans);
}