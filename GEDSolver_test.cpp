//
// Created by jiry on 18-5-14.
//

#include "GEDSolver.h"
#include "Graph.h"

void read_graph(Graph& g) {
    scanf("%d", &g.n);
    g.label = new int[g.n + 10];
    for (int i = 1; i <= g.n; ++i) scanf("%d", &g.label[i]);
    int m; scanf("%d", &m);
    for (int i = 1; i <= m; ++i) {
         int u, v, label;
         scanf("%d%d%d", &u, &v, &label);
         g.e.push_back((Graph::Edge){u, v, label});
    }
}
int main() {
    int c_node_ins, c_node_sub, c_edge_ins, c_edge_sub;
    scanf("%d%d%d%d", &c_node_ins, &c_node_sub, &c_edge_ins, &c_edge_sub);
    Graph g1,g2; read_graph(g1); read_graph(g2);
    // printf("%d %d\n",g1.label[1], g2.label[1]);
    printf("Read Complete\n");
    GEDSolver solver(g1, g2, c_node_ins, c_node_sub, c_edge_ins, c_edge_sub, 25);
    solver.calculate_GED();
    printf("%d\n", solver.get_GED());
}