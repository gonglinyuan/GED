//
// Created by jiry on 18-6-14.
//

//
// Created by jiry on 18-6-13.
//

#include "GraphTreeSolver.h"
#include "Graph.h"

#include <set>

using std::pair;
using std::make_pair;
using std::set;

Graph generateGraph() {
    Graph graph;
    graph.n = rand() % 5 + 1;
    set<pair<int, int> > isUsed;
    int T = rand() % 10 + 1;
    // printf("generate %d\n", graph.n);
    for (; T; --T) {
        int u = rand() % graph.n + 1;
        int v = rand() % graph.n + 1;
        if (isUsed.find(make_pair(u, v)) != isUsed.end() || u == v) continue;
        isUsed.insert(make_pair(u, v));
        isUsed.insert(make_pair(v, u));
        graph.e.push_back((Graph::Edge){u, v, rand() % 2 + 1});
    }
    // for (auto edge: graph.e) printf("%d %d %d\n", edge.x, edge.y, edge.type);
    graph.label = new int[graph.n + 1];
    for (int i = 1; i <= graph.n; ++i) graph.label[i] = rand() % 2 + 1;
    return graph;
}

Graph g1, g2;

int main() {
    int T = 10000;
    for (; T; --T) {
        g1 = generateGraph();
        g2 = generateGraph();
        GraphTreeSolver solver(g1, g2, 1, 1, 1, 1, 1);
        solver.solve();
        // printf("%d", solver.getans().first);
    }
}