//
// Created by jiry on 18-5-9.
//

#include "IPSolver.h"

#include <cmath>
#include <cassert>
#include <iostream>

double Kminpossible = -1e100;

int IPSolver::check_TL() {
    return (clock() - start) / CLOCKS_PER_SEC <= time_limit - 0.5;
}

void IPSolver::get_value_for_candidate(candidate_solution &current) {
    currentSimplex = baseSimplex;
    for (int idx = 1; idx < current.depth; ++idx) {
        currentSimplex.setvar(idx, current.way[idx]);
    }
    current.result = currentSimplex.getresult();
    assert(current.result != UNBOUNDED);
    if (current.result == BOUNDED) {
        current.value = currentSimplex.get_answer();
    }
}

void IPSolver::extend_candidate(const candidate_solution& candidate, int list_id) {
    candidate_solution current = candidate;
    current.depth += 1;
    current.way[current.depth] = 0;
    get_value_for_candidate(current);
    // printf("extend %.11lf ", current.value);
    if (current.result != INFEASIBLE && current.value > current_best + fabs(current_best) * 0.01) {
        list[list_id].push_back(current);
    }
    current.way[current.depth] = 1;
    get_value_for_candidate(current);
    // printf("%.11lf\n", current.value);
    if (current.result != INFEASIBLE && current.value > current_best + fabs(current_best) * 0.01) {
        list[list_id].push_back(current);
    }
}

void IPSolver::solve(int width) {
    // printf("start solve %d\n", width);
    list[0].clear();
    list[0].push_back(candidate_solution());
    int now_list = 0;
    for (int idx = 1; idx <= n; ++idx) {
        // printf("%d\n", idx);
        if (list[now_list].size() == 0 || check_TL() == 0) {
            return;
        }
        int next_list = now_list ^ 1;
        list[next_list].clear();
        for (const auto& candidate: list[now_list]) {
            extend_candidate(candidate, next_list);
        }
        std::sort(list[next_list].begin(), list[next_list].end(), std::greater<candidate_solution>());
        list[next_list].resize(std::min(int(list[next_list].size()), width));
        now_list = next_list;
    }
    if (list[now_list].size() == 0) return;
    candidate_solution best_candidate = list[now_list][0];
    current_best = best_candidate.value;
    // printf("%.11lf\n", current_best);
    for (int i = 1; i <= n; ++i) {
        way[i] = best_candidate.way[i];
        // printf("%d", way[i]);
    }
    // puts("");
}

double IPSolver::calculate() {
    int width = 1;
    // printf("var %d, cons %d\n", n, baseSimplex.getm());
    while (width <= 1e6 && check_TL()) {
        solve(width);
        width += std::ceil(sqrt(width));
    }
    return current_best;
}