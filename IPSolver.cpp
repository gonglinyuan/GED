//
// Created by jiry on 18-5-9.
//

#include "IPSolver.h"

#include <cmath>
#include <cassert>

double Kminpossible = -1e100;

int IPSolver::check_end() {
    return (clock() - start) / CLOCKS_PER_SEC <= time_limit - 0.5;
}

void IPSolver::trydfs(int location) {
    // printf("%d\n", location);
    // for (int i = 1; i < location; ++i) printf("%d", current_way[i]); puts("");
    if (check_end() == 0) {
        return;
    }
    currentSimplex = baseSimplex;
    for (int idx = 1; idx < location; ++idx) {
        currentSimplex.setvar(idx, current_way[idx]);
    }
    auto result = currentSimplex.getresult();
    if (result == UNBOUNDED) {
        printf("try dfs %d\n", location);
        for (int i = 1; i < location; ++i) {
            printf("%d ", current_way[i]);
        }
        puts("");
        printf("%d\n", currentSimplex.getm());
        while (true);
    }
    assert(result != UNBOUNDED);
    if (result == INFEASIBLE) return;
    if (result == BOUNDED) {
        double current_ans = currentSimplex.get_answer();
        // double* way = currentSimplex.get_way();
        // for (int i = 1; i <= n; ++i) printf("%.5lf ",way[i]); puts("");
        // printf("current_ans %lf\n", current_ans);
        if (current_ans < current_best + fabs(current_best) * 0.01) {
            return;
        }
    }
    if (location > n) {
        current_best = std::max(current_best, (double)currentSimplex.get_answer());
        memcpy(way, current_way, sizeof current_way);
        return;
    }
    current_way[location] = 0;
    trydfs(location + 1);
    current_way[location] = 1;
    trydfs(location + 1);
}

double IPSolver::solve() {
    start = clock();
    current_best = Kminpossible;
    trydfs(1);
}