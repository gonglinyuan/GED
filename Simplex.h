//
// Created by jiry on 18-5-9.
//

#ifndef GED_SIMPLEX_H
#define GED_SIMPLEX_H

#endif //GED_SIMPLEX_H


#include <cstring>
#include <algorithm>
#include <iostream>
#include <cassert>

#define EPS 1e-8
#define INF 1e100
#define MAXM (4000+5)
#define MAXN (2000+5)

enum SimplexResult {
    INFEASIBLE, UNBOUNDED, BOUNDED
};

class Simplex {
    double way[MAXN];
    int idm[MAXM], idn[MAXN];
    double ans, bias;
    double* a[MAXM];
    int m, n;

    void pivot(int l, int e);
    bool solve();

public:
    Simplex(int _n):n(_n), ans(-1e100), m(_n), bias(0) {
        for (int i = 0; i < MAXM; ++i) a[i] = nullptr;
        a[0] = new double[n + 5];
        for (int i = 0; i <= n + 1; ++i) a[0][i] = 0;
        for (int i = 1; i <= n; ++i) {
            a[i] = new double[n + 5];
            for (int j = 0; j <= n + 1; ++j) a[i][j] = 0;
            a[i][i] = -1;
            a[i][0] = 1;
        }
    }

    Simplex(): n(0), ans(-1e100), m(0), bias(0) {
        for (int i = 0; i < MAXM; ++i) a[i] = nullptr;
    }

    void clear(int n);
    void copy(const Simplex& pre);
    double get_value_from_way(int* current_way);
    SimplexResult getresult();
    void setcondition(double *x, double b);
    void setmaximal(double *x);
    double get_answer() {return ans + bias;}
    double* get_way();
    int getm(){return m;}
    bool setvar(int idx, int w);
};