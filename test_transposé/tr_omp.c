#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

int n=1024;
struct timeval begin, end;


int* transpose(int *A)
{
    int *B = (int*) malloc(sizeof(int)*n*n);
    #pragma omp parallel for
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
int* opt(int *A,int  *B)
{

    int *C = (int*) malloc(sizeof(int)*n*n);

    int i,j,k;
 
     
  

    //double start = rclock();
    gettimeofday (&begin, NULL);
      B = transpose(B);

    #pragma omp parallel for
        for (i=0; i<n; i++)
            for (j=0; j<n; j++)
            {
                C[i*n+j] = 0;

                for (k=0; k<n; k++)
                    C[i*n+j] += A[i*n+k] * B[j*n+k];
            }
    gettimeofday (&end, NULL);
    double elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    
    printf(" runtime  opt: %f", elapsed);
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
    int i,j,k;

    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
            A[i*n+j] = rand() % 50;
            B[i*n+j] = rand() % 50;
        }

    int *C1 = non_opt(A,B);
    int *C2= opt(A,B);
    compare_matrix(C1,C2); 
}