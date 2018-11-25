#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
    /*点列の入力*/
    int N;
    scanf("%d",&N);
    double *x;
    x = (double*)malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) {
        scanf("%lf", &x[i]);
    }
    double *delta;
    delta = (double*)malloc((N-1) * sizeof(double));
    for(int i=0;i<N-1;i++){
        delta[i]=x[i+1]-x[i];
    }
    double *m;
    m = (double*)malloc(N * sizeof(double));
    for(int i=1;i<N-1;i++){
        if(delta[i-1]*delta[i]<0) m[i]=0;
        else m[i]=(delta[i-1]+delta[i])/2;
    }
    m[0]=delta[0];
    m[N]=delta[N-1];
    for(int i=0;i<N;i++){
        if(delta[i]==0){
            m[i]=0;
            m[i+1]=0;
        }
    }
    double a,b;
    for(int i=0;i<(N-1);i++){
        if(delta[i]!=0){
            a=m[i]/delta[i];
            b=m[i+1]/delta[i];
            double tau=3/sqrt(a*a+b*b);
            if(a*a+b*b>9.0){
                printf("a");
                m[i]=tau*a*delta[i];
                m[i+1]=tau*b*delta[i];
            }
        }
    }
    double n;
    for(int i=0;i<=10*(N-1);i++){
        n=i/10.0;
        int ind=n;
        n-=ind;
        double out;
        double h00,h10,h01,h11;
        h00=(1+2*n)*(1-n)*(1-n);
        h01=n*n*(3-2*n);
        h10=n*(1-n)*(1-n);
        h11=n*n*(n-1);
        out=x[ind]*h00+m[ind]*h10+x[ind+1]*h01+m[ind+1]*h11;
        printf("%lf\n",out);
    }
}
