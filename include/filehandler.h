#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <stdio.h>

int readLines(char *pFileName, float *pY, float *pX, int nLines);
int writeLines(char *pFileName, float *pY, float *pX, int nLines);

#endif
