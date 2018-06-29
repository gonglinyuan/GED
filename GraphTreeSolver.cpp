//
// Created by pro on 18/6/14.
//

#include "GraphTreeSolver.h"

#include <cassert>
#include <algorithm>
#include <thread>

using std::pair;
using std::make_pair;
using std::set;
using std::vector;
using std::max;
using std::thread;
using std::bind;
using std::mutex;

pair<Tree, vector<int> > deleteNodeFromTree(int *isUsed, Tree tree) {
    int id[tree.n + 1] = {0};
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
    for (int i = 0; i < tree.edge.size(); i += 2) {
        pair<int, int> edge = tree.edge[i];
        int u = edge.first, v = edge.second;
        if (isUsed[u] || isUsed[v] || u == 0 || v == 0) continue;
        u = id[u], v = id[v];
        remTree.edge.emplace_back(make_pair(u, v));
        remTree.edge.emplace_back(make_pair(v, u));
        remTree.go[u].push_back(v);
        remTree.go[v].push_back(u);
        remTree.edgeLocation[u].push_back(remTree.edge.size() - 2);
        remTree.edgeLocation[v].push_back(remTree.edge.size() - 1);
        remTree.edgeLabel.push_back(tree.edgeLabel[i]);
        remTree.edgeLabel.push_back(tree.edgeLabel[i]);
    }
    for (int i = 1; i <= remTree.n; ++i) {
        remTree.edge.emplace_back(make_pair(0, i));
        remTree.edgeLabel.push_back(0);
    }

    vector<int> preid(remTree.n + 1);
    for (int i = 1; i <= tree.n; ++i)
        if (isUsed[i] == 0) preid[id[i]] = i;

    return make_pair(remTree, preid);
}

vector<int> GraphTreeSolver::solveTree(Tree t1, Tree t2) const {
    int n = t1.n;
    int m = t2.n;
    int isUsed[max(n, m) + 1];
    vector<int> perm(n + 1);
    for (int i = 1; i <= n; ++i) perm[i] = 0;
    while (true) {
        memset(isUsed, 0x00, sizeof isUsed);
        for (int i = 1; i <= n; ++i) if (perm[i]) isUsed[i] = 1;
        pair<Tree, vector<int> > remTree1 = deleteNodeFromTree(isUsed, t1);
        memset(isUsed, 0x00, sizeof isUsed);
        for (int i = 1; i <= n; ++i) if (perm[i]) isUsed[perm[i]] = 1;
        pair<Tree, vector<int> > remTree2 = deleteNodeFromTree(isUsed, t2);
        if (remTree1.first.n == 0 || remTree2.first.n == 0) break;
        // printf("%d %d\n", remTree1.first.n, remTree2.first.n);
        TreeSolver treeSolver(remTree1.first, remTree2.first);
        auto result = treeSolver.solve();
        if (result.first == 0) break;
        for (int i = 1; i <= remTree1.first.n; ++i) {
            if (result.second[i]) {
                perm[remTree1.second[i]] = remTree2.second[result.second[i]];
            }
        }
        // printf("%d %d %d\n", remTree1.first.n, remTree2.first.n, result.first);
        // for (int i = 1; i <= n; ++i) printf("%d ", perm[i]); puts(""); int kk; scanf("%d", &kk);
    }
    return perm;
}

int GraphTreeSolver::calculateCost(const int *perm) const {
    int total_cost = 0;
//    static int isUsed[KMaxTreeNode];
//    static int edgeTag[KMaxTreeNode][KMaxTreeNode] = {0}, edgeId[KMaxTreeNode][KMaxTreeNode];
//    memset(isUsed, 0x00, sizeof isUsed);
    int n = g1.n;
    int m = g2.n;
    int isUsed[m + 1] = {0};
    int sign = 0;
    int edgeTag[m + 1][m + 1] = {0}, edgeId[m + 1][m + 1];
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
        int u = g2.e[i].x, v = g2.e[i].y;
        assert(edgeTag[u][v] != sign);
        edgeTag[u][v] = edgeTag[v][u] = sign;
        edgeId[u][v] = edgeId[v][u] = i;
    }
    for (auto edge: g1.e) {
        int u = edge.x, v = edge.y;
        if (perm[u] == 0 || perm[v] == 0) {
            total_cost += addEdge;
            continue;
        }
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
        if (edgeTag[u][v] == sign && edgeTag[v][u] == sign) total_cost += addEdge;
    }
    return total_cost;
}

GraphTreeSolver::CandidateSolution GraphTreeSolver::mutate(const GraphTreeSolver::CandidateSolution current) const {
    GraphTreeSolver::CandidateSolution now = current;
    int n = g1.n;
    int m = g2.n;
//    static int isUsed[KMaxTreeNode];
//    memset(isUsed, 0x00, sizeof isUsed);
    int isUsed[m + 1] = {0};
    for (int i = 1; i <= n; ++i) isUsed[current.perm[i]] = 1;
    while (true) {
        int u = rand() % n + 1;
        int v = rand() % (n + m + 1) - n;
        if (v < 0) {
            v = -v;
            if (current.perm[u] == current.perm[v]) continue;
            now = current;
            std::swap(now.perm[u], now.perm[v]);
            now.cost = calculateCost(now.perm);
            return now;
        } else {
            if (isUsed[v] || current.perm[u] == v) continue;
            now = current;
            now.perm[u] = v;
            now.cost = calculateCost(now.perm);
            return now;
        }
    }
}

GraphTreeSolver::CandidateSolution GraphTreeSolver::mixTwoCandidate(const GraphTreeSolver::CandidateSolution candidate1,
                                                                    const GraphTreeSolver::CandidateSolution candidate2) const {
    return candidate1;
}

void GraphTreeSolver::getChildNew(const GraphTreeSolver::CandidateSolution currentCandidate) {
//    static int isUsed[KMaxTreeNode];
//    memset(isUsed, 0x00, sizeof isUsed);
//    int n = g1.n, m = g2.n;
//    int isUsed[m+ 1] = {0};
//    for (int i = 1; i <= n; ++i) if (currentCandidate.perm[i]) isUsed[currentCandidate.perm[i]] = 1;
//    GraphTreeSolver::CandidateSolution now = currentCandidate;
    for (int i = 0; i < KMutateNum; ++i) {
        if (rand() % 10 <= 10) {
            insert(mutate(currentCandidate));
        } else {
            int u = rand() % totalCandidate;
            insert(mixTwoCandidate(currentCandidate, current[u]));
        }
    }
//    vector<CandidateSolution> sols;
//    for (int i = 0; i < KMutateNum; ++i) {
//        CandidateSolution sol = mutate(currentCandidate);
//        sols.push_back(sol);
//    }
//    mtx.try_lock();
//    for (const auto &it : sols) {
//        insert(it);
//    }
//    mtx.unlock();
}

void GraphTreeSolver::getChild(const GraphTreeSolver::CandidateSolution current) {
    for (int i = 0; i < KMutateNum; ++i) {
        insert(mutate(current));
    }
}

GraphTreeSolver::CandidateSolution GraphTreeSolver::drawFromCandidate() {
    double total = 0;
    int flag = 0;
    const double eps = 1e-8;
    for (const auto &current: candidate)
        if (current.cost != -1) total += 1.0 / (1 + current.cost), flag = 1;
    std::uniform_real_distribution<double> distribution(0, total);
    double drawNumber = distribution(rng);
    for (auto &current: candidate) {
        if (current.cost == -1) continue;
        double currentP = 1.0 / (1 + current.cost);
        if (drawNumber < currentP + eps) {
            CandidateSolution result = current;
            current.cost = -1;
            return result;
        }
        drawNumber -= currentP;
    }
    assert(0);
}

void GraphTreeSolver::insert(const GraphTreeSolver::CandidateSolution &currentCandidate) {
    int num = currentCandidate.getHash();
    if (isCandidate.find(num) != isCandidate.end()) return;
    isCandidate.insert(num);
    candidate.emplace_back(currentCandidate);
}

GraphTreeSolver::CandidateSolution GraphTreeSolver::randomSolution() const {
    int n = g1.n, m = g2.n;
//    static int perm[KMaxTreeNode], isUsed[KMaxTreeNode];
//    memset(perm, 0x00, sizeof perm);
//    memset(isUsed, 0x00, sizeof isUsed);
    int perm[n + 1] = {0}, isUsed[m + 1] = {0};
    GraphTreeSolver::CandidateSolution result;
    for (int i = 1; i <= n; ++i) {
        int u = rand() % (m + 1);
        while (u && isUsed[u]) u = rand() % (m + 1);
        isUsed[u] = 1;
        perm[i] = u;
    }
    for (int i = 1; i <= n; ++i) {
        result.perm[i] = perm[i];
    }
    result.cost = calculateCost(result.perm);
    return result;
}

void GraphTreeSolver::getNewCandidate() {
    totalCandidate = 0;
    std::sort(candidate.begin(), candidate.end());
    for (int i = 0; i < KKeepFirstNum && i < candidate.size(); ++i) {
        current[i] = candidate[i];
        candidate[i].cost = -1;
        totalCandidate += 1;
    }
    for (int i = KKeepFirstNum; i < KMaxKeep && i < candidate.size() && checkTL(); ++i) {
        current[i] = drawFromCandidate();
        totalCandidate += 1;
    }
    assert(totalCandidate <= KMaxKeep);
}

void GraphTreeSolver::get_all_children(int num) {
    vector<CandidateSolution> sols;
    for (int i = 0; i < totalCandidate; ++i) {
        if (i % 4 == num) {
            for (int j = 0; j < KMutateNum; ++j) {
                CandidateSolution sol = mutate(current[i]);
                sols.push_back(sol);
            }
        }
    }
    mtx.lock();
    for (const auto &it : sols) {
        insert(it);
    }
    mtx.unlock();
}

void GraphTreeSolver::solve() {
    isCandidate.clear();
    for (int i = 0; i < KMaxKeep; ++i) {
        vector<int> result = solveTree(g1, g2);
        for (int j = 1; j <= g1.n; ++j) current[i].perm[j] = result[j];
        current[i].cost = calculateCost(current[i].perm);
        insert(current[i]);
    }
    getNewCandidate();
    for (int i = 0; i < totalCandidate; ++i) {
        if (current[i].cost < optimal.cost) {
            optimal = current[i];
        }
    }
    int round = 0;
    while (checkTL()) {
        candidate.clear();
        isCandidate.clear();
//        for (int i = 0; i < totalCandidate && checkTL(); ++i) getChildNew(current[i]);
        thread thread0(bind(&GraphTreeSolver::get_all_children, this, 0));
        thread thread1(bind(&GraphTreeSolver::get_all_children, this, 1));
        thread thread2(bind(&GraphTreeSolver::get_all_children, this, 2));
        thread thread3(bind(&GraphTreeSolver::get_all_children, this, 3));
        thread0.join();
        thread1.join();
        thread2.join();
        thread3.join();
//        printf("%lu\n", candidate.size());
        for (const auto &current: candidate) {
            if (current.cost < optimal.cost) optimal = current;
        }
        getNewCandidate();
    }
    // for (int i = 1; i <= g1.n; ++i) printf("%d ", optimal.perm[i]); puts("");
}