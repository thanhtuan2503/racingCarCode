#include"func.h"
using namespace std;
double abs(double val){
    return (val > 0 ? val : -val);
}
void transpose(double result[N][N], int& res_c, int& res_r, double m[N][N], int m_c, int m_r){
    res_c = m_r;
    res_r = m_c;
    for(int i = 0; i < res_r; i++){
        for(int j = 0; j < res_c; j++){
            result[i][j] = m[j][i];
        }
    }
}

int power(int base, int exp){
    int res = 1;
    for(int i = 0; i < exp; i++) res *= base;
    return res;
}
double getCofactor(double matrix[N][N], int size, int row, int col){
    // cout << "row = " << row << "\ncol = " << col << endl;
    double tmp[N][N] = {0};
    int x = 0, y = 0;
    for(int i = 0; i < size; i++){
        y = 0;
        if (i != row){
            for(int j = 0; j < size; j++){
                if (j != col){
                    // cout << x << ' ' << y << endl;
                    // cout << i << ' ' << j << endl;
                    tmp[x][y] = matrix[i][j];
                    y++;
                }
            }
            x++;
        }
    }
    return power(-1, row + col) * det(tmp, size - 1);
}
double det(double matrix[N][N], int size){
    int determinant = 0;
    if (size == 1) return matrix[0][0];
    for(int i = 0; i < size; i++){
        determinant += matrix[0][i] * getCofactor(matrix, size, 0, i);
    }
    return determinant; 
}
bool invert(double matrix[N][N], double inv[N][N], int size){
    double determinant = det(matrix, size);
    // cout << determinant << endl;
    if (determinant == 0) return 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            inv[i][j] = getCofactor(matrix, size, j, i) / determinant;
        }
    }
    return 1;
}

bool multiply(double result[N][N], int& res_c, int& res_r, double m1[N][N], int m1_c, int m1_r, double m2[N][N], int m2_c, int m2_r){
    if (m1_c != m2_r) return 0;
    res_r = m1_r;
    res_c = m2_c;
    for(int i = 0; i < res_r; i++){
        for(int j = 0; j < res_c; j++){
            double tmp = 0;
            for(int k = 0; k < m1_c; k++){
                tmp += m1[i][k] * m2[k][j];
            }
            result[i][j] = tmp;
        }
    }
    return 1;
}

double weightedAverage(double* input, int l, int r){
    double numerator = 0;
    double denominator = 0;
    for(int i = l; i <= r; i++){
        numerator += i * input[i];
        denominator += input[i];
    }
    double ans = numerator / denominator;
    return ans;
}
double quadraticInterpolation(double* input, int l, int r){
    double secondOrder = 0;
    double firstOrder = 0;
    if (r - l == 2){
        firstOrder = (input[l] + input[l+2] + input[l+1]);
        secondOrder = input[l+1] - input[l] - 2 * firstOrder * l - firstOrder;
    }
    else{
        int size = r - l + 1;
        double H[N][N] = {0};
        for(int i = 0; i < size; i++){
            H[i][0] = power(i + l, 2);
            H[i][1] = i + l;
            H[i][2] = 1;
        }
        double tmpInput[N][N] = {0};
        for(int i = 0; i < size; i++){
            tmpInput[i][0] = input[i + l];
        }
        double transp[N][N] = {0};
        int tr_c, tr_r;
        transpose(transp, tr_c, tr_r, H, 3, size);

        double tmpRes[N][N];
        int tmp_r, tmp_c;
        multiply(tmpRes, tmp_c, tmp_r, transp, tr_c, tr_r, H, 3, size);
        
        double inv[N][N] = {0};
        int invSize = tmp_c;
        invert(tmpRes, inv, invSize);

        multiply(tmpRes, tmp_c, tmp_r, inv, invSize, invSize, transp, tr_c, tr_r);

        double result[N][N] = {0};
        int res_c, res_r;
        multiply(result, res_c, res_r, tmpRes, tmp_c, tmp_r, tmpInput, 1, size);

        firstOrder = result[0][0];
        secondOrder = result[1][0];
    }
    return -secondOrder / (2 * firstOrder);
}
void normalize(double input[N], int maxArr[N], int minArr[N], int maxVal, int minVal){
    for(int i = 0; i < N; i++){
        if (input[i] < minArr[i]) input[i] = minArr[i];
        if (input[i] > maxArr[i]) input[i] = maxArr[i];
        input[i] = minVal + (double)(maxVal - minVal) / (maxArr[i] - minArr[i]) * (input[i] - minArr[i]);
    }
}
double getLinePos(double input[N]){
    for(int i = 0; i < N; i++){
        input[i] = MAXVALUE - input[i];
    }
    const int THRESHOLD = 70;
    int maxIndex = 0;
    for(int i = 0; i < N; i++){
        if (input[maxIndex] < input[i]){
            maxIndex = i;
        }
    }
    if (input[maxIndex - 1] > input[maxIndex + 1]) return weightedAverage(input, maxIndex - 2, maxIndex + 1);
    else return weightedAverage(input, maxIndex - 1, maxIndex + 2);
//    int left = maxIndex, right = maxIndex;
//    while(left > 0 && abs(input[maxIndex] - input[left]) < THRESHOLD) left--;
//    while(right < N - 1 && abs(input[maxIndex] - input[right]) < THRESHOLD) right++; 
//    return weightedAverage(input, left, right);
}
