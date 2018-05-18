//
// Created by jiry on 18-5-12.
//

#include "GEDSolver.h"

#include <cassert>
#include <cmath>

void my_pause() {
    int k1;
    scanf("%d", &k1);
}

int GEDSolver::edge_sub_cost(int i, int j) {
    return g1.e[i].type == g2.e[j].type ? 0 : cost_sub_edge;
}

int GEDSolver::node_sub_cost(int i, int j) {
    return g1.label[i] == g2.label[j] ? 0 : cost_sub_node;
}

int GEDSolver::other_costs() const {
    return (g1.n + g2.n) * cost_ins_node + (int) (g1.e.size() + g2.e.size()) * cost_ins_edge;
}

int GEDSolver::check_TL() {
    return (clock() - start) / CLOCKS_PER_SEC <= time_limit - 0.5;
}

void GEDSolver::get_lower_bound_for_candidate(GEDSolver::candidate_solution &candidate) {
    // printf("calculate lower bound\n");
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
            assert(used[candidate.node_state[i]] == 0);
            used[candidate.node_state[i]] = 1;
            init_value_x[i][candidate.node_state[i]] = 1;
        }

    for (int i = candidate.depth + 1; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j)
            if (used[j] == 0) x[i][j] = ++total_var;
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) y[i][j] = ++total_var;
    }

    simplex_solver.clear(total_var);
    for (int i = 0; i <= total_var; ++i) aim[i] = 0;

    for (int i = 1; i <= g1.n; ++i) {
        for (int j = 1; j <= g2.n; ++j) {
            if (x[i][j] == -1) {
                aim[0] += init_value_x[i][j] * (2 * cost_ins_node - node_sub_cost(i, j));
            } else aim[x[i][j]] = 2 * cost_ins_node - node_sub_cost(i, j);
        }
    }
    for (int i = 0; i < g1.e.size(); ++i) {
        for (int j = 0; j < g2.e.size(); ++j) {
            aim[y[i][j]] = 2.0 * cost_ins_edge - 1.0 * edge_sub_cost(i, j);
        }
    }
    // printf("%lf\n",aim[0]);
    for (int i = candidate.depth + 1; i <= g1.n; ++i) {
        for (int idx = 0; idx <= total_var; ++idx) cons[0][idx] = 0;
        for (int j = 1; j <= g2.n; ++j) {
            if (x[i][j] != -1) cons[0][x[i][j]] = 1;
            else
                assert(init_value_x[i][j] == 0);
        }
        simplex_solver.setcondition(cons[0], 1);
    }
    for (int j = 1; j <= g2.n; ++j) {
        if (used[j]) continue;
        for (int idx = 0; idx <= total_var; ++idx) cons[0][idx] = 0;
        for (int i = 1; i <= g1.n; ++i) {
            if (x[i][j] != -1) cons[0][x[i][j]] = 1;
        }
        simplex_solver.setcondition(cons[0], 1);
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
            simplex_solver.setcondition(cons[k], cons[k][0]);
        }
    }

    simplex_solver.setmaximal(aim);
    // printf("current solve\n"); simplex_solver.print();
    candidate.result = simplex_solver.getresult();
    assert(candidate.result != UNBOUNDED);
    if (candidate.result != INFEASIBLE) {
        // printf("%lf %lf %d\n", simplex_solver.get_answer(), aim[0], other_costs());
        candidate.value = -simplex_solver.get_answer() - aim[0] + other_costs();
    }

    //printf("calculate value for candidate\n");
    //candidate.print();
    //double *way = simplex_solver.get_way();
    //for (int i = 1; i <= total_var; ++i) printf("%.3lf ",way[i]); puts("");
}

void GEDSolver::get_final_value_for_candidate(GEDSolver::candidate_solution &candidate) {
    // printf("get final value\n");
    // candidate.print();
    static int used_node[MAXV], ans, used_edge[MAXE];
    memset(used_node, 0x00, sizeof used_node);
    memset(used_edge, 0x00, sizeof used_edge);
    ans = 0;
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
        int node1 = candidate.node_state[edge.x];
        int node2 = candidate.node_state[edge.y];
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
        if (used_edge[i] == 0) ans += cost_ins_edge;

    candidate.value = ans;
}

void GEDSolver::extend(GEDSolver::candidate_solution pre, std::vector<candidate_solution> &list) {
    static int used[MAXV];
    // pre.print();
    memset(used, 0x00, sizeof used);
    pre.depth++;
    for (int i = 1; i < pre.depth; ++i)
        if (pre.node_state[i] != -1) used[pre.node_state[i]] = 1;

    pre.node_state[pre.depth] = -1;
    if (pre.depth == g1.n) {
        get_final_value_for_candidate(pre);
    } else {
        get_lower_bound_for_candidate(pre);
    }
    if (pre.result != INFEASIBLE && pre.value < current_best)
        list.push_back(pre);

    for (int i = 1; i <= g2.n; ++i) {
        if (used[i]) continue;
        pre.node_state[pre.depth] = i;
        if (pre.depth == g1.n) {
            get_final_value_for_candidate(pre);
        } else {
            get_lower_bound_for_candidate(pre);
        }
        if (pre.result != INFEASIBLE && pre.value < current_best)
            list.push_back(pre);
    }
}

void GEDSolver::solve(int width) {
    // printf("solve %d\n", width);
    list[0].clear();
    candidate_solution base;
    get_lower_bound_for_candidate(base);
    list[0].push_back(base);
    int now_list = 0;
    for (int idx = 1; idx <= g1.n; ++idx) {
        // printf("current depth %d\n", idx);
        if (list[now_list].size() == 0 || check_TL() == 0) {
            return;
        }
        //printf("%d %.10lf\n", idx, list[now_list][0].value);
        int next_list = now_list ^1;
        list[next_list].clear();
        for (const auto &candidate: list[now_list]) {
            extend(candidate, list[next_list]);
        }
        std::sort(list[next_list].begin(), list[next_list].end());
        //for (auto k: list[next_list]) k.print();
        //my_pause();
        list[next_list].resize(std::min(list[next_list].size(), (size_t)width));
        now_list = next_list;
    }
    if (list[now_list].size() == 0) return;
    // printf("%d\n", int(list[now_list].size()));
    candidate_solution best_candidate = list[now_list][0];
    current_best = best_candidate.value;
    for (int i = 1; i <= g1.n; ++i) {
        way[i] = best_candidate.node_state[i];
    }
    get_final_value_for_candidate(best_candidate);
}

void GEDSolver::get_search_trace(std::vector<int> way) {
    printf("%d %d %d %d\n", cost_ins_node, cost_sub_node, cost_ins_edge, cost_sub_edge);
    printf("g1\n");
    g1.print();
    printf("g2\n");
    g2.print();
    candidate_solution test;
    for (int i = 0; i < way.size(); ++i) {
        test.depth += 1;
        test.node_state[test.depth] = way[i];
        get_lower_bound_for_candidate(test);
        printf("current\n");
        test.print();
    }
    get_final_value_for_candidate(test);
    printf("final\n");
    test.print();
    my_pause();
}

void GEDSolver::calculate_GED() {
    start = clock();
    // printf("start calculate\n");
    // get_search_trace({1, 6, 5, 3, 8, 7});
    // printf("%d\n", g1.n * g2.n + int(g1.e.size() * g2.e.size()));
    int width = 1;
    for (int i = 1; i <= g1.n; ++i) way[i] = -1;
    while (width <= 1e6 && check_TL()) {
        solve(width);
        width += ceil(width / 2.0);
        // printf("%f %d\n", current_best, check_TL());
    }
}