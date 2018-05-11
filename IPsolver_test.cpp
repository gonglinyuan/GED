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

void gentest() {
    int way[n + 10];
    for (int i = 1; i <= n; ++i) way[i] = rand() % 2;
    for (int i = 1; i <= n; ++i) printf("%d", way[i]); puts("");
    for (int i = 1; i <= n; ++i) A[0][i] = rand() % 10 - 5;
    for (int i = 1; i <= m; ++i) {
        A[i][0] = 0;
        for (int j = 1; j <= n; ++j) {
            A[i][j] = rand() % 10 - 5;
            A[i][0] += A[i][j] * way[j];
        }
        A[i][0] += rand() % 5;
    }
}

int main() {
    srand(time(0));
    printf("start\n");
    scanf("%d%d", &n, &m);
    /*for (int i = 1; i <= n; ++i) scanf("%d", &A[0][i]);
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) scanf("%d", &A[i][j]);
        scanf("%d", &A[i][0]);
    }*/
    gentest();
    for (int i = 1; i <=n; ++i) {
        printf("%d ", A[0][i]);
    } puts("");
    for (int i = 1; i <= m; ++i) {
        for (int j = 0; j <= n; ++ j)
            printf("%d ",A[i][j]);
        printf("%d\n");
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