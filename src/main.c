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
	float pX[nLines];
	float pY[nLines];
	for(int i=0; i<nLines; i++) {
		pX[i] = 0.0;
		pY[i] = 0.0;
	}

	struct GraphicsDat gd;
	gd.g.x_max = 100;
	gd.g.x_min = -1;
	gd.g.y_max = 100;
	gd.g.y_min = -1;
	gd.g.x_pos = 100;
	gd.g.y_pos = 100;
	gd.g.width = 800;
	gd.g.height = 800;
	recalcGraphParams(&gd.g);
	gd.pause = false;
	gd.pX = pX;
	gd.pY = pY;
	gd.beta = 0.0;
	gd.alpha = 0.0;
	gd.hasLRModel = false;

	thrd_t t[1];
	thrd_create(t, graphicsHandler, &gd);

	char cmd[100];

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
				pX[i] = 5.0 + (float)(rand()%95) + ((float)rand()/(float)RAND_MAX - 0.5)*10.0;
				pY[i] = y_int + slope * pX[i] + ((float)rand()/(float)RAND_MAX - 0.5)*9.5;
			}
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
			printf("clear:\t\tclears the screen\n");
		}
		printf("> ");
		scanf("%s", cmd);
	}
	gd.quit = true;
	int res;
	thrd_join(t[0], &res);
}
