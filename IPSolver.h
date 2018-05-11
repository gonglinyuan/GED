//
// Created by jiry on 18-5-9.
//

#ifndef GED_IPSOLVER_H
#define GED_IPSOLVER_H

#endif //GED_IPSOLVER_H

#include "Simplex.h"

#include <bitset>

class candidate_solution {
public:
    std::bitset<KmaxN> way;
    int depth;
    double value;
    SimplexResult result;

    int operator < (const candidate_solution& k1) const {
        if (result == INFEASIBLE) return 0;
        if (k1.result == INFEASIBLE) return 1;
        return value < k1.value;
    }
    int operator > (const candidate_solution& k1) const {
        return k1 < (*this);
    }

    candidate_solution(): way(0), depth(0), value(0), result(INFEASIBLE) {}
};

class IPSolver {
    Simplex baseSimplex, currentSimplex;
    int n;
    int way[KmaxN];
    int time_limit;
    double current_best;
    clock_t start;
    std::vector<candidate_solution> list[2];

    void get_value_for_candidate(candidate_solution& current);
    void extend_candidate(const candidate_solution& candidate, int list_id);
    int check_TL();
    void solve(int width);

public:
    IPSolver(int _n, int _TL): baseSimplex(_n), n(_n), currentSimplex(_n),
                               time_limit(_TL), start(clock()), current_best(-INF) {};

    void setcondition(double *x, double lim) {
        baseSimplex.setcondition(x, lim);
    }

    void setmaximal(double *x) {
        baseSimplex.setmaximal(x);
    }

    double calculate();
    double getans() {return current_best;}
    int* getway() {return way;}
};
