//
// Created by jiry on 18-5-12.
//

#include "GEDSolver.h"

#include <cassert>

int GEDSolver::edge_sub_cost(int i, int j) {
    return g1.e[i].type == g2.e[j].type ? 0 : cost_sub_edge;
}

int GEDSolver::node_sub_cost(int i, int j) {
    return g1.label[i] == g2.label[j] ? 0 : cost_sub_node;
}

int GEDSolver::other_costs() const {
    return (g1.n + g2.n) * c_node_ins + (int) (g1.e.size() + g2.e.size()) * c_edge_ins;
}

void GEDSolver::get_lower_bound_for_candidate(GEDSolver::candidate_solution &candidate) {
    static int x[MAXV][MAXV], y[MAXE][MAXE], used[MAXV];
    static double aim[MAXN], cons[MAXV][MAXN];
    static int init_value_x[MAXV][MAXV];
    int total_var = 0;
    memset(used, 0x00, sizeof used);
    memset(x, 0xff, sizeof x);
    memset(y, 0xff, sizeof y);
    memset(init_value_x, 0x00, sizeof init_value_x);
    for (int i = 1; i <= candidate.depth; ++i)
        if (candidate.node_state[i] != -1) {
            used[candidate.node_state[i]] = 1;
            init_value_x[candidate.node_state[i]] = 1;
        }

    for (int i = candidate.depth; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j) x[i][j] = ++total_var;
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) y[i][j] = ++total_var;
    }

    simplex_solver.clear(total_var);
    for (int i = 0; i <= total_var; ++i) aim[i] = 0;

    for (int i = 1; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j) {
            if (x[i][j] == -1) {
                aim[0] += init_value_x[i][j] * cost_ins_node - node_sub_cost(i, j);
            } else aim[x[i][j]] = cost_node_ins - node_sub_cost(i, j);
        }
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) {
            aim[y[i][j]] = 2.0 * c_edge_ins - 1.0 * edge_sub_cost(i, j);
        }
    }
    for (int i = candidate.depth + 1; i <= g1.n; ++i) {
        for (int idx = 0; idx <= total_var; ++idx) cons[0][idx] = 0;
        for (int j = 1; j <= g2.n; ++j) {
            if (x[i][j] != -1) cons[0][x[i][j]] = 1;
        }
        solver.setcondition(cons[0], 1);
    }
    for (int j = 1; j <= g2.n; ++j) {
        if (used[j]) continue;
        for (int idx = 0; idx <= total_var; ++idx) cons[0][idx] = 0;
        for (int i = 1; i <= g1.n; ++i) {
            if (x[i][j] != -1) cons[0][x[i][j]] = 1;
        }
        solver.setcondition(cons, 1);
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 1; j <= g2.n; ++j)
            for (int idx = 0; idx <= total_var; ++idx) {
                cons[j][idx] = 0;
            }
        for (int j = 0; j < g2.e.size(); ++j) {
            cons[g2.e[j].x][y[i][j]] += 1;
            cons[g2.e[j].y][y[i][j]] += 1;
        }
        for (int k = 1; k <= g2.n; ++k) {
            if (x[g1.e[i].x][k] != -1) {
                cons[k][x[g1.e[i].x][k]] -= 1;
            } else cons[k][0] += init_value_x[g1.e[i].x][k];
            if (x[g1.e[i].y][k] != -1) {
                cons[k][x[g1.e[i].y][k]] -= 1;
            } else cons[k][0] += init_value_x[g1.e[i].y][k];
            solver.setcondition(cons[k], 0);
        }
    }

    candidate.result = simplex_solver.getresult();
    assert(candidate.result != UNBOUNDED);
    if (candidate.result != INFEASIBLE)
        candidate.value = -simplex_solver.get_answer() + aim[0] + other_costs();
}

void GEDSolver::get_final_value_for_candidate(GEDSolver::candidate_solution &candidate) {
    static int used_node[MAXV], ans = 0, used_edge[MAXE];
    memset(used_node, 0x00, sizeof used_node);
    memset(used_edge, 0x00, sizeof used_edge);
    for (int i = 1; i <= g1.n; ++i) {
        if (candidate.node_state[i] == -1) {
            ans += cost_ins_node;
        } else {
            assert(used_node[candidate.node_state[i]] == 0);
            used_node[candidate.node_state[i]] = 1;
            if (g1.label[i] != g2.label[candidate.node_state[i]])
                ans += cost_sub_node;
        }
    }
    for (int i = 1; i <= g2.n; ++i)
        if (used_node[i] == 0) ans += cost_ins_node;
    for (int i = 0; i < g1.e.size(); ++i) {
        auto edge = g1.e[i];
        int node1 = candidate.node_state[node1];
        int node2 = candidate.node_state[node2];
        if (node1 == -1 || node2 == -1) {
            ans += cost_ins_edge;
            continue;
        }
        int flag = 0;
        for (int j = 0; j < g2.e.size(); ++j) {
            if ((g2.e[j].x == node1 && g2.e[j].y == node2) || (g2.e[j].y == node1 && g2.e[j].x == node2)) {
                used_edge[j] = 1;
                ans += edge_sub_cost(i, j);
                flag = 1;
            }
        }
        if (flag == 0) ans += cost_ins_edge;
    }
    for (int i = 0; i < g2.e.size(); ++i)
        if (used[i] == 0) ans += cost_ins_edge;

    candidate.value = ans;
}

void GEDSolver::extend(GEDSolver::candidate_solution pre, vecotr <candidate_solution> &list) {
    static int used[MAXV];
    memset(used, 0x00, sizeof used);
    pre.depth++;
    for (int i = 1; i < pre.depth; ++i)
        if (pre.node_state[i] != -1) used[pre.node_state[i]] = 1;

    pre.node_state[pre.depth] = -1;
    if (pre.depth == n) {
        get_final_value_for_candidate(pre);
    } else get_lower_bound_for_candidate(pre);
    if (pre.result != INFEASIBLE && pre.result < current_ans * 0.99)
        list.push_back(pre);

    for (int i = 1; i <= g2.n; ++i) {
        if (used[i]) continue;
        pre.node_state[pre.depth] = i;
        if (pre.depth == n) {
            get_final_value_for_candidate(pre);
        } else get_lower_bound_for_candidate(pre);
        if (pre.result != INFEASIBLE && pre.result < current_ans * 0.99)
            list.push_back(pre);
    }
}

void GEDSolver::solve(int width) {
    list[0].clear();
    list[0].push_back(candidate_solution());
    int now_list = 0;
    for (int idx = 1; idx <= n; ++idx) {
        if (list[now_list].size() == 0 || check_TL() == 0) {
            return;
        }
        int next_list = now_list ^ 1;
        list[next_list].clear();
        for (const auto& candidate: list[now_list]) {
            extend_candidate(candidate, list[next_list]);
        }
        std::sort(list[next_list].begin(), list[next_list].end(), std::greater<candidate_solution>());
        list[next_list].resize(std::min(int(list[next_list].size()), width));
        now_list = next_list;
    }
    if (list[now_list].size() == 0) return;
    candidate_solution best_candidate = list[now_list][0];
    current_best = best_candidate.value;
    for (int i = 1; i <= n; ++i) {
        way[i] = best_candidate.way[i];
    }
    get_value_for_candidate(best_candidate);
}