#include <stdio.h>
#include <stdlib.h>

#define M 0

//display 1-D array of type float
void display1(float *, int);

//display 2-D array of type float
void display2(float **, int, int);

//pqrs rule function for simplex method
float pqrs(float, float, float, float);

//changing row and column values in the simplex tableau
void rc_change(float **, int, int, int, int);

//generate next tableau
void gen_tableau(float **, int, int, int, int);

//find the pivot element if it exists
int find_pivot(float **, int, int);

float pqrs(float p, float q, float r, float s)
{
  float new_s;
  new_s = s - ((q*r) / p);
  return new_s;
}

//m and n denote the size of the array which is to be changed
void rc_change(float **S, int m, int n, int pivot_row, int pivot_col)
{
  int i, j;
  //printf("%d, %d", pivot_row, pivot_col);
  float pivot = S[pivot_row][pivot_col];
  //changing the pivot
  if(pivot != 0)
    S[pivot_row][pivot_col] = 1/pivot;
  //changing the row elements
  for(j=0; j<n; j++)
    {
      if(j==pivot_col)
	continue;
      if(pivot != 0)
	S[pivot_row][j] = S[pivot_row][j]/pivot;
    }
  //changing the column elements
  for(i=0; i<m; i++)
    {
      if(i==pivot_row)
	continue;
      if(pivot != 0)
	S[i][pivot_col] = -(S[i][pivot_col]/pivot);
    }
}

void copy_mat(float **S, float **copy_S, int m, int n)
{
  int i, j;
  for (i=0; i<m; i++)
    {
      copy_S[i] = (float *)malloc(n*sizeof(float));
      for(j=0; j<n; j++)
	copy_S[i][j] = S[i][j];
    }
}

void gen_tableau(float **S, int m, int n, int pivot_row, int pivot_col)
{
  int i, j;
  float **copy_S;
  //generate a copy of the given matrix
  copy_S = (float **)malloc(m*sizeof(float *));
  copy_mat(S, copy_S, m, n);
  //effect the row and column changes
  rc_change(S, m, n, pivot_row, pivot_col);
  //effect the PQRS change
  for(i=0; i<m; i++)
    {
      if(i==pivot_row)
	continue;
      for(j=0; j<n; j++)
	{
	  if(j==pivot_col)
	    continue;
	  S[i][j] = pqrs(copy_S[pivot_row][pivot_col], copy_S[pivot_row][j], copy_S[i][pivot_col], copy_S[i][j]);
	}
    }
}

int find_pivot(float **S, int m, int n)
{
  int i, j, pivot_col, pivot_row;
  float min=0, ratio;
  for(j=0;j<n-1;j++)
    {
      if(S[m-1][j]<min)
	{
	  min = S[m-1][j];
	  pivot_col = j;
	}
    }
  printf("min=%f\n", min);
  //in case the pivot is not found
  if(min==0)
    return 1;
  //if the pivot is found
  else
    {	
      min=99;
      for(i=0; i<m-1; i++)
	{
	  if(S[i][pivot_col] > 0)
	    {
	      ratio = S[i][n-1]/S[i][pivot_col];
	      if(ratio<min)
		{
		  min = ratio;
		  pivot_row = i;
		}
	    }
	}
      printf("row=%d\n", pivot_row);
      gen_tableau(S, m, n, pivot_row, pivot_col);
      return 0;
    }	
}

void display1(float *A, int m)
{
  int i; 
  for(i=0; i<m; i++)
    printf("%0.2f ",A[i]); 
  printf("\n\n");
}

void display2(float **A, int m, int n)
{
  int i, j;
  for(i=0; i<m; i++)
    {
      for(j=0; j<n; j++)
	printf("%9.2f	", A[i][j]);
      printf("\n");	
    }
  printf("\n");
}

int main()
{
  //m = num_of_constraints
  //n = num_of_variables
  
  int i, j, k, l=0, m, n, count_surp=0, count_art=0, c=0, dummy=0, res=0;
  float **A, **S, *obj_fun, *type, sum=0;
  // char *c1, *c2, c;
  
  printf("enter the number of constraints: ");
  scanf("%d", &m);
  printf("enter the number of variables: ");
  scanf("%d", &n);
  
  printf("\n");
  
  //forming the constraint matrix
  A = (float **)malloc(m * sizeof(float *));
  for(i=0; i<m; i++)
    {
      printf("constraint #%d: ",i+1);
      A[i] = (float *)malloc((n+1) * sizeof(float));
      for(j=0; j<(n+1) ; j++)
		scanf("%f",&A[i][j]);
    }
  printf("\n");
  
  //forming the objective function array
  obj_fun  = (float *)malloc(n * sizeof(float));
  printf("enter the coefficients of the objective function: ");
  for(i=0; i<n; i++)
    scanf("%f", &obj_fun[i]);
  printf("\n");
  
  //forming the type array
  type = (float *)malloc(m * sizeof(float));
  printf("1 -> less than equal to\n2 -> greater than equal to\n3 -> equal to\n");
  for(i=0; i<m; i++)
    {
      printf("type for constraint #%d: ",i+1);
      scanf("%f", &type[i]);
      if(type[i]==2)
	count_surp++;
    }
  printf("\n");
  
  //forming the first simplex tableau
   S = (float **)calloc((m+1), sizeof(float *));
  for(i=0; i<m+1; i++)
    {
      k=0;
      l=0;
      S[i] = (float *)calloc((n+count_surp+1), sizeof(float));
      for(j=0; j<(n+count_surp+1) ; j++)
	{
	  sum=0;
	  if(i<m && j<n)
	    S[i][j] = A[i][j];
	  if(i<m && k==0 && j>=n && j<count_surp+n && type[i]==2)
	    {
	      k=1;
	      S[i][j+c] = -1;
	      c++;
	    }
	  if(i<m && j==n+count_surp)
	    S[i][j] = A[i][n];
	  if(i==m)
	    {
	      for(k=0; k<m; k++)
		sum += S[k][l];
	      if(j<n)
		S[i][j] = -(sum*M) - obj_fun[j];
	      if(j>=n && j<=count_surp+n)
		S[i][j] = -(sum*M);
	    }
	  l++;
	}
    }
  
  //forming title matrices
  /* c1 = (char **)malloc((m+1) * sizeof(char*));
  for(i=0; i<m+1; i++)
    {
      c1[i] = (char *)malloc(sizeof(char));
      for(j=0; j<2; j++)
	{
	  c = (char)(i+1);
	  c1[i][
	}
    }
  
  */  
  printf("Coefficient matrix:\n");
  display2(A, m, n+1);
  
  printf("type array:\n");
  display1(type, m);
  
  printf("objective function\n");
  display1(obj_fun, n);
  
  printf("Simplex matirx\n");
  display2(S, m+1, n+count_surp+1);
  
  while(res==0)
    {
      res = find_pivot(S, m+1, n+count_surp+1);
      display2(S, m+1,n+count_surp+1);
    }
  
  return 0;
}
