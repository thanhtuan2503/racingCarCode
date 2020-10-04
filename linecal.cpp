#include "lineCal.h"

float uCal(float u, int n) {
	float res = u;
	for (int i = 1; i < n; ++i) {
		res *= (u - i);
		res /= (i + 1);
	}
	return res;
}

float absolute(float n) {
	return n > 0 ? n : -n;
}

int isInside(int* arr, int k, int n) {	
	for (int i = 0; i < k; ++i) {
		if (arr[i] == n) return 1;
	}
	return 0;
}

int lineType(double val[8], float y) {
	int sensorWhite[8];
	int sensorBlack[8];
	int numWhite = 0;
	int whiteMax = 100;
	int numBlack = 0;
	int delta = 30;
	for (int i = 0; i < 8; ++i) {
		if (isInside(sensorWhite, numWhite, i) || isInside(sensorBlack, numBlack, i)) continue;
		else {
			for (int j = i; j < 8; ++j) {
				if ((absolute)(val[i] - val[j]) < delta) {
					if (val[i] < whiteMax) {
						sensorWhite[numWhite] = j;
						numWhite++;
					}
					else {
						sensorBlack[numBlack] = j;
						numBlack++;
					}
				}
			}
		}
	}
	if (numWhite >= 7) return 1;
	if (numBlack >= 7) return 2;
	if (numWhite >= 5 && numWhite <= 6) {
		if (sensorWhite[numWhite - 1] >= 7) return 4;
		else return 3;
	}
	if (numWhite <= 4) return 0;
	return 5;
}

//float cal(float val[8][8]) {
//	float res = 0;
//	float max = 0, min = 1027, y, u;
//	float xmax = 0, xmin = 0;
//	float step = 0.1f;
//	for (float x = 0; x <= 7.05; x += step) {
//		y = val[0][0];
//		u = x;
//		for (int i = 1; i < 8; ++i) {
//			y += uCal(u, i) * val[0][i];
//		}		
//		if (y > max) {
//			max = y; xmax = x;
//		}
//		if (y < min) {
//			min = y; xmin = x;
//		}
//	}
//	res = xmin;
//	return res;
//}
