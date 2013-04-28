#include <stdio.h>
#include <stdlib.h>

void matrix_multiplication(float **A, float **B, float **C, int m1, int m2, int m3);
void extract_col_vector(float **A,float **Pj, int col, int size);
void replace_col_vector(float **A, float **Pj, int col, int size);
void iden(float **I, int size);
void display(float **A, int m, int n);
void iterate(float **M, int size);

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

//extracting a column vector
void extract_col_vector(float **M,float **P, int col, int size)
{
  int i;
  for(i=0; i<size; i++)
    {
      P[i] = (float *)malloc(2 * sizeof(float));
      P[i][0] = M[i][col];
      
    }
}

//replacing a column vector
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
	printf("%f ",A[i][j]);
      printf("\n");
    }
  printf("\n");
}

void copy_mat(float **B, float **copy_B, int size)
{
  int i;
  for(i=0; i<size; i++)
    {
		copy_B[i] = B[i];
    }
}

void iterate(float **M, int size)
{
  float **P, **M_inv, **res, pivot, **B;
  int c_size = 0, i, j;

  res = (float **)malloc(size * sizeof(float *));
  M_inv = (float **)malloc(size * sizeof(float *));
  P = (float **)malloc(sizeof(float *));
  iden(M_inv, size);

  while (c_size < size)
    {
      printf("iteration number: %d\n", c_size+1);
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
      printf("c^\n");
      display(res, size, 1);
      replace_col_vector(B, res, c_size, size);
	  //res is getting changed over here
      matrix_multiplication(B, M_inv, res, size, size, size);
	  copy_mat(res, M_inv, size);
      printf("B\n");
      display(B, size, size);
      printf("M_inv\n");
      display(M_inv, size, size);
      c_size++;
    }
}

int main()
{
  int i, j, size;
  float **M;

  scanf("%d",&size);

  //forming the user matrix
  M = (float **)malloc(size * sizeof(float));
  for(i=0; i<size; i++)
    {
      M[i] = (float *)malloc(size * sizeof(float));
      for(j=0; j<size; j++)
	  scanf("%f",&M[i][j]);
    }

  iterate(M, size);

  return 0;
}
