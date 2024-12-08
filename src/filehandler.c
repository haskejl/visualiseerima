#include "../include/filehandler.h"

int readLines(char *pFileName, float *pY, float *pX, int nLines) {
	FILE *pFile;
	pFile = fopen(pFileName, "r");
	if(pFile == NULL) {
		printf("Error: \"%s\" doesn't exist!\n", pFileName);
		return -1;
	}

	for(int i=0; i<nLines; i++) {
		fscanf(pFile, "%f", &pY[i]);
		fscanf(pFile, "%f", &pX[i]);
	}
	fclose(pFile);
	return 0;
}

int writeLines(char *pFileName, float *pY, float *pX, int nLines) {
	FILE *pFile;
	pFile = fopen(pFileName, "w");
	if(pFile == NULL) {
		printf("Error: something went wrong creating \"%s\"!\n", pFileName);
		return -1;
	}

	for(int i=0; i<nLines; i++) {
		fprintf(pFile, "%f %f\n", pY[i], pX[i]);
	}
	fclose(pFile);
	return 0;
}
