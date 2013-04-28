#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//for the hash tables
int row[100];
int col[100];

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

float* dual_impact(float **, int *, int, int);
float* primal_impact(float **, int *, int, int);
int *check_table_type(float **, int, int);

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

//return mixed->0, primal only->1, dual only->2
int *check_table_type(float **S, int m, int n)
{
	int i, j, primal_flag=0, dual_flag=0, min_col, min_row, *key_arr;
	float min=0;
	//finding primal feasibility
	for(j=0; j<n-1; j++)
	{
		if(S[m-1][j] < min)//for every negative entry in the last row check the corresponding column
		{
			for(i=0; i<m-1; i++)
			{
				if(S[i][j] > 0 && S[i][n-1] > 0)//at least one positive entry found in the column andthe corressponding  entry in the last column is also positive and update if this element is less than the previous element
				{
					min = S[m-1][j];
					min_col = j;
					primal_flag=1;//primal key can be found
					break;
				}
			}
		}
	}
	//finding dual feasibility
	min=0;
	for(i=0; i<m-1; i++)
	{
		if(S[i][n-1] < min)//for every negative entry in the last column check the corresponding row
		{
			for(j=0; j<n-1; j++)
			{
				if(S[i][j] < 0)
				{
					dual_flag = 1;
					min = S[i][n-1];
					min_row = i;
				}
			}
		}
	}
	if(primal_flag==1 && dual_flag==1)
	{
		key_arr = (int *)malloc(3* sizeof(int));//(type, min_col, min_row)
		key_arr[0]=0;
		key_arr[1]=min_col;//primal
		key_arr[2]=min_row;//dual
	}
	else if(primal_flag==1 && dual_flag==0)
	{
		key_arr = (int *)malloc(2 * sizeof(int));//(type, min_col)
		key_arr[0]=1; 
		key_arr[1]=min_col; 
	}
	else if(primal_flag==0 && dual_flag==1)
	{
		key_arr = (int *)malloc(2 * sizeof(int));//(type, min_row)
		key_arr[0]=2; 
		key_arr[1]=min_row; 
	}
	else
	{
		key_arr = (int *)malloc(sizeof(int));//(type, min, min_col or min_row)
		key_arr[0]=-1;
	}
	return key_arr;
}

float* primal_impact(float **S, int *key_arr, int m, int n)
{
	int i, p_row, p_col = key_arr[1];
	float *p_arr, ratio, min=999;
	p_arr = (float *)malloc(3 * sizeof(float));
	for(i=0; i<m-1; i++)
	{
		if(S[i][p_col] > 0 && S[i][n-1] > 0)
		{
			ratio = S[i][n-1] / S[i][p_col];
			if(ratio < min)
			{
				min = ratio;
				p_row = i;
			}
		}
	}
	p_arr[0] = fabs((S[m-1][p_col] * S[p_row][n-1]) / S[p_row][p_col]);
	p_arr[1] = p_row;
	p_arr[2] = p_col;
	return p_arr;
}

float* dual_impact(float **S, int *key_arr, int m, int n)
{
	int i, j, d_row = key_arr[2], d_col;
	float *d_arr, ratio, min=999;
	d_arr = (float *)malloc(3 * sizeof(float));
	for(j=0; j<n-1; j++)
	{
		if(S[d_row][j] < 0)
		{
			ratio = fabs(S[m-1][j] / S[d_row][j]);
			if(ratio < min)
			{
				min = ratio;
				d_col = j;
			}
		}
	}
	d_arr[0] = fabs((S[m-1][d_col] * S[d_row][n-1]) / S[d_row][d_col]);
	d_arr[1] = d_row;
	d_arr[2] = d_col;
	return d_arr;
}

int find_pivot(float **S, int m, int n)
{
	int *key_arr, i, j, pivot_col, pivot_row;
	float *p_arr, *d_arr;
	key_arr = check_table_type(S, m, n);
	if(key_arr[0] == 0)
	{
		p_arr = primal_impact(S, key_arr, m, n);
		d_arr = dual_impact(S, key_arr, m, n);
		if(p_arr[0] > d_arr[0])
		{
			pivot_row = p_arr[1];
			pivot_col = p_arr[2];
		}
		if(p_arr[0] <= d_arr[0])
		{
			pivot_row = d_arr[1];
			pivot_col = d_arr[2];
		}
	}
	else if(key_arr[0] == 1)
	{
		p_arr = primal_impact(S, key_arr, m, n);
		pivot_row = p_arr[1];
		pivot_col = p_arr[2];
	}
	else if(key_arr[0] == 2)
	{
		d_arr = dual_impact(S, key_arr, m, n);
		pivot_row = d_arr[1];
		pivot_col = d_arr[2];
	}
	else if(key_arr[0] == -1)
		return 1;
	gen_tableau(S, m, n, pivot_row, pivot_col);
	//exchanging the values
	row[pivot_col]^=col[pivot_row]^=row[pivot_col]^=col[pivot_row];
	return 0;

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
		for(j=0; j<n ; j++)
			printf("%9.2f	", A[i][j]);
		printf("\n");	
	}
	printf("\n");
}

int main()
{
	//m = num_of_constraints
	//n = num_of_variables
	int i, j, k, l=0, m, n, count_art=0, c=0, dummy=0, res=0;
	float **A, **S, *obj_fun, *type, sum=0, values[100];
	//printf("enter the number of constraints: ");
	scanf("%d", &m);
	//printf("enter the number of variables: ");
	scanf("%d", &n);
	
	//forming the constraint matrix
	A = (float **)malloc(m * sizeof(float *));
	for(i=0; i<m; i++)
	{
	  //printf("constraint #%d: ",i+1);
		A[i] = (float *)malloc((n+1) * sizeof(float));
		for(j=0; j<(n+1) ; j++)
			scanf("%f",&A[i][j]);
	}
	
	//forming the objective function array
	obj_fun  = (float *)malloc(n * sizeof(float));
	//printf("enter the coefficients of the objective function: ");
	for(i=0; i<n; i++)
		scanf("%f", &obj_fun[i]);
	printf("\n");
	
	//forming the type array
	type = (float *)malloc(m * sizeof(float));
	//printf("1 -> less than equal to\n2 -> greater than equal to\n3 -> equal to\n");
	for(i=0; i<m; i++)
	{
		//printf("type for constraint #%d: ",i+1);
		scanf("%f", &type[i]);
		if(type[i]==3)
			count_art++;
	}
	
	//forming the first simplex tableau
	S = (float **)calloc((m+1), sizeof(float *));
	for(i=0; i<m+1; i++)
	{
		l=0;
		S[i] = (float *)calloc((n+count_art+1), sizeof(float));
		for(j=0; j<(n+count_art+1); j++)
		{
			sum=0;
			if(i<m && j<n)
				S[i][j] = A[i][j];
			if(i<m && j>=n && j<count_art+n && type[i]==3)
			{
				S[i][j+c] = -1;
				c++;
			}
			if(i<m && j==n+count_art)
			{
				S[i][j] = A[i][n];
				if(type[i]==2)
					S[i][j] = -A[i][j];
			}
			if(i<m && type[i]==2)
				S[i][j] = -S[i][j];
			if(i==m)
			{
				if(j<n)
					S[i][j] = - obj_fun[j];
			}
			l++;
		}
	}
	
	for(i=0; i<n+count_art; i++){
		row[i]=i;
	}
	for(i=0; i<m; i++){
		col[i]=n+count_art+i;
	}
	
	printf("Coefficient matrix:\n");
	display2(A, m, n+1);
	
	printf("type array:\n");
	display1(type, n);
	
	printf("objective function\n");
	display1(obj_fun, n);
	
	printf("Simplex matirx\n");
	display2(S, m+1, n+count_art+1);
	
	do
	{
		res = find_pivot(S, m+1, n+count_art+1);
		display2(S, m+1,n+count_art+1);
	}while(res==0);

	for(i=0; i<n+count_art; i++){
		values[row[i]]=0;	
	}
	for(i=0; i<m; i++){
		values[col[i]]=S[i][n+count_art];
	}
	/*for(i=n+count_art; i<=n+count_art+m; i++){
		if(values[i]>0.001){
			printf("No feasible Solution\n");
			return 0;
		}
	}*/
	for(i=0; i<n; i++){
		printf("x%d= %.2lf\n", i+1, values[i]);
	}
	printf("z= %.2lf\n", S[m][n+count_art]);
	return 0;
}
