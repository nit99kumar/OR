#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

class Matrix{
public:
    int m, n;
    vector< vector<float> > matrix;
    Matrix multiply(Matrix &a);
    void display();
    void input();
    void mninput();
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
    if (this->n != mp.m){
        cout << "Can\'t multiply" << endl;
        exit(1);
    }
    for(int i = 0; i < a.size(); i++){
        vector<float> row;
        for (int j = 0; j < mp.n; j++){
            float sum = 0;
            for(int k = 0; k < mp.m; k++){
                sum += a[i][k]* b[k][j];
            }
            row.push_back(sum);
        }
        newm.push_back(row);
    }
    t->m = this->m;
    t->n = mp.n;
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

void Matrix::mninput(){
    cout << "Enter dimensions:";
    int m, n;
    cin >> m >> n;
    vector <vector<float> > mat;
    for (int i = 0; i < m; i++){
        vector<float> row;
        for(int j = 0; j < n; j++){
            float t;
            cin >> t;
            row.push_back(t);
        }
        mat.push_back(row);
    }
    this->m = m;
    this->n = n;
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

Matrix inverse(Matrix a){
    Matrix binv = identity(a.m);
    for(int i = 0; i < a.m; i++){
        Matrix c = identity(a.m);
        Matrix col = a.getColumn(i);
        Matrix temp = binv.multiply(col);
        for(int j = 0; j < a.m; j++){
            if(j != i)
                c.matrix[j][i] = -temp.matrix[j][0]/temp.matrix[i][0];
            else
                c.matrix[j][i] = +1/temp.matrix[i][0];
        }
        binv = c.multiply(binv);
        //c.display();
        cout << endl;
    }
    return binv;
}

int main(){
    Matrix basis;
    Matrix binv;
    Matrix A;
    Matrix coeff;
    Matrix cb;
    Matrix cn;

    cout << "Enter main matrix:" << endl;
    A.mninput();
    basis = identity(A.m);
    for (int i = 0; i < A.m; i++){
        A.matrix[i].insert(A.matrix[i].end(), basis.matrix[i].begin(),
                           basis.matrix[i].end());
    }


    cout << "Enter coeff matrix" << endl;
    coeff.mninput();
    Matrix orig_coeff(coeff);

    //Form Cb
    vector< vector<float> > cbmat;
    vector<float> cbrow;
    for(int i = 0; i < basis.m; i++){
        cbrow.push_back(0);
    }
    cbmat.push_back(cbrow);
    cb.matrix = cbmat;
    cb.m = 1;
    cb.n = basis.m;

    //Form Cn
    cout << "Enter objective coefficients" << endl;
    vector< vector<float> > cnmat;
    vector<float> cnrow;
    for(int i = 0; i < A.n; i++){
        int t;
        cin >> t;
        cnrow.push_back(t);
    }
    cnmat.push_back(cnrow);
    cn.matrix = cnmat;
    cn.m = 1;
    cn.n = basis.m;

    A.n += basis.n;

    while(true){
        binv = inverse(basis);
        cout << "B inv" << endl;
        binv.display();
        bool pos = true;
        float min = 0; int minindex = 0;
        for(int i = 0; i < basis.m; i++){
            Matrix z;
            Matrix tempcol = A.getColumn(i);
            Matrix temp = cb.multiply(binv);
            z = temp.multiply(tempcol);
            float zi = z.matrix[0][0];
            float zj = zi - cn.matrix[0][i];
            if (zj < 0)
                pos = false;
            if (zj < min){
                min = zj;
                minindex = i;
            }
        }

        coeff = binv.multiply(coeff);
        if (pos == true){
            float sol = (cb.multiply(binv)).multiply(orig_coeff).matrix[0][0];
            cout << "Solution: " << sol << endl;
            Matrix Xb = binv.multiply(orig_coeff);
            Xb.display();
            exit(0);
        }

        Matrix tempcol = A.getColumn(minindex);
        Matrix bpj = binv.multiply(tempcol);
        float ratio = 0; float min2 = 9999; float minindex2;
        for (int i = 0; i < A.m; i++)
        {
            if (bpj.matrix[i][0] > 0){
		Matrix temp = A.getColumn(minindex);
                ratio = coeff.matrix[i][0]/binv.multiply(temp).matrix[i][0];
                cout << "Ratio " << ratio << endl;
                if (ratio < min2){
                    min2 = ratio;
                    minindex2 = i;
                }
            }
        }
        minindex2 += A.m;
        cout << "Entering column: P" << minindex+1 << " " << endl;
        cout << "Leaving column: P" << minindex2+1 << " "<< endl;
        if (ratio == 0){
            cout << "Unbounded solution" << endl;
            exit(0);
        }
        for(int i = 0; i < A.m; i++){
            swap(A.matrix[i][minindex2], A.matrix[i][minindex]);
            basis.matrix[i][minindex2 - A.m] = A.matrix[i][minindex2];
        }
        cout << "B" << endl;
        basis.display();
        swap(cn.matrix[0][minindex], cb.matrix[0][minindex2 - A.m]);
    }
    return 0;
}
