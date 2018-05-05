//
// Created by gonglinyuan on 2018-05-04.
//

#ifndef GED_MODEL_H
#define GED_MODEL_H


#include <vector>
#include <algorithm>
#include "Graph.h"

class Model {
public:
    const int c_node_sub, c_node_ins, c_edge_sub, c_edge_ins;
    const Graph &g1, &g2;

    Model(int c_node_sub_, int c_node_ins_, int c_edge_sub_, int c_edge_ins_, const Graph &g1_, const Graph &g2_)
            : c_node_sub(std::min(c_node_sub_, 2 * c_node_ins_)),
              c_node_ins(c_node_ins_),
              c_edge_sub(std::min(c_edge_sub_, 2 * c_edge_ins_)),
              c_edge_ins(c_edge_ins_),
              g1(g1_), g2(g2_) {
    }

    std::pair<int, std::vector<int> > solve() const;

    int check_ans(std::vector<int> perm) const;

private:
    int node_sub_cost(int i, int j) const;

    int edge_sub_cost(int i, int j) const;

    int other_costs() const;
};


#endif //GED_MODEL_H
