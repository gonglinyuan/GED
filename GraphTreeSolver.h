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

const int KMaxKeep = 70;
const int KMutateNum = 20;
const int KKeepFirstNum = 40;
class GraphTreeSolver {
public:
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

    std::vector<int> solveTree(Tree t1, Tree t2)const;
    int calculateCost(const int* perm)const;
    CandidateSolution mutate(const CandidateSolution candidate)const;
    void getChild(const CandidateSolution candidate);
    void getChildNew(const CandidateSolution candidate);
    CandidateSolution drawFromCandidate();
    CandidateSolution mixTwoCandidate(const CandidateSolution candidate1, const CandidateSolution candidate2)const;
    CandidateSolution randomSolution()const;
    void getNewCandidate();
    void insert(const CandidateSolution& currentCandidate);
    bool checkTL()const {
        return (clock() - start) / CLOCKS_PER_SEC < timeLimit;
    }


    GraphTreeSolver(Graph _g1, Graph _g2, int _addEdge, int _subEdge, int _addNode, int _subNode, int _timeLimit):
            g1(_g1), g2(_g2), addEdge(_addEdge), subEdge(_subEdge), addNode(_addNode), subNode(_subNode) {
        timeLimit = _timeLimit;
        rng.seed(std::random_device()());
        start = clock();
    }
    GraphTreeSolver() {}
    ~GraphTreeSolver() {}

    void solve();
    std::pair<int, std::vector<int> > getans()const {
        std::vector<int> ans;
        for (int i = 1; i <= g1.n; ++i) {
            if (optimal.perm[i] == 0) ans.push_back(-1); else ans.push_back(optimal.perm[i]);
        }
        return std::make_pair(optimal.cost, ans);
    }
};


#endif //GED_GRAPHTREESOLVER_H
