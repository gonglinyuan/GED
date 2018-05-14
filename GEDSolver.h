//
// Created by jiry on 18-5-12.
//

#ifndef GED_GEDSOVLER_H
#define GED_GEDSOVLER_H

#endif //GED_GEDSOVLER_H

#include <vector>

#include "Simplex.h"
#include "Graph.h"

const int MAXV = 50;
const int MAXE = 50;

class GEDSolver {

    class candidate_solution {
    public:
        int node_state[MAXV];
        int depth;
        double value;
        SimplexResult result;

        int operator < (const candidate_solution& k1) const {
            return value < k1.value;
        }

        candidate_solution(): value(0), depth(0) {
            memset(node_state, 0x00, sizeof node_state);
        }

        void print() const {
            printf("%d %.5lf\n", depth, value);
            for (int i = 1; i <= depth; ++i) printf("%d ", node_state[i]); puts("");
        }
    };

    Simplex simplex_solver;
    Graph g1, g2;
    int cost_ins_node, cost_sub_node;
    int cost_ins_edge, cost_sub_edge;
    int current_best;
    int way[MAXV];
    int time_limit;
    std::vector<candidate_solution> list[2];
    clock_t start;

    int check_TL();
    int node_sub_cost(int i, int j);
    int edge_sub_cost(int i, int j);
    int other_costs() const;
    void get_lower_bound_for_candidate(candidate_solution& candidate);
    void get_final_value_for_candidate(candidate_solution& candidate);
    void solve(int width);
    void extend(candidate_solution pre, std::vector<candidate_solution>& list);

public:
    GEDSolver(Graph _g1, Graph _g2, int c_ins_node, int c_sub_node, int c_ins_edge, int c_sub_edge, int TL):
            g1(_g1), g2(_g2){
        time_limit = TL;
        cost_ins_node = c_ins_node;
        cost_sub_node = c_sub_node;
        cost_ins_edge = c_ins_edge;
        cost_sub_edge = c_sub_edge;
        current_best = 1e9;
    }

    void calculate_GED();
    int* get_way() {return way;}
    int get_GED() {return current_best;}



};
