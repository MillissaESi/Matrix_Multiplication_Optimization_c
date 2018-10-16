#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <omp.h>
#include <sys/time.h>

int n=1024;
struct timeval begin, end;

int* non_opt(int *A,int  *B)
{
    int *C = (int*) malloc(sizeof(int)*n*n);
    int i,j,k;

    
    //double start = rtclock();
    gettimeofday (&begin, NULL);

    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
            C[i*n+j] = 0;

            for (k=0; k<n; k++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
        }

    /*double end = rtclock();
    printf("clock opt %lf\n", end - start);*/
    gettimeofday (&end, NULL);
    double elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf(" runtime non opt: %f\n", elapsed);

    return C;
}

int* tiling2d(int *A,int  *B, int tbloc)
{
    int *C = (int*) malloc(sizeof(int)*n*n);

    int i1,j1,i0,j0,k;


    //double start = rtclock();
    gettimeofday (&begin, NULL);

    for (i0=0; i0<n/tbloc; i0++)
        for (j0=0; j0<n/tbloc; j0++)
            for (i1=32*i0; i1<tbloc*i0+tbloc; i1++)
                for (j1=32*j0; j1<tbloc*j0+tbloc; j1++)
                {
                    C[i1*n+ j1] = 0;
                    for (k=0;k<n;k++)
                        C[i1*n+ j1] += A[i1*n+ k] * B[k*n+ j1];
                }

    /*double end = rtclock();
    printf("clock opt %lf\n", end - start);*/
    gettimeofday (&end, NULL);
    double elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf(" runtime opt: %f\n", elapsed);

    return C;
}
/*
static double rtclock()
{
 struct timeval Tp;
 gettimeofday (&Tp, NULL);
 return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}*/


int compare_matrix(int *A, int* B){
    int i, j;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
           if( A[i*n+j]!=B[i*n+j]){
            printf("unequal\n");
            return -1;
           }
        }
    printf("equal\n");
    return 0;
}

int main(){

    int *A = (int*) malloc(sizeof(int)*n*n);
    int *B = (int*) malloc(sizeof(int)*n*n);
    int i,j,k;

    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
            A[i*n+j] = rand() % 50;
            B[i*n+j] = rand() % 50;
        }


    int *C1 = non_opt(A,B);
    int *C2= tiling2d(A,B,32);
    compare_matrix(C1,C2);
}