//
// Created by jiry on 18-5-9.
//

#ifndef GED_IPSOLVER_H
#define GED_IPSOLVER_H

#endif //GED_IPSOLVER_H

#include "Simplex.h"

class IPSolver {
    Simplex baseSimplex, currentSimplex;
    int n;
    int way[KmaxN];
    int current_way[KmaxN];
    int time_limit;
    int total_iterator;
    double current_best;
    clock_t start;

    void trydfs(int location);
    int check_end();

public:
    IPSolver(int _n, int _TL): baseSimplex(_n), n(_n), currentSimplex(_n),
                               time_limit(_TL), total_iterator(0) {};
    void setcondition(double *x, double lim) {
        baseSimplex.setcondition(x, lim);
    }
    void setmaximal(double *x) {
        baseSimplex.setmaximal(x);
    }
    double solve();
    double getans() {return current_best;}
    int* getway() {return way;}
};
