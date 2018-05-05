//
// Created by gonglinyuan on 2018-05-04.
//

#include <cassert>
#include "Model.h"
#include "gurobi_c++.h"
#include "main.h"

using std::vector;
using std::to_string;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

pair<int, vector<int>> Model::solve() const {
    GRBEnv env = GRBEnv();
#ifndef DEBUG
    env.set(GRB_IntParam_OutputFlag, 0);
#endif
    GRBModel model = GRBModel(env);
//    GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
//    GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
//    GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");
//    model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
//    model.addConstr(x + 2 * y + 3 * z <= 4, "c0");
//    model.addConstr(x + y >= 1, "c1");
//    model.optimize();
    GRBVar x[g1.n + 1][g2.n + 1], y[g1.e.size()][g2.e.size()];
    GRBLinExpr obj = other_costs();
    model.set(GRB_DoubleParam_TimeLimit, 25.0); // 25 seconds
    for (int i = 1; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j) {
            x[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_" + to_string(i) + "_" + to_string(j));
            obj += x[i][j] * (1.0 * node_sub_cost(i, j) - 2.0 * c_node_ins);
        }
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) {
            y[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y_" + to_string(i) + "_" + to_string(j));
            obj += y[i][j] * (1.0 * edge_sub_cost(i, j) - 2.0 * c_edge_ins);
        }
    }
    for (int i = 1; i <= g1.n; ++i) {
        GRBLinExpr lhs = 0;
        for (int j = 1; j <= g2.n; ++j) {
            lhs += x[i][j];
        }
        model.addConstr(lhs, GRB_LESS_EQUAL, 1.0, "c0_" + to_string(i));
    }
    for (int j = 1; j <= g2.n; ++j) {
        GRBLinExpr lhs = 0;
        for (int i = 1; i <= g1.n; ++i) {
            lhs += x[i][j];
        }
        model.addConstr(lhs, GRB_LESS_EQUAL, 1.0, "c1_" + to_string(j));
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        GRBLinExpr lhs[g2.n + 1] = {0};
        for (int j = 0; j < g2.e.size(); ++j) {
            lhs[g2.e[j].x] += y[i][j];
            lhs[g2.e[j].y] += y[i][j];
        }
        for (int k = 1; k <= g2.n; ++k) {
            model.addConstr(lhs[k], GRB_LESS_EQUAL, x[g1.e[i].x][k] + x[g1.e[i].y][k],
                            "c2_" + to_string(i) + "_" + to_string(k));
        }
    }
    model.setObjective(obj, GRB_MINIMIZE);
    model.optimize();
    vector<int> res;
#ifdef DEBUG
//    cout << other_costs() << endl;
//    double tmp = 1.0 * other_costs();
//    cout << tmp << endl;
//    for (int i = 1; i <= g1.n; ++i) {
//        for (int j = 1; j <= g2.n; ++j) {
//            if (x[i][j].get(GRB_DoubleAttr_X) > 0.5) {
//                cout << "x " << i << ' ' << j << ' ' << x[i][j].get(GRB_DoubleAttr_X) << endl;
//                tmp += x[i][j].get(GRB_DoubleAttr_X) * (1.0 * node_sub_cost(i, j) - 2.0 * c_node_ins);
//            }
//        }
//    }
//    cout << tmp << endl;
//    for (int i = 0; i < g1.e.size(); ++i) {
//        for (int j = 0; j < g2.e.size(); ++j) {
//            if (y[i][j].get(GRB_DoubleAttr_X) > 0.5) {
//                cout << "y " << i << ' ' << j << ' ' << y[i][j].get(GRB_DoubleAttr_X) << endl;
//                tmp += y[i][j].get(GRB_DoubleAttr_X) * (1.0 * edge_sub_cost(i, j) - 2.0 * c_edge_ins);
//            }
//        }
//    }
//    cout << tmp << endl;
#endif
    for (int i = 1; i <= g1.n; ++i) {
        bool erased = true;
        for (int j = 1; j <= g2.n; ++j) {
//            cout << i << ' ' << j << ' ' << x[i][j].get(GRB_DoubleAttr_X) << endl;
            if (x[i][j].get(GRB_DoubleAttr_X) > 0.5) {
                res.push_back(j);
                erased = false;
                break;
            }
        }
        if (erased) {
            res.push_back(-1);
        }
    }
    int code;
    if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
        code = 1;
    } else {
        code = 2;
    }
    return make_pair(code, res);
}

int Model::node_sub_cost(int i, int j) const {
    return g1.label[i] == g2.label[j] ? 0 : c_node_sub;
}

int Model::edge_sub_cost(int i, int j) const {
    return g1.e[i].type == g2.e[j].type ? 0 : c_edge_sub;
}

int Model::other_costs() const {
    return (g1.n + g2.n) * c_node_ins + (int) (g1.e.size() + g2.e.size()) * c_edge_ins;
}

int Model::check_ans(std::vector<int> perm) const {
    assert(perm.size() == g1.n);
    int p[g1.n + 1] = {0}, r[g2.n + 1] = {0};
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
    bool marked[g2.e.size()] = {0};
    for (int i = 0; i < g1.e.size(); ++i) {
        auto ed = g1.e[i];
        if (p[ed.x] != -1 && p[ed.y] != -1) {
            for (int j = 0; j < g2.e.size(); ++j) {
                auto e2 = g2.e[j];
                if ((e2.x == p[ed.x] && e2.y == p[ed.y]) || (e2.x == p[ed.y] && e2.y == p[ed.x])) {
                    assert(!marked[j]);
                    marked[j] = true;
                    res += edge_sub_cost(i, j);
                    break;
                }
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
