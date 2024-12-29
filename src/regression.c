#include "../include/regression.h"

void simpleLinearRegression(float *pX, float *pY, const int n, float *pBeta, float*pAlpha) {
    float avgX = 0.0;
    float avgY = 0.0;
    for(int i=0; i<n; i++) {
        avgX += pX[i];
        avgY += pY[i];
    }
    avgX = avgX / (float)n;
    avgY = avgY / (float)n;

    float num = 0.0;
    float denom = 0.0;
    for(int i=0; i<n; i++) {
        num += (pX[i]-avgX)*(pY[i]-avgY);
        denom += (pX[i]-avgX)*(pX[i]-avgX);
    }
    *pBeta = num / denom;
    *pAlpha = avgY - *pBeta*avgX;
    printf("SLR: y=%f+%fx\n", *pAlpha, *pBeta);
}

void olsLinearRegression(float **pX, float *pY, const int n, int p, float *pBeta) {
    // Check if x is already padded with 1's
    bool padded = false;
    for(int i=0; i<n; i++) {
        padded = padded || (pX[i][0] == 0.0);
    }
    if(!padded) {
        //realloc & add
        //p = p+1; //added another column
    }
    //Compute X^T X
    // Do not free pXTX or pXTX[i] until after the calculation & final use of R*
    // This could be cut in half as the matrix is always symmetric and R* only 
    // needs half of the matrix
    float **pXTX = (float**)malloc(p*sizeof(float*));
    for(int i=0; i<p; i++) {
        pXTX[i] = (float*)malloc(p*sizeof(float));
    }

    for(int tRows=0; tRows < p; tRows++) {
        for(int rCols=0; rCols < p; rCols ++) {
            pXTX[tRows][rCols] = 0.0;
            for(int rRows=0; rRows < n; rRows++) { //also tCols
                pXTX[tRows][rCols] += pX[rRows][tRows] * pX[rRows][rCols];
            }
        }
    }

    // Calculate R* using the Cholesky–Banachiewicz algorithm
    // This can be calculated in place, so no new memory is needed,
    // however, a new variable is defined to prevent confusion
    float **pRStar = pXTX;

    for(int i=0; i< p; i++) {
        for(int j=0; j<=i; j++) {
            float sum = 0.0;
            for(int k=0; k < j; k++) {
                sum += pRStar[j][k] * pRStar[j][k];
            }
            if(i==j) pRStar[i][j] = sqrt(pXTX[i][i]-sum);
            else pRStar[i][j] = (pXTX[i][j] - sum)/pRStar[j][j];
        }
    }

    // Make it symmetric for later, but this can be cleaned up
    for(int i=0; i<p; i++) {
        for(int j=0; j<i; j++) {
            pRStar[j][i] = pRStar[i][j];
        }
    }

    //Calculate X^T y
    float* pXTY = (float*)malloc(p*sizeof(float));
    
    for(int xtRows=0; xtRows < p; xtRows++) {
        pXTY[xtRows] = 0.0;
        // yRows is also xtCols
        for(int yRows = 0; yRows < n; yRows++) {
            pXTY[xtRows] += pX[yRows][xtRows] * pY[yRows];
        }
    }

    float pb[p];
    // Forward Substitution
    for(int i=0; i<p; i++) {
        float sum = 0.0;
        for(int j=0; j<i; j++) {
            sum += pRStar[i][j] * pb[j];
        }
        pb[i] = (pXTY[i] - sum) / pRStar[i][i];
    }

    // Backward Substitution
    for(int i=p-1; i>=0; i--) {
        float sum = 0.0;
        for(int j=i+1; j<p; j++) {
            sum += pRStar[i][j] * pBeta[j];
        }
        pBeta[i] = (pb[i] - sum) / pRStar[i][i];
    }
    printf("OLS LR: y = %f", pBeta[0]);
    for(int i=1; i<p; i++) {
        printf("+%fx_%d", pBeta[i], i);
    }
    printf("\n");


    // Cleanup allocated memory
    for(int i=0; i<p; i++) {
        free(pXTX[i]);
    }
    free(pXTX);
    free(pXTY);
}