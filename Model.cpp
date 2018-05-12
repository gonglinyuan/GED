//
// Created by gonglinyuan on 2018-05-04.
//

#include <cassert>
#include <iostream>
#include "Model.h"
#include "IPSolver.h"

using std::vector;
using std::to_string;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

pair<int, vector<int>> Model::solve() const {
//    GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
//    GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
//    GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");
//    model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
//     model.addConstr(x + 2 * y + 3 * z <= 4, "c0");
//    model.addConstr(x + y >= 1, "c1");
//    model.optimize();
    int x[g1.n + 1][g2.n + 1], y[g1.e.size()][g2.e.size()], total_var = 0, bias = 0;
    for (int i = 1; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j) {
            x[i][j] = ++total_var;
        }
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) {
            y[i][j] = ++total_var;
        }
    }
    IPSolver solver(total_var, 25);
    double aim[total_var + 1];
    for (int i = 1; i <= total_var; ++i) {
        aim[i] = 0;
    }
    bias = other_costs();
    for (int i = 1; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j) {
            // x[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_" + to_string(i) + "_" + to_string(j));
            // obj += x[i][j] * (1.0 * node_sub_cost(i, j) - 2.0 * c_node_ins);
            aim[x[i][j]] = 2.0 * c_node_ins - 1.0 * node_sub_cost(i, j);
        }
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) {
            // y[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y_" + to_string(i) + "_" + to_string(j));
            // obj += y[i][j] * (1.0 * edge_sub_cost(i, j) - 2.0 * c_edge_ins);
            aim[y[i][j]] = 2.0 * c_edge_ins - 1.0 * edge_sub_cost(i, j);
        }
    }
    for (int i = 1; i <= g1.n; ++i) {
        double cons[total_var + 1];
        for (int idx = 0; idx <= total_var; ++idx) cons[idx] = 0;
        /*for (int j = 1; j <= g2.n; ++j) {
            lhs += x[i][j];
        }
        model.addConstr(lhs, GRB_LESS_EQUAL, 1.0, "c0_" + to_string(i));*/
        for (int j = 1; j <= g2.n; ++j) {
            cons[x[i][j]] = 1;
        }
        solver.setcondition(cons, 1);
    }
    for (int j = 1; j <= g2.n; ++j) {
        double cons[total_var + 1];
        for (int idx = 0; idx <= total_var; ++idx) cons[idx] = 0;
        /*GRBLinExpr lhs = 0;
        for (int i = 1; i <= g1.n; ++i) {
            lhs += x[i][j];
        }
        model.addConstr(lhs, GRB_LESS_EQUAL, 1.0, "c1_" + to_string(j));*/
        for (int i = 1; i <= g1.n; ++i) {
            cons[x[i][j]] = 1;
        }
        solver.setcondition(cons, 1);
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        double cons[g2.n + 1][total_var];
        for (int j = 1; j <= g2.n; ++j)
            for (int idx = 0; idx <= total_var; ++idx) {
                cons[j][idx] = 0;
            }
        //GRBLinExpr lhs[g2.n + 1] = {0};
        for (int j = 0; j < g2.e.size(); ++j) {
            //lhs[g2.e[j].x] += y[i][j];
            //lhs[g2.e[j].y] += y[i][j];
            cons[g2.e[j].x][y[i][j]] += 1;
            cons[g2.e[j].y][y[i][j]] += 1;
        }
        for (int k = 1; k <= g2.n; ++k) {
            cons[k][x[g1.e[i].x][k]] -= 1;
            cons[k][x[g1.e[i].y][k]] -= 1;
            solver.setcondition(cons[k], 0);
            //model.addConstr(lhs[k], GRB_LESS_EQUAL, x[g1.e[i].x][k] + x[g1.e[i].y][k],
            //                "c2_" + to_string(i) + "_" + to_string(k));
        }
    }
    solver.setmaximal(aim);
    solver.calculate();
    //model.setObjective(obj, GRB_MINIMIZE);
    //model.optimize();
    int *way = solver.getway();
    std::cerr << -solver.getans() + bias << std::endl;
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
            //if (x[i][j].get(GRB_DoubleAttr_X) > 0.5) {
            if (way[x[i][j]] > 0.5) {
                res.push_back(j);
                erased = false;
                break;
            }
        }
        if (erased) {
            res.push_back(-1);
        }
    }
    int code = 1;
    // TODO(jiruyi) check whether the solver finds the global optimal.
    /*if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
        code = 1;
    } else {
        code = 2;
    }*/
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
