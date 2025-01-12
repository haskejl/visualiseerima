#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>

#include <SDL2/SDL.h>
#include <time.h>

#include "../include/filehandler.h"
#include "../include/graphics.h"
#include "../include/regression.h"

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int nLines = 100;
	int n = nLines;
	int p = 2;

	float **pX = (float**)malloc(n*sizeof(float*));
	float *pY = (float*)malloc(n*sizeof(float));
	float *pBeta = (float*)malloc(n*sizeof(float));

	for(int i=0; i<n; i++) {
		pX[i] = (float*)malloc((p*sizeof(float)));
		pX[i][0] = 1.0;
	}

	float pXSLR[nLines];

	char cmd[10];
	thrd_t t[1];

	struct GraphicsDat gd;

	gd.g.xMax = 100;
	gd.g.xMin = -10;
	gd.g.yMax = 100;
	gd.g.yMin = -10;
	gd.g.xPos = 100;
	gd.g.yPos = 100;
	gd.g.width = 800;
	gd.g.height = 800;
	gd.pause = false;
	gd.pX = pXSLR;
	gd.pY = pY;
	gd.beta = 0.0;
	gd.alpha = 0.0;
	gd.hasLRModel = false;

	recalcGraphParams(&gd.g);

	thrd_create(t, graphicsHandler, &gd);

	while(strcmp(cmd, "quit") != 0) {
		if(strcmp(cmd, "read") == 0) {
			readLines("test.csv", pY, pX[1], nLines);
		}
		else if(strcmp(cmd, "write") == 0) {
			writeLines("test.csv", pY, pX[1], nLines);
		}
		else if(strcmp(cmd, "generate") == 0) {
			float slope = ((float)rand()/(float)RAND_MAX * 4.0)-2.0;
			float yInt = rand() % 10;
			for(int i=0; i<nLines; i++) {
				pXSLR[i] = pX[i][1] = ((float)(rand()%200) +
				 ((float)rand()/(float)RAND_MAX - 0.5)*10.0)-100;
				pY[i] = yInt + slope * pX[i][1] + ((float)rand()/(float)RAND_MAX - 0.5)*10.0;
				if(pY[i] > 100) i--;
			}
			gd.nVals = nLines;
			gd.hasLRModel = false;
		}
		else if(strcmp(cmd, "slr") == 0) {
			olsLinearRegression(pX, pY, n, p, pBeta);
			simpleLinearRegression(gd.pX, gd.pY, nLines, &gd.beta, &gd.alpha);
			gd.hasLRModel = true;
		}
		else if(strcmp(cmd, "q1") == 0) {
			gd.g.xMax = 100;
			gd.g.xMin = -10;
			gd.g.yMax = 100;
			gd.g.yMin = -10;
			recalcGraphParams(&gd.g);
		}
		else if(strcmp(cmd, "q2") == 0) {
			gd.g.xMax = 10;
			gd.g.xMin = -100;
			gd.g.yMax = 100;
			gd.g.yMin = -10;
			recalcGraphParams(&gd.g);
		}
		else if(strcmp(cmd, "q3") == 0) {
			gd.g.xMax = 10;
			gd.g.xMin = -100;
			gd.g.yMax = 10;
			gd.g.yMin = -100;
			recalcGraphParams(&gd.g);
		}
		else if(strcmp(cmd, "q4") == 0) {
			gd.g.xMax = 100;
			gd.g.xMin = -10;
			gd.g.yMax = 10;
			gd.g.yMin = -100;
			recalcGraphParams(&gd.g);
		}
		else if(strcmp(cmd, "q1234") == 0) {
			gd.g.xMax = 100;
			gd.g.xMin = -100;
			gd.g.yMax = 100;
			gd.g.yMin = -100;
			recalcGraphParams(&gd.g);
		}
		else if(strcmp(cmd, "pause") == 0) {
			gd.pause = true;
		}
		else if(strcmp(cmd, "unpause") == 0) {
			gd.pause = false;
		}
		else if(strcmp(cmd, "print") == 0) {
			for(int i=0; i<nLines; i++) {
				printf("%f, %f\n", pY[i], pX[i][1]);
			}
		}
		else if(strcmp(cmd, "clear") == 0) {
			system("clear");
		}
		else if(strcmp(cmd, "help") == 0) {
			printf("read/write:\tread or write the current data to test.csv\n");
			printf("print:\t\tprint currently loaded data to the screen\n");
			printf("generate:\tgenerate new data\n");
			printf("slr:\t\tmake a simple linear regression model on the data\n");
			printf("pause/unpause:\tpauses or unpauses the graph\n");
			printf("clear:\t\tclears the console\n");
			printf("quit:\t\tquits the program\n");
		}
		printf("> ");
		scanf("%9s", cmd);
		int c = getchar();
		while(c != '\n' && c != EOF) {
			c = getchar();
		}
	}
	gd.quit = true;
	int res;
	thrd_join(t[0], &res);
	
	for(int i=0; i<n; i++) {
		free(pX[i]);
	}
	free(pX);
	free(pY);
	free(pBeta);
}
