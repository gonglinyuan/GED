//
// Created by gonglinyuan on 2018-05-04.
//

#include <cassert>
#include <iostream>

#include "Model.h"
#include "IPSolver.h"
#include "GraphTreeSolver.h"

using std::vector;
using std::to_string;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

pair<int, vector<int> > Model::tree_solve() {
    GraphTreeSolver solver(g1, g2, c_edge_ins, c_edge_sub, c_node_ins, c_node_sub, 25);
    solver.solve();
    auto result = solver.get_ans();
    assert(result.first == check_ans(result.second));
    return make_pair(1, result.second);
}

int Model::node_sub_cost(int i, int j) const {
    return g1.label[i] == g2.label[j] ? 0 : c_node_sub;
}

int Model::edge_sub_cost(int i, int j) const {
    return g1.e[i].type == g2.e[j].type ? 0 : c_edge_sub;
}


int Model::check_ans(std::vector<int> perm) const {
    assert(perm.size() == g1.n);
    int p[g1.n + 1], r[g2.n + 1];
    memset(p, 0x00, sizeof p);
    memset(r, 0x00, sizeof r);
    int res = 0;
    for (int i = 1; i <= g1.n; ++i) {
        int j = perm[i - 1];
        p[i] = j;
        if (j == -1) {
            res += c_node_ins;
        } else {
            assert(r[j] == 0);
            r[j] = i;
            res += node_sub_cost(i, j);
        }
    }
    for (int j = 1; j <= g2.n; ++j) {
        if (r[j] == 0) {
            res += c_node_ins;
        }
    }
    bool marked[g2.e.size()];
    memset(marked, 0x00, sizeof marked);
    for (int i = 0; i < g1.e.size(); ++i) {
        auto ed = g1.e[i];
        if (p[ed.x] != -1 && p[ed.y] != -1) {
            bool found = false;
            for (int j = 0; j < g2.e.size(); ++j) {
                auto e2 = g2.e[j];
                if ((e2.x == p[ed.x] && e2.y == p[ed.y]) || (e2.x == p[ed.y] && e2.y == p[ed.x])) {
                    assert(!marked[j]);
                    marked[j] = true;
                    res += edge_sub_cost(i, j);
                    found = true;
                    break;
                }
            }
            if (!found) {
                res += c_edge_ins;
            }
        } else {
            res += c_edge_ins;
        }
    }
    for (int j = 0; j < g2.e.size(); ++j) {
        if (!marked[j]) {
            res += c_edge_ins;
        }
    }
    return res;
}
