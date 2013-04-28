#include <iostream>
#include <vector>
using namespace std;

class Matrix{
public:
  int m, n;
  vector< vector<float> > matrix;
  Matrix multiply(Matrix &a);
  void Matrix::display();
  void input();
  Matrix getColumn(int i);
};

Matrix Matrix::getColumn(int j){
	Matrix n;
	vector< vector<float> > t;
	for (int i =0; i < this->m;i++)
	{
		vector<float> row(1);
		row[0] = this->matrix[i][j];
		t.push_back(row);
	}
	n.matrix = t;
	n.m = this->m;
	n.n = 1;
	return n;
}

Matrix Matrix::multiply(Matrix &mp){
  Matrix *t = new Matrix();
  vector< vector<float> > & a = this->matrix;
  vector< vector<float> > & b = mp.matrix;
  vector< vector<float> > newm;
  for(int i = 0; i < a.size(); i++){
    vector<float> row;
    for (int j = 0; j < this->n; j++){
      float sum = 0;
      for(int k = 0; k < this->n; k++){
		sum += a[i][k]* b[k][j];
      }
      row.push_back(sum);
    }
    newm.push_back(row);
  }  
  t->m = newm.size();
  t->n = newm[0].size();
  t->matrix = newm;
  return *t;
}

void Matrix::input(){
  cout << "Enter matrix: " << endl;
  cout << "Enter dimension: ";
  int dim;
  cin >> dim;
  vector < vector<float> > mat;
  cout << "Enter elements:" << endl;
  for (int i = 0; i < dim; i++){
    vector<float> row;
    for(int j = 0; j < dim; j++){
      float t;
      cin >> t;
      row.push_back(t);
    }
    mat.push_back(row);
  }
  this->m = this->n = dim;
  this->matrix = mat;
}

void Matrix::display(){
	for(int i = 0 ; i < this->m; i++){
		for(int j = 0; j < this->n; j++)
			cout << this->matrix[i][j] << " ";
		cout << endl;
	}	
}

Matrix identity(int dim){
	Matrix m;
	vector< vector<float> > t;
	for(int i = 0; i < dim; i++){
		vector<float> x(dim);
		for(int j = 0; j < dim; j++)
			x[j] = 0;
		t.push_back(x);
	}
	for(int i = 0; i < dim; i++){
		t[i][i] = 1;
	}
	m.matrix = t;
	m.m = m.n = dim;
	return m;
}
	

int main(){
  Matrix a;
  a.input();
  Matrix b = identity(a.m);
  for(int i = 0; i < a.m; i++){
	  Matrix c = identity(a.m);
	  Matrix col = a.getColumn(i);
	  Matrix temp = b.multiply(col);
	  for(int j = 0; j < a.m; j++){
		  if(j != i)
	  		c.matrix[j][i] = -temp.matrix[j][0]/temp.matrix[i][0];
	  	  else
	  	  	c.matrix[j][i] = +1/temp.matrix[i][0];
	  }
	  b = c.multiply(b);
	  c.display();
	  cout << endl;
  }
  b.display();
  
  return 0;
}



