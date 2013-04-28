#include <stdio.h>
#include <stdlib.h>

void matrix_multiplication(float **A, float **B, float **C, int m1, int m2, int m3);
void extract_col_vector(float **A, float **Pj, int col, int size);
void replace_col_vector(float **A, float **Pj, int col, int size);
void copy_mat(float **B, float **copy_B, int r1, int r2, int c1, int c2);
void iden(float **I, int size);
void display(float **A, int m, int n);
void inverse(float **M, int size);
void user_matrix(float **M, int n, int m);
void iterate(float **M, int m, int n);

void matrix_multiplication(float **A, float **B, float **C, int m1, int m2, int m3)
{
  int i, j, k;
  for(i=0; i<m1; i++)
    {
      C[i] = (float *)calloc(m1, sizeof(float));
      for(j=0; j<m3; j++)
	{
	  for(k=0; k<m2; k++)
	    C[i][j] += A[i][k]*B[k][j];
	}
    }
}

void extract_col_vector(float **M,float **P, int col, int size)
{
  int i;
  for(i=0; i<size; i++)
    {
      P[i] = (float *)malloc(2 * sizeof(float));
      P[i][0] = M[i][col];
      
    }
}

void replace_col_vector(float **A, float **Pj, int col, int size)
{
  int i, j;
   for(i=0; i<size; i++)
     A[i][col] = Pj[i][0];
}

void iden(float **I, int size)
{
  int i, j;
  for(i=0; i<size; i++)
  {
    I[i] = (float *)calloc(size, sizeof(float));
      for(j=0; j<size; j++)
	{
	  if(i==j)
	    I[i][j] = 1;
	}
    }
}

void display(float **A, int m, int n)
{
  int i, j;
  for(i=0; i<m; i++)
    {
      for(j=0; j<n; j++)
	printf("%7.2f ",A[i][j]);
      printf("\n");
    }
  printf("\n");
}

void copy_mat(float **B, float **copy_B, int r1, int r2, int c1, int c2)
{
  int i, j, k=0, l=0;
  for(i=r1; i<=r2; i++)
    {
      l=0;
      copy_B[k] = (float *)malloc((c2-c1+1) * sizeof(float));
      for(j=c1; j<=c2; j++)
	{
	  copy_B[k][l] = B[i][j];
	  l++;
	}
      k++;
    }
}

void inverse(float **M, int size)
{
  float **P, **M_inv, **res, pivot, **B;
  int c_size = 0, i, j;

  res = (float **)malloc(size * sizeof(float *));
  M_inv = (float **)malloc(size * sizeof(float *));
  P = (float **)malloc(sizeof(float *));
  iden(M_inv, size);

  while (c_size < size)
    {
      B = (float **)malloc(size * sizeof(float *));
      iden(B, size);
      extract_col_vector(M, P, c_size, size);

      matrix_multiplication(M_inv, P, res, size, size, 1);
     
      pivot = res[c_size][0];
      for(i=0; i<size; i++)
	{
	  if(i==c_size)
	    res[i][0] = 1 / pivot;
	  else
	    res[i][0] = -(res[i][0]/pivot);
	}
      replace_col_vector(B, res, c_size, size);
      matrix_multiplication(B, M_inv, res, size, size, size);
      copy_mat(res, M_inv, 0, size-1, 0, size-1);
      c_size++;
    }
  copy_mat(M_inv, M, 0, size-1, 0, size-1);
}

void user_matrix(float **M, int m, int n)
{
  int i, j, row=m+1, col=n+m+1;
  for(i=0; i<row; i++)
    {
      M[i] = (float *)calloc((col), sizeof(float));
      for(j=0; j<col; j++)
	{
	  if(i<m && j<n)
	      scanf("%f",&M[i][j]);
	  if(i<m && j>=n && j<n+m)
	    {
	      if(i+n == j)
		M[i][j] = 1;
	    }
	  if(i<m && j==n+m)
	      scanf("%f", &M[i][j]);
	  if(i==m && j<n)
	    {
	      scanf("%f",&M[i][j]);
	      M[i][j] = -M[i][j];
	    }
	}
    }
}

void iterate(float **M, int m, int n)
{
  int e_var, l_var, count=1, *track, i, j, t;
  float **B, **b, **Y, **P, **Cb, **YP, yp, **A, **Xb, **Z, min_zc, min_A, temp=0;

  //basis matrix
  B = (float **)malloc(m * sizeof(float *));
  //constraint rhs matrix
  b = (float **)malloc(m * sizeof(float *));
  copy_mat(M, b, 0, m-1, n+m, n+m);
  //supporting matrices
  track = (int*)malloc((n+m) * sizeof(int));
  Y = (float **)malloc(sizeof(float *));
  Cb = (float **)malloc(sizeof(float *));
  P = (float **)malloc(m * sizeof(float *));
  YP = (float **)malloc(sizeof(float *));
  A = (float **)malloc(m * sizeof(float *));
  Xb = (float **)malloc(m * sizeof(float *));
  Z = (float **)malloc(sizeof(float *));

  //forming the tracking matrix
  for(i=0; i<n+m; i++)
    track[i] = i;

  do
    {
      min_zc=0;
      min_A=999;
      printf("\n\niteration number: %d\n",count++);
      //computing inverse of the basis matrix
      copy_mat(M, B, 0, m-1, n, n+m-1);
      inverse(B, m);
      printf("inverse of the basis matrix\n");
      display(B, m, m);
      //forming the Cb matrix
      copy_mat(M, Cb, m, m, n, n+m-1);
      for(j=0; j<m; j++)
	Cb[0][j] = -Cb[0][j];

      //computing the Xb matrix
      matrix_multiplication(B, b, Xb, m, m, 1);
      //computing the Z matrix
      matrix_multiplication(Cb, Xb, Z, 1, m, 1);
      //replacing Xb and Z into M
      for(i=0; i<m+1; i++)
	{
	  if(i<m)
	    M[i][n+m] = Xb[i][0];
	  if(i==m)
	    M[i][n+m] = Z[0][0];
	}
      printf("extended revised simplex table\n");
      display(M, m+1, n+m+1);
      //computing the Y matrix
      matrix_multiplication(Cb, B, Y, 1, m, m);
 
      //determining the entering variable
      for(j=0; j<m; j++)
      {
        copy_mat(M, P, 0, m-1, j, j);
	matrix_multiplication(Y, P, YP, 1, m, 1);
	YP[0][0] = YP[0][0] + M[m][j];
	if(YP[0][0] < min_zc)
	  {
	    min_zc = YP[0][0];
	    e_var = j;
	  }
      }
      if(min_zc < 0)
	{
	  //determining the leaving variable
	  copy_mat(M, P, 0, m-1, e_var, e_var);
	  matrix_multiplication(B, P, A, m, m, 1);
	  for(j=0; j<m; j++)
	    {
	      if(A[j][0] > 0)
		{
		  temp = Xb[j][0] / A[j][0];
		  if(temp > 0 && temp < min_A)
		    {
		      min_A = temp;
		      l_var = j+n;
		    }
		}
	    }
	  if(min_A == 0)
	    {
	      printf("the problem is unbounded\n");
	      exit(0);
	    }
	  printf("entering variable = x%d\nleaving variable = x%d\n", track[e_var]+1, track[l_var]+1);
	  for(i=0; i<m+1; i++)
	    {
	      temp = M[i][e_var];
	      M[i][e_var] = M[i][l_var];
	      M[i][l_var] = temp;
	    }
	  //updating the hash table
	  t = track[e_var];
	  track[e_var] = track[l_var];
	  track[l_var] = t;
	}
      
    }while(min_zc < 0);
  for(i=m; i<n+m; i++)
    {
      if(track[i] < m)
	printf("x%d = %7.2f\n", track[i]+1, Xb[i-m][0]);
    }
  printf("all the other variables in the objective function have zero value.\noptimal value of the objective function = %7.2f\n\n",Z[0][0]);
}

int main()
{
  //m = num_of_constraints
  //n = num_of_variables
  
  int m, n;
  float **M;
  M = (float **)calloc((m+1), sizeof(float *));

  scanf("%d%d", &m,&n);
  printf("\n");

  //forming the user matrix
  user_matrix(M, m, n);

  iterate(M, m, n);

  return 0;
}
