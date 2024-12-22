#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>

#include <SDL2/SDL.h>

#include "../include/filehandler.h"
#include "../include/graphics.h"
#include "../include/regression.h"

int main(int argc, char* argv[]) {
	int nLines = 100;

	char cmd[10];
	float pX[nLines];
	float pY[nLines];
	thrd_t t[1];

	struct GraphicsDat gd;

	for(int i=0; i<nLines; i++) {
		pX[i] = 0.0;
		pY[i] = 0.0;
	}

	gd.g.x_max = 100;
	gd.g.x_min = -10;
	gd.g.y_max = 100;
	gd.g.y_min = -10;
	gd.g.x_pos = 100;
	gd.g.y_pos = 100;
	gd.g.width = 800;
	gd.g.height = 800;
	gd.pause = false;
	gd.pX = pX;
	gd.pY = pY;
	gd.beta = 0.0;
	gd.alpha = 0.0;
	gd.hasLRModel = false;

	recalcGraphParams(&gd.g);

	thrd_create(t, graphicsHandler, &gd);

	while(strcmp(cmd, "quit") != 0) {
		if(strcmp(cmd, "read") == 0) {
			readLines("test.csv", pY, pX, nLines);
		}
		else if(strcmp(cmd, "write") == 0) {
			writeLines("test.csv", pY, pX, nLines);
		}
		else if(strcmp(cmd, "generate") == 0) {
			float slope = (float)rand()/(float)RAND_MAX * 2.f;
			float y_int = rand() % 10;
			for(int i=0; i<nLines; i++) {
				pX[i] = (float)(rand()%95) +
				 ((float)rand()/(float)RAND_MAX - 0.5)*10.0;
				pY[i] = y_int + slope * pX[i] + ((float)rand()/(float)RAND_MAX - 0.5)*10.0;
				if(pY[i] > 100) i--;
			}
			gd.nVals = nLines;
			gd.hasLRModel = false;
		}
		else if(strcmp(cmd, "slr") == 0) {
			simpleLinearRegression(gd.pX, gd.pY, nLines, &gd.beta, &gd.alpha);
			gd.hasLRModel = true;
		}
		else if(strcmp(cmd, "pause") == 0) {
			gd.pause = true;
		}
		else if(strcmp(cmd, "unpause") == 0) {
			gd.pause = false;
		}
		else if(strcmp(cmd, "print") == 0) {
			for(int i=0; i<nLines; i++) {
				printf("%f, %f\n", pY[i], pX[i]);
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
}
