//
// Created by pro on 18/6/14.
//

#ifndef GED_GRAPHTREESOLVER_H
#define GED_GRAPHTREESOLVER_H

#include <cstring>
#include <vector>
#include <set>
#include <random>
#include <ctime>

#include "TreeSolver.h"
#include "Graph.h"

const int KMaxKeep = 50;
const int KMutateNum = 20;
const int KKeepFirstNum = 20;
class GraphTreeSolver {
    Graph g1, g2;
    int addEdge, subEdge, addNode, subNode, timeLimit, totalCandidate;
    std::mt19937 rng;
    clock_t start;

    class CandidateSolution {
    public:
        int cost;
        int perm[KMaxTreeNode];

        CandidateSolution() {
            cost = 1000000000;
            memset(perm, 0x00, sizeof perm);
        }
        int operator < (const CandidateSolution& candidate) const {
            return cost < candidate.cost;
        }
        unsigned long long getHash() const {
            static const unsigned long long key = 10007;
            unsigned long long hash = 0;
            for (int i = 1; i < KMaxTreeNode; ++i) {
                hash = hash * key + perm[i];
            }
            return hash;
        }
    } optimal, current[KMaxKeep];
    std::vector<CandidateSolution> candidate;
    std::set<unsigned long long> isCandidate;

    std::vector<int> solveTree(Tree t1, Tree t2);
    int calculateCost(int* perm);
    CandidateSolution mutate(const CandidateSolution candidate);
    void getChild(const CandidateSolution candidate);
    void getChildNew(const CandidateSolution candidate);
    CandidateSolution drawFromCandidate();
    CandidateSolution mixTwoCandidate(const CandidateSolution candidate1, const CandidateSolution candidate2);
    CandidateSolution randomSolution();
    void getNewCandidate();
    void insert(const CandidateSolution& currentCandidate);
    bool checkTL() {
        return (clock() - start) / CLOCKS_PER_SEC < timeLimit;
    }


public:
    GraphTreeSolver(Graph _g1, Graph _g2, int _addEdge, int _subEdge, int _addNode, int _subNode, int _timeLimit):
            g1(_g1), g2(_g2), addEdge(_addEdge), subEdge(_subEdge), addNode(_addNode), subNode(_subNode) {
        timeLimit = _timeLimit;
        rng.seed(std::random_device()());
        start = clock();
    }

    void solve();
    std::pair<int, int*> getans() {
        return std::make_pair(optimal.cost, optimal.perm);
    }
};


#endif //GED_GRAPHTREESOLVER_H
