#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

int n=1024;
struct timeval begin, end;

/* transpose */
int* transpose(int *A)
{
    int *B = (int*) malloc(sizeof(int)*n*n);
    //#pragma omp parallel for
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            B[i*n+j]=A[j*n+i];
        }
    }
    return B;
}

/* Fonction non optimisée */
int* non_opt(int *A,int  *B)
{
   
    int *C = (int*) malloc(sizeof(int)*n*n);
    int i,j,k;
  
    //double start = rclock();
    gettimeofday (&begin, NULL);

   // B = transpose(B);
   
    for (i=0; i<n; i++)

        for (j=0; j<n; j++)
            {
                C[i*n+j] = 0;

                for (k=0; k<n; k++)
                    C[i*n+j] += A[i*n+k] * B[k*n+j];
            }

    gettimeofday (&end, NULL);
    double elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    
    printf(" runtime non opt: %f", elapsed);

    return C;
}

/* fonction optimisée */
int* opt(int *A,int  *B,int tblock)
{

    int *C = (int*) malloc(sizeof(int)*n*n);
    gettimeofday (&begin, NULL);
    int i,j,k;
    int i0, i1, j0 ,j1,k0; 
    //B = transpose(B);
    //double start = rclock();

    #pragma omp parallel for private(i,j)
    for (i=0; i<n; i++)
        for(j=0; j<n; j++)
            C[i*n+j]=0;

    #pragma omp parallel for private(i0,j0,i1,j1,k,k0)
    for (i0=0; i0<n/tblock; i0++)
        for (j0=0; j0<n/tblock; j0++)
                for (k0=0; k0<n/tblock; k0++)
                    for (i1=tblock*i0; i1<tblock*i0+tblock; i1++)
                        for (j1=tblock*j0; j1<tblock*j0+tblock; j1++)
                            for (k=k0*tblock; k<k0*tblock+tblock; k++)
                                C[i1*n+j1] += A[i1*n+k] * B[k*n+j1];

    gettimeofday (&end, NULL);
    double elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    
    printf(" runtime opt: %f", elapsed);

    return C;
}


/* Compare */
int compare_matrix(int *A, int* B){
    int i, j;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
           if( A[i*n+j]!=B[i*n+j]){
            printf(" \n unequal \n");
            return -1;
           }
        }
    printf("\n equal \n");
    return 0;
}

int main(){

    int *A = (int*) malloc(sizeof(int)*n*n);
    int *B = (int*) malloc(sizeof(int)*n*n);
    int i,j;
    
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
            A[i*n+j] = rand() % 5;
            B[i*n+j] = rand() % 5;
        }

    int tblock = 32 ;
    int *C1 = non_opt(A,B);
    int *C2= opt(A,B, tblock);
    compare_matrix(C1,C2); 
}