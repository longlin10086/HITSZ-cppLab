#include <iostream>
#include <cmath>

using namespace std;

class Matrix{
private:
    double _matrix[4][4]={
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
    };

    int change_times = 0;


    // transfer the row of matrix
    void switch_row(int row_origin, int row_target){
        for (int col = 0; col < 4; ++col) {
            double tmp = this->_matrix[row_origin][col];
            this->_matrix[row_origin][col] = this->_matrix[row_target][col];
            this->_matrix[row_target][col] = tmp;
        }
    }

    // let the row divide the first element
    void divide_row(Matrix& result, int row){
        double tmp = this->_matrix[row][row];
        for (int col = 0; col < 4; ++col) {
            this->_matrix[row][col] /= tmp;
            result._matrix[row][col] /= tmp;
        }
    }

    // turn the other row to 0
    void subtract(Matrix& result, int col){
        for (int row = col+1; row < 4; ++row) {
            double times = this->_matrix[row][col]/this->_matrix[col][col];
            for (int col_n = 0; col_n < 4; ++col_n) {
                this->_matrix[row][col_n] -= this->_matrix[col][col_n]*times;
                result._matrix[row][col_n] -= result._matrix[col][col_n]*times;
            }
        }
    }

    void subtract_upper(Matrix& result, int col){
        for (int row = 0; row < col; ++row) {
            double times = this->_matrix[row][col];
            for (int col_n = 0; col_n < 4; ++col_n) {
                this->_matrix[row][col_n] -= this->_matrix[col][col_n]*times;
                result._matrix[row][col_n] -= result._matrix[col][col_n]*times;
            }
        }
    }

    // turn the matrix to upper triangle
    void turn_triangle(Matrix& result, int row){
        for (int row_origin = row; row_origin < 4; ++row_origin) {
            bool is_changed = false;
            if(this->_matrix[row_origin][row]!=0){
                this->switch_row( row, row_origin);
                result.switch_row(row, row_origin);
                is_changed = true;
            }
            if(is_changed) {
                this->change_times++;
                break;
            }
        }
    }


public:

    // init with the zero matrix
    void init(){
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                this->_matrix[row][col] = 0;
            }
        }
    }

    // init the matrix as unit matrix
    Matrix(){
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                if(row==col) this->_matrix[row][col] = 1;
                else this->_matrix[row][col] = 0;
            }
        }
    }

    // init with a double array of 2 dimension
    Matrix(double(&matrix)[4][4]){
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                this->_matrix[row][col] = matrix[row][col];
            }
        }
    }

    // a copy init
    Matrix(Matrix& matrix){
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                this->_matrix[row][col] = matrix._matrix[row][col];
            }
        }
    }

    // reload the [] operator
    const double* operator[](const int i) const {return this->_matrix[i];}
    double* operator[](const int i) {return this->_matrix[i];}

    // reload the = operator
    Matrix& operator=(Matrix&& matrix){
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                this->_matrix[row][col] = matrix._matrix[row][col];
            }
        }
        return *this;
    }

    // reload the = operator with the double array
    Matrix& operator=(double(&matrix)[4][4]){
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                this->_matrix[row][col] = matrix[row][col];
            }
        }
        return *this;
    }

    // reload the + operator
    Matrix operator+(const Matrix& other){
        Matrix result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result._matrix[row][col] = this->_matrix[row][col]+other._matrix[row][col];
            }
        }
        return result;
    }

    // reload the - operator
    Matrix operator-(Matrix& other){
        Matrix result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result._matrix[row][col] = this->_matrix[row][col]-other._matrix[row][col];
            }
        }
        return result;
    }

    // reload the * operator
    Matrix operator*(const Matrix& other){
        Matrix result;
        result.init();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    result._matrix[i][j] += this->_matrix[i][k]*other._matrix[k][j];
                }
            }
        }
        return result;
    }

    // calculate the inverse matrix
    Matrix inverse(){
        Matrix result;
        Matrix tmp = *this;
        for (int row = 0; row < 4; ++row) {
            tmp.turn_triangle(result, row);
            tmp.divide_row(result, row);
            tmp.subtract(result, row);
        }
        for (int row = 3; row > 0; --row) {
            tmp.subtract_upper(result, row);
        }
        return result;
    }


    // realize the power method
    Matrix operator^(const int n){
        Matrix result;
        if(n>0) {
            for (int times = 0; times < n; ++times) {
                result = result * (*this);
            }
            return result;
        }
        else if(n<0){
            for (int times = 0; times > n; --times) {
                Matrix tmp = *this;
                result = result * tmp.inverse();
            }
            return result;
        }
        else return result;
    }

    // realize the matrix transpose
    Matrix transpose(){
        Matrix result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result._matrix[row][col] = this->_matrix[col][row];
            }
        }
        return result;
    }


    // calculate the determinant of matrix
    double determinant(){
        Matrix result;
        Matrix tmp = *this;
        double ans = 1.0;
        for (int row = 0; row < 4; ++row) {
            tmp.turn_triangle(result, row);
        }
        for (int row = 0; row < 4; ++row) {
            tmp.subtract(result, row);
        }
        for (int row = 0; row < 4; ++row) {
            ans *= tmp._matrix[row][row];
        }
        ans *= pow((-1),tmp.change_times);
        return ans;
    }

    friend istream& operator>>(istream& input, Matrix& matrix);
    friend ostream& operator<<(ostream& output,const Matrix& matrix);
};

istream& operator>>(istream& input, Matrix& matrix){
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            input >> matrix._matrix[row][col];
        }
    }
}

ostream& operator<<(ostream& output, const Matrix& matrix){
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            output << matrix._matrix[row][col] << ' ';
        }
        output << '\n';
    }
    output << endl;
}

double arr[4][4] = {
        {1,2,3,5},
        {6,3,2,1},
        {4,3,5,2},
        {6,1,3,8}
};

double arr1[4][4] = {
        {0,2,0,1},
        {3,3,2,1},
        {3,2,4,0},
        {2,2,1,1}
};

double m[4][4] = {
        {1,2,3,4},
        {8,6,7,9},
        {4,10,-4,12},
        {-13,14,45,28}
};

int main() {
    Matrix a;
    Matrix b(a);
    Matrix c(m);
    Matrix d;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            d[i][j] = m[i][j];
        }
    }
    cout << "a: " << '\n' << a << endl;
    cout << "b(a): " << '\n' << b << endl;
    cout << "c(m): " << '\n' << c << endl;
    cout << "d: " << '\n' << d << endl;

    d=a+c;
    cout << "d=a+c: " << '\n' << d << endl;

    d=d-a;
    cout << "d=d-a: " << '\n' << d << endl;

    d=d*d.inverse();
    cout << "d=d*d.inverse(): " << '\n' << d << endl;

    c=c^3;
    cout << "d=c^3: " << '\n' << c << endl;

    d=c.transpose();
    cout << "d=c.transpose(): " << '\n' << d << endl << "c: " << '\n' << c;
}
