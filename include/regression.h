#ifndef REGRESSION_H
#define REGRESSION_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void simpleLinearRegression(float *pX, float *pY, const int n, float *pBeta, float*pAlpha);
void olsLinearRegression(float **pX, float *pY, const int n, int p, float *pBeta);

#endif