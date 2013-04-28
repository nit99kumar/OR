#include <iostream>
#include <algorithm>
#include <limits>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void display(float **, int, int);
void row_interchange(float **, int, int, int);
int check(float **, int, int);
int result(float **, float *, int, int, int);
void solve_linear_eq(float **, float *, int *, float *, float *, float *, int, int, int);
float get_value(float *, float *, int *, int);

void display(float **a, int m, int n){
	int i, j;
	for(i=0; i<m; i++){
		for(j=0; j<n+1; j++)
			printf("%f ", a[i][j]);
		printf("\n");
	}
}

void row_interchange(float **a, int m, int n, int i){
	int x=i, y;
	float temp;
	while(x<m-1 && a[x][i] == 0)
		x++;
	for(y=i;y<n+1;y++){
		temp = a[i][y];
		a[i][y] = a[x][y];
		a[x][y] = temp;
	}
}

// 1 -> consistent ;0 -> infinite; -1 -> inconsistent
int check(float **a, int m, int n){
	int i, j, count_a = 0, count_ab = 0;
	for(i=0; i<m; i++){
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

int result(float **ab, float *res, int m, int n, int z){
	if(z==-1){
		printf("inconsistent system of linear equations\n\n");
		return 0;}
	if(z==0){
		printf("infinite number of solutions\n\n");
		return 0;}
	if(z==1){
		printf("consistent system of linear eqautions\n\n");
		int i, j, count=0, k, l;
		float copy;
		for(i=0; i<m; i++){
			if(ab[i][n-1] != 0)
				count++;}
		for(i=count-1; i>=0; i--){
			k = count - 1;
			copy = ab[i][n];
			for(j=count-1; j>=i; j--){
				if(j == i)
					res[i] = copy / ab[i][j];
				else{	
					copy -= res[k] * ab[i][j];
					k--;
		}}}
		printf("result:\n");
		for(i=0; i<count; i++)
			printf("%f\n", res[i]);
		for(i=0; i<count; i++){
			if(res[i]<0)
				l=1;}
		if(l==1){
			printf("basic solution\n");
			return 0;}
		else{
			printf("basic feasible solution\n");
			return 1;
		}
	}
}

void solve_linear_eq(float **a, float *b, int *per_arr, float *obj_fn, float *res, float *value, int choice, int m, int n){
	int i, j, k, l, z, p;
	float **ab, copy_ii, copy_ji, temp_value;
	if(choice==0)
		temp_value = 9999;//numeric_limits<float>::max();
	if(choice==1)
		temp_value = -9999;//-numeric_limits<float>::max();
	ab = (float **)malloc(sizeof(float *) * m);
	for(i=0; i<m; i++){
		l=0;
		ab[i] = (float *)malloc(sizeof(float) * (m+1));
		for(j=0; j<n+1; j++){
			if(j!=n && per_arr[j]!=0){
				ab[i][l] = a[i][j];
				l++;}
			if(j==n)
				ab[i][l] = b[i];}}
	for(i=0; i<m-1; i++){
		if(ab[i][i] == 0)
			row_interchange(ab, m, m, i);
		copy_ii = ab[i][i];
		for(j=i+1; j<m; j++){
			copy_ji = ab[j][i];
			for(k=i; k<m+1; k++)
				if(copy_ii != 0)
					ab[j][k] -= ((ab[i][k] / copy_ii) * copy_ji);
				else
					continue;}}
	z = check(ab, m, m);
	printf("\ntype of solution: %d\n",z);
	p = result(ab, res, m, m, z);
	printf("result = %d\n", p);
	if(p==1)
		temp_value = get_value(obj_fn, res, per_arr, n);
	if(choice == 1 && temp_value > *value)
		*value = temp_value;
	if(choice == 0 && temp_value < *value)
		*value = temp_value;
	printf("\n\n");
}

float get_value(float *obj_fn, float *res, int *per_arr, int n){
	int i, j=0;
	float temp_value = 0;
	for(i=0; i<n; i++){
		if(per_arr[i] == 0)
			continue;
		else{
			temp_value += obj_fn[i] * res[j];
			j++;}}
	return temp_value;
}

int main(){
	int *per_arr, m, n, i, j, num_bv, num_nbv, choice;
	float **a, *b, *obj_fn, *res, value;
	printf("Ax = b\n\n");
	printf("enter the number of rows for matrix A: ");
	scanf("%d",&m);
	printf("enter the number of colomns for matrix A: ");
	scanf("%d",&n);
	num_nbv = n-m;
	num_bv = m;
	res = (float *)calloc(m, sizeof(float));
	printf("1: maximise, 0: minimise\nchoice: ");
	scanf("%d",&choice);
	if(choice==0)
		value = 9999;//numeric_limits<float>::max();
	if(choice==1)
		value = -9999;//-numeric_limits<float>::max();
	printf("enter the coefficients of the objective function\n");
	obj_fn = (float *)malloc(sizeof(float) * n);
	for(i=0; i<n; i++){
		printf("coefficient(%d): ",i+1);
		scanf("%f", &obj_fn[i]);}
	per_arr = (int *)malloc(n * sizeof(int));
	for(i=0; i<n; i++){
		if(num_nbv > 0){
			per_arr[i] = 0;
			num_nbv--;}
		else
			per_arr[i] = 1;}
	a = (float **)malloc(sizeof(float *) * m);
	for(i=0; i<m; i++){
		a[i] = (float *)malloc(sizeof(float) * n);
		for(j=0; j<n; j++){
			printf("A[%d,%d]: ",i+1,j+1);
			scanf("%f", &a[i][j]);}}
	b = (float *)malloc(sizeof(float) * m);
	for(i=0; i<m;i++){
		printf("B[%d]: ",i);
		scanf("%f",&b[i]);}
	i=0;
	do{
		printf("iteration %d\n", ++i);
		solve_linear_eq(a, b, per_arr, obj_fn, res, &value, choice, m, n);}
	while(next_permutation(per_arr,per_arr+n));
	if(value == 9999 || value == -9999)
		printf("no feasible solution exists\n");
	else
		printf("the optimal solution: %f\n", value);
	return 0;
}
