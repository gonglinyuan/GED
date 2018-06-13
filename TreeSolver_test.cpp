//
// Created by jiry on 18-6-13.
//

#include "TreeSolver.h"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <cassert>

using std::pair;
using std::make_pair;
using std::vector;
using std::max;
using std::min;

Tree readTree() {
    Tree tree;
    tree.n = 5 + rand() % 3;
    //scanf("%d", &tree.n);
    for (int i = 1; i <= tree.n; ++i) {
        tree.label[i] = rand() % 2;
    //    scanf("%d", &tree.label[i]);
    }
    for (int i = 1; i < tree.n; ++i) {
        int u, v, type;
        u = i + 1;
        v = rand() % i + 1;
        type = rand() % 2;
    //    scanf("%d%d%d", &u, &v, &type);
        tree.edgeLabel.push_back(type);
        tree.edgeLabel.push_back(type);
        tree.edge.push_back(make_pair(u, v));
        tree.edge.push_back(make_pair(v, u));
        tree.go[u].push_back(v);
        tree.go[v].push_back(u);
        tree.edgeLocation[u].push_back(tree.edgeLabel.size() - 2);
        tree.edgeLocation[v].push_back(tree.edgeLabel.size() - 1);
    }

    for (int i = 1; i <= tree.n; ++i) {
        tree.edge.push_back(make_pair(0, i));
        tree.edgeLabel.push_back(0);
    }
    return tree;
}

Tree t1, t2;
int n, m, ans, isUsed[10], per[10];

void force(int loc) {
    if (loc > n) {
        int flag = 0, total_edge = 0;
        for (int i = 0; i < t1.edge.size(); ++i) {
            pair<int, int> edge = t1.edge[i];
            int node1 = edge.second, father1 = edge.first;
            if (father1 == 0 || per[node1] == 0 || per[father1] == 0) continue;
            total_edge += 1;
            int node2 = per[node1], father2 = per[father1], isOccur = 0;
            for (int j = 0; j < t2.edge.size(); ++j) {
                pair<int, int> edge2 = t2.edge[j];
                if (edge2.second == node2 && edge2.first == father2 && t1.edgeLabel[i] == t2.edgeLabel[j])
                    isOccur = 1;
            }
            if (isOccur == 0) {
                flag = 1; break;
            }
        }
        if (flag == 0) {
            assert(total_edge % 2 == 0);
            total_edge /= 2;
            int num = 0;
            for (int i = 1; i <= n; ++i) num += (per[i] > 0);
            assert(total_edge < num || num == 0);
            if (num > ans && total_edge == num - 1) {
                ans = num;
            }
        }
        return;
    }
    per[loc] = 0; force(loc + 1);
    for (int i = 1; i <= m; ++i) {
        if (isUsed[i] == 0 && t1.label[loc] == t2.label[i]) {
            per[loc] = i; isUsed[i] = 1;
            force(loc + 1);
            isUsed[i] = 0;
        }
    }
}

void printTree(Tree tree) {
    printf("%d\n", tree.n);
    for (int i = 1; i <= tree.n; ++i) printf("%d ", tree.label[i]);
    puts("");
    for (int i = 0; i < tree.edge.size(); i += 2) {
        if (tree.edge[i].first == 0) continue;
        printf("%d %d %d\n", tree.edge[i].first, tree.edge[i].second, tree.edgeLabel[i]);
    }
}

void check() {
    t1 = readTree(); n = t1.n;
    t2 = readTree(); m = t2.n;
    TreeSolver solver(t1, t2);
    int ans1 = solver.solve().first;
    ans = 0;
    memset(isUsed, 0x00, sizeof isUsed);
    force(1);
    printf("%d %d\n", ans1, ans);
    if (ans1 != ans) {
        puts("t1");
        printTree(t1);
        puts("t2");
        printTree(t2);
        exit(0);
    }
}

int main() {
    int T = 10000;
    for (; T; T--) check();
}