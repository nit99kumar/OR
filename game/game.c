#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 0.0001

void check_stability(float **A, int m, int n){
	int i, j;
	float *row_max = (float *)malloc(m * sizeof(float));
	float *col_min = (float *)malloc(n * sizeof(float));
	float r_min=99, c_max=-99;

	for(i=0; i<m; i++)
		row_max[i] = -99;
	for(j=0; j<n; j++)
		col_min[j] = 99;
	
	//finding the max in a row
	for(i=0; i<m; i++)
		for(j=0; j<n; j++)
			if(A[i][j] > row_max[i])
				row_max[i] = A[i][j];

	//finding the min in a col
	for(j=0; j<n; j++)
		for(i=0; i<m; i++)
			if(A[i][j] < col_min[j])
				col_min[j] = A[i][j];

	//finding the min in row_max
	for(i=0; i<m; i++)
		if(row_max[i] < r_min)
			r_min = row_max[i];

	//finding the max in col_min
	for(j=0; j<n; j++)
		if(col_min[j] > c_max)
			c_max = col_min[j];

	if(fabs(c_max - r_min) < epsilon){
		printf("the matrix is stable.\nsaddle point(s)\n");
		for(i=0; i<m; i++)
			for(j=0; j<n; j++)
				if(fabs(row_max[i]-col_min[j]) < epsilon)
					printf("(%d, %d)\n", i+1, j+1);
	}
	else
		printf("the matrix is not stable\n");
}

int main(){
	int m, n, i, j;
	float **A = (float **)calloc(m, sizeof(float *));
	//printf("enter the number of rows & columns: ");
	scanf("%d %d", &m, &n);
	for(i=0; i<m; i++){
		A[i] = (float *)calloc(n, sizeof(float));
		//printf("enter row(%d): ", i+1);
		for(j=0; j<n; j++)
			scanf("%f", &A[i][j]);
	}
	check_stability(A, m, n);
	return 0;
}
