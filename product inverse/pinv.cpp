#include <iostream>
#include <vector>

using namespace std;

class Mat
{
	private:
		int size, i, j, dummy;
		float temp;
		vector<float> Pj;		
		
	public:
		vector<vector<float> > init;
		vector<vector<float> > identity;
		
		//constructor to define the variable size
		Mat(int x)
		{
			size = x;
		}
		
		//forming the user matrix and the identity matrix
		void input()
		{
			for(i=0; i<size; i++)
			{
			 	vector<float> t1(size);
				vector<float> t2(size);	 
				for(j=0; j<size; j++)
				{
					cout << "(" << i+1 << ", " << j+1 << "): ";
					cin >> t1[j];
					if(i==j)
						t2[j]=1;
					else
						t2[j]=0;
				}
				init.push_back(t1);
				identity.push_back(t2);
			}
			display(identity);
		}
		
		//displaying the matrix
		void display(vector< vector<float> > A)
		{
			int dummy=0;
			for(i=0; i<size; i++)
			{
				for(j=0; j<size; j++)
					cout << A[i][j] << " ";
				cout << endl;
			}
			while(dummy==0)
			{
				cout << "enter 1 to exit: ";
				cin >> dummy;
			}
		}
		
		//multiplying two matrices
		void multiplication(const vector< vector<float> > &A, const vector< vector<float> > &B, int m1, int m2, int m3)
		{
			int i, j, k;
			vector< vector<float> > M;
			
			for(i=0; i<m1; i++)
			{
			 	vector<float> t(m3);
				for(j=0; j<m3; j++)
					t[j] = 0;
				M.push_back(t);
			}
			
			for(i=0; i<m1; i++)
				for(j=0; j<m3; j++)
					for(k=0; k<m2; k++)
						M[i][j] += A[i][k] * B[k][j];
			
			display(M);
		}
		
		//extracting a column vector
		vector extract_col_vector(vector<vector <float> > A, int col, int size)
		{
			int i, j;
			for(i=0; i<size; i++)
				Pj.push_back(A[i][col]);
		}
		
		//replacing a column vector
		void replace_col_vector(vector<vector <float> > &A, vector<float> Pj, int col, int size)
		{
			int i, j;
			for(i=0; i<size; i++)
				A[i][col] = Pj[i];
		}
		
		void iterate()
		{
			vector<vector <float> > A;
			vector<float> B;
			vector<float> Pj;
			int i, j, c_size = size;
			//generating the identity matrix
			for(i=0; i<size; i++)
			{
				vector<float> t(size);
				for(j=0; j<size; j++)
				{
					if(i==j)
						t[j] = 1;
					else
						t[j] = 0;
				}
				A.push_back(t);
			}
			while(c_size > 0)
			{
				Pj = extract_col_vector(A, c_size, size);
				multiplication(A, Pj, );
				extract_col_vector(
				c_size--;
			}
		}
};

int main()
{
	int size, c_size;
	cout << "enter the size of the square matrix: ";
	cin >> size;
	c_size = size;
	
	//creating an object of the Mat class
	Mat *obj = new Mat(size);
	
	//calling the functions of the Mat class
	obj->input();
	cout << endl;
	
	while(c_size > 0)
	{
		
		obj->multiplication(obj->init, obj->init, size, size, size);
	}
	
	return 0;
}
