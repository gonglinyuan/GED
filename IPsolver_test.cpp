//
// Created by jiry on 18-5-9.
//

#include "IPSolver.h"

int A[KmaxN][KmaxN];
double x[KmaxN];
int n, m;

double force() {
    double best_ans = -1e1000;
    for (int S = 0; S < (1 << n); ++S) {
        int is_valid = 1;
        for (int i = 1; i <= m; ++i) {
            int current_sum = 0;
            for (int j = 1; j <= n; ++j) {
                if (S & (1 << j - 1)) current_sum += A[i][j];
            }
            if (current_sum > A[i][0]) {
                is_valid = 0;
                break;
            }
        }
        if (is_valid) {
            int current_sum = 0;
            for (int i = 1; i <= n; ++i) {
                if (S & (1 << i - 1)) current_sum += A[0][i];
            }
            if (current_sum > best_ans) best_ans = current_sum;
        }
    }
    return best_ans;
}
int main() {
    printf("start\n");
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", &A[0][i]);
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) scanf("%d", &A[i][j]);
        scanf("%d", &A[i][0]);
    }
    IPSolver solver(n, 5);
    for (int i = 1; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            x[j] = A[i][j];
        }
        solver.setcondition(x, A[i][0]);
    }
    for (int j = 1; j <= n; ++j) x[j] = A[0][j];
    solver.setmaximal(x);
    solver.calculate();
    double ans=solver.getans();
    double force_ans = force();
    printf("%lf %lf\n", ans, force_ans);
}