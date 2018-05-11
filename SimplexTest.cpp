//
// Created by pro on 18/5/11.
//

#include<iostream>
#include<cmath>
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
    double ans;

    void pivot(int l, int e);
    bool solve();

public:
    Simplex(int _n):n(_n), ans(-1e100), m(0) {}

    SimplexResult getresult();
    void setcondition(double *x, double b);
    void setmaximal(double *x);
    double get_answer() {return ans;}
    double* get_way();
};

void Simplex::pivot(int l,int e){
    std::swap(idm[l],idn[e]);
    static int next[MAXN];
    int i,j,last=MAXN-1; double tmp=-a[l][e];a[l][e]=-1.0;
    for (j=0;j<=n;j++)if(fabs(a[l][j])>EPS)a[l][last=next[last]=j]/=tmp;
    next[last]=-1;
    for (i=0;i<=m;i++)if(i!=l && fabs(tmp=a[i][e])>EPS)
            for (a[i][e]=0.0,j=next[MAXN-1];~j;j=next[j])a[i][j]+=tmp*a[l][j];
}

bool Simplex::solve() {
    int e,l; double ma,tmp;
    while(1){
        e=n+1;idn[e]=n+m+1;
        for (int i=1;i<=n;i++)if(a[0][i]>EPS)
                if(idn[i]<idn[e])e=i;
        if(e==n+1) return true;
        l=m+1;idm[l]=n+m+1;ma=-INF;
        for(int i=1;i<=m;i++)if(a[i][e]<-EPS && ((tmp=a[i][0]/a[i][e])>ma+EPS || tmp>ma-EPS && idm[i]<idm[l]))ma=tmp,l=i;
        if(l==m+1) return false;
        pivot(l,e);
    }
}

SimplexResult Simplex::getresult(){
    for (int i=1;i<=n;i++)idn[i]=i;
    for (int i=1;i<=m;i++)idm[i]=i+n;
    static double tmp[MAXN];
    double mi=0.0;int l;
    for (int i=1;i<=m;i++)
        if(a[i][0]<mi)mi=a[i][0],l=i;
    if(mi>-EPS) {
        if (!solve()) return UNBOUNDED;
        ans = a[0][0];
        return BOUNDED;
    }

    idn[++n]=0;
    for (int i=1;i<=m;i++)a[i][n]=1.0;
    for (int i=0;i<=n;i++)tmp[i]=a[0][i],a[0][i]=0.0;
    a[0][n]=-1.0;

    pivot(l,n);
    if (!solve()) return UNBOUNDED;
    if(a[0][0]<-EPS) return INFEASIBLE;
    for (int i=1;i<=m;i++)if(idm[i]==0){
            for (int j=1;j<=n;j++)if(fabs(a[0][j])>EPS){
                    pivot(i,j);
                    break;
                }
            break;
        }
    int e;for (e=1;e<=n;e++)if(idn[e]==0)break;
    for (int i=0;i<=m;i++)a[i][e]=a[i][n];
    idn[e]=idn[n];n--;

    for (int i=0;i<=n;i++)a[0][i]=0.0;
    for (int i=1;i<=m;i++)if(idm[i]<=n){
            for (int j=0;j<=n;j++)a[0][j]+=a[i][j]*tmp[idm[i]];
        }
    for (int i=1;i<=n;i++)if(idn[i]<=n)a[0][i]+=tmp[idn[i]];
    if (!solve()) return UNBOUNDED;
    ans = a[0][0];
    return BOUNDED;
}

void Simplex::setcondition(double *x, double b) {
    ++m;
    for (int i = 0; i <= n + 1; ++i) a[m][i] = 0;
    for (int i = 1; i <= n; ++i) a[m][i] = -x[i];
    a[m][0] = b;
}

void Simplex::setmaximal(double *x) {
    for (int i = 0; i <= n + 1; ++i) a[0][i] = 0;
    for (int i = 1; i <= n; ++i) a[0][i] = x[i];
}

double* Simplex::get_way(){
    for (int i=1;i<=n;i++)way[i]=0.0;
    for (int i=1;i<=m;i++)if(idm[i]<=n) way[idm[i]]=a[i][0];
    return way;
}

int main()
{
    int ta,n,m;
    scanf("%d%d%d",&n,&m,&ta);
    Simplex solver(n);
    double x[MAXM];
    for (int i=1;i<=n;i++) scanf("%lf",&x[i]);
    solver.setmaximal(x);
    for (int i=1;i<=m;i++){
        for (int j=1;j<=n;j++){
            scanf("%lf",&x[j]);
        }
        scanf("%lf",&x[0]);
        solver.setcondition(x, x[0]);
    }
    SimplexResult res = solver.getresult();
    if (res == INFEASIBLE) {
        puts("Infeasible");
        return 0;
    } else if (res == UNBOUNDED) {
        puts("Unbounded");
        return 0;
    }
    printf("%.10lf\n",double(solver.get_answer()));
    //printf("%e\n",double(solve()));
    if(ta){
        double *ans = solver.get_way();
        for (int i=1;i<=n;i++) printf("%.10lf ",fabs(double(ans[i])));
    }
    return 0;
}