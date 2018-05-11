//
// Created by jiry on 18-5-9.
//

#ifndef GED_SIMPLEX_H
#define GED_SIMPLEX_H

#endif //GED_SIMPLEX_H


#include <cstring>
#include <algorithm>

const int KmaxN = 500;
#define EPS 1e-8
#define INF 1e100
#define MAXM (300+5)
#define MAXN (300+5)

enum SimplexResult {
    INFEASIBLE, UNBOUNDED, BOUNDED
};

class Simplex {
    double a[MAXM][MAXN];
    double way[MAXN];
    int idm[MAXM], idn[MAXN];
    int m, n;
    double ans, bias;

    void pivot(int l, int e);
    bool solve();

public:
    Simplex(int _n):n(_n), ans(-1e100), m(_n), bias(0) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j <= n + 1; ++j) a[i][j] = 0;
            a[i][i] = -1;
            a[i][0] = 1;
        }
    }

    SimplexResult getresult();
    void setcondition(double *x, double b);
    void setmaximal(double *x);
    double get_answer() {return ans + bias;}
    double* get_way();
    int getm(){return m;}
    void setvar(int idx, int w);
};