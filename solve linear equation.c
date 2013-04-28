#include <stdio.h>
#include <stdlib.h>

void display(float **a, int m, int n)
{
	int i, j;
	for(i=0; i<m; i++)
	{
		for(j=0; j<n+1; j++)
		{
			printf("%f ", a[i][j]);
		}
		printf("\n");
	}
}

void row_interchange(float **a, int m, int n, int i)
{
	int x=i, y, temp;
	while(a[x][i] == 0 && x<m)
		x++;
	for(y=i;y<n+1;y++)
	{
		temp = a[i][y];
		a[i][y] = a[x][y];
		a[x][y] = temp;
	}
}

// 1 -> consistent
// 0 -> infinite
// -1 -> inconsistent
int check(float **a, int m, int n)
{
	int i, j, count_a = 0, count_ab = 0;
	for(i=0; i<m; i++)
	{
		if(a[i][n-1] != 0)
			count_a++;
		if(a[i][n] != 0)
			count_ab++;
	}
	if(count_a > count_ab)
		j=0;
	else if(count_a == count_ab)
		j=1;
	else if(count_a < count_ab)
		j=-1;
	
	return j;
}

void result(float **ab, int m, int n)
{
	int i, j, count=0, k;
	float copy;
	for(i=0; i<m; i++)
	{
		if(ab[i][n-1] != 0)
			count++;
	}
	float *res = (float *)calloc(count, sizeof(float));

	for(i=count-1; i>=0; i--)
	{
		k = count - 1;
		copy = ab[i][n];
		for(j=count-1; j>=i; j--)
		{
			if(j == i)
				res[i] = copy / ab[i][j];
			else
			{
				copy -= res[k] * ab[i][j];
				k--;
			}
		}
	}
	
	//display the answer
	for(i=0; i<count; i++)
		printf("%f\n", res[i]);
}

int main()
{
	int m, n, i, j, k, q, w, z;
	float **ab, **a, *b, copy_ii, copy_ji;;
	printf("Ax = b\n\n");
	
	printf("enter the number of rows for matrix A: ");
	scanf("%d",&m);
	printf("enter the number of colomns for matrix A: ");
	scanf("%d",&n);
	
	//forming the matrix A
	a = (float **)malloc(sizeof(float *) * m);
	for(i=0; i<m; i++)
	{
		a[i] = (float *)malloc(sizeof(float) * n);
		for(j=0; j<n; j++)
		{
			printf("A[%d,%d]: ",i+1,j+1);
			scanf("%f", &a[i][j]);
		}
	}
	
	//forming the matrix b
	b = (float *)malloc(sizeof(float) * m);
	for(i=0; i<m;i++)
	{
		printf("B[%d]: ",i);
		scanf("%f",&b[i]);
	}
	
	//forming the augmented matrix
	ab = (float **)malloc(sizeof(float *) * m);
	for(i=0; i<m; i++)
	{
		ab[i] = (float *)malloc(sizeof(float) * (n+1));
		for(j=0; j<n+1; j++)
		{
			if(j != n)
				ab[i][j] = a[i][j];
			else
				ab[i][j] = b[i];
		}
	}
	
	for(i=0; i<m-1; i++)
	{
		if(ab[i][i] == 0)
			row_interchange(ab, m, n, i);
		copy_ii = ab[i][i];
		for(j=i+1; j<m; j++)
		{
			copy_ji = ab[j][i];
			for(k=i; k<n+1; k++)
				ab[j][k] -= ((ab[i][k] / copy_ii) * copy_ji);
		}
	}
	display(ab, m, n);
	z = check(ab, m, n);
	printf("type of solution: %d\n",z);
	result(ab, m, n);
	return 0;
}
