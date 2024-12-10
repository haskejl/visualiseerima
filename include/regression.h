#ifndef REGRESSION_H
#define REGRESSION_H

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
    printf("LR: y=%f+%fx\n", *pAlpha, *pBeta);
}

#endif