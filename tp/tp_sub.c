#include <stdio.h>
#include <stdlib.h>
 
void display_matrix(float **, int, int);
void free_matrix(float **, int, int);
float **alloc_matrix(int, int);
void read_matrix(float **, int, int);
void tp_phase1(float **, int, int);
int check_balance(float **, int, int);
 
void display_matrix(float **A, int m, int n)
{
    int i, j;
        printf("\n");
        for(i=0; i<m; i++)
        {
                for(j=0; j<n; j++)
                        printf("%7.2f ", A[i][j]);
                printf("\n");
        }
        printf("\n");
}
 
void free_matrix(float **A, int m, int n)
{
        int i;
        for(i=0; i<m; i++)
                free(A[i]);
        free(A);
        A = NULL;
}
 
float **alloc_matrix(int m, int n)
{
        int i;
        float **temp = (float **)calloc(m, sizeof(float *));
        for(i=0; i<m; i++)
                temp[i] = (float *)calloc(n, sizeof(float *));
        return temp;
}
 
void read_matrix(float **A, int m, int n)
{
        int i, j;
        for(i=0; i<m; i++)
        {
                for(j=0; j<n-1; j++)
                {
                        if(i<m-1)
                        {
                                //printf("enter the per unit transportation cost from source(%d) to destination(%d): ", i+1, j+1);
                                scanf("%f", &A[i][j]);
                        }
                        if(i==m-1)
                        {
                                //printf("enter the demand for destination %d: ", j+1);
                                scanf("%f", &A[m-1][j]);
                        }
                }
                if(i<m-1)
                {
                        //printf("enter the supply for source %d: ", i+1);
                        scanf("%f", &A[i][n-1]);
                }
        }
}
 
void tp_phase1(float **A, int m, int n)
{
        int i=0, j=0;
        float **S = alloc_matrix(m-1, n-1);
        printf("here\n");
        while(i<m-1 && j<n-1)
        {
                if(A[i][n-1] > A[m-1][j])//supply > demand
                {
                        S[i][j] = A[m-1][j];
                        A[i][n-1] -= A[m-1][j];//update supply
                        A[m-1][j] -= 0;//update demand
                        j++;//increment the column number
                }
                if(A[i][n-1] <= A[m-1][j])//supply <= demand
                {
                        S[i][j] = A[i][n-1];
                        A[m-1][j] -= A[i][n-1];//update demand
                        A[i][n-1] = 0;//update supply
                        i++;//increment the row number
                }
        }
        display_matrix(A, m, n);
        display_matrix(S, m-1, n-1);
}
 
int main()
{
        int m, n;
        float **A;
        //printf("enter the number of sources: ");
        scanf("%d", &m); m=m+1;//to include the demand row
        //printf("enter the number of destinations: ");
        scanf("%d", &n); n=n+1;//to include the supply column
        A = alloc_matrix(m, n);
        read_matrix(A, m, n);
        display_matrix(A, m, n);
        tp_phase1(A, m, n);
        return 0;
}
 