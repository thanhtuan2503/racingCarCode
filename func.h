#define N 8
#define MAXVALUE 1023
#define MINVALUE 0
#ifndef DEFINEPIN_H_
#define DEFINEPIN_H_
#define A_IN1   A5
#define A_IN2   A4
#define A_IN3   10
#define A_IN4   9
#define A_IN5   8
#define A_IN6   6 
#define A_IN7   12
#define A_IN8   4

#define SERVO_PPM   13
#define MOTOR_PPM   5
#define PWN_LINE    11
#define TX   1
#define RX   0
#define ECD_A   3
#define ECD_B   2
#endif
void transpose(double[N][N], int&, int&, double[N][N], int ,int);
double weightedAverage(double* input, int l, int r);
double det(double matrix[N][N], int size);
double quadraticInterpolation(double* input, int l, int r);
int power(int base, int exp);
double getCofactor(double matrix[N][N], int size, int row, int col);
bool invert(double matrix[N][N], double inv[N][N], int size);
void printMatrix(double matrix[N][N], int row, int col);
bool multiply(double result[N][N], int& res_c, int& res_r, double m1[N][N], int m1_c, int m1_r, double m2[N][N], int m2_c, int m2_r);
void transpose(double result[N][N], int& res_c, int& res_r, double m[N][N], int m_c, int m_r);
double getLinePos(double input[N]);
void calibrate(int input[N]);
void normalize(double input[N], int maxArr[N], int minArr[N], int max, int min);
