//
// Created by jiry on 18-5-12.
//

#ifndef GED_GEDSOVLER_H
#define GED_GEDSOVLER_H

#endif //GED_GEDSOVLER_H

#include "Simplex.h"
#include "Graph.h"

const int MAXV = 30;
const int MAXE = 30;

class GEDSolver {

    class candidate_solution {
    public:
        int node_state[MAXV];
        int depth;
        double value;
        SimplexResult result;

        int operator > (const candidate_solution& k1) {
            return value > k1.value;
        }

        candidate_solution():value(0) {
            memset(node_state, 0x00, sizeof node_state);
        }
    };

    Simplex simplex_solver;
    Graph g1, g2;
    int cost_ins_node, cost_sub_node;
    int cost_ins_edge, cost_sub_edge;
    int current_ans;
    int way[MAXV];
    vector<candidate_solution> list[2];

    int node_sub_cost(int i, int j);
    int edge_sub_cost(int i, int j);
    int other_costs();
    void get_lower_bound_for_candidate(candidate_solution& candidate);
    void get_final_value_for_candidate(candidate_solution& candidate);
    void solve(int width);
    void extend(candidate_solution pre, vecotr<candidate_solution>& list);

public:
    GEDSolver(Graph _g1, Graph _g2, int c_ins_node, int c_sub_node, int c_ins_edge, int c_sub_edge) {
        g1 = _g1; g2 = _g2;
        cost_ins_node = c_ins_node;
        cost_sub_node = c_sub_node;
        cost_ins_edge = c_ins_edge;
        cost_sub_edge = c_sub_edge;
        current_ans = 1e9;
    }

    void calculate_GED();
    int* get_way() {return way;}
    int get_GED() {return current_ans;}



};
