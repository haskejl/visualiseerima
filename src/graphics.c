#include "../include/graphics.h"
#include <SDL2/SDL_render.h>

//assign constants values
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;

int graphicsHandler(void *arg) {
	struct GraphicsDat *gd = (struct GraphicsDat*)arg;
	
	// Initialize Graphics
	//printf("Initializing video...\n");
	int ret_val = SDL_Init(SDL_INIT_VIDEO);
	if(ret_val < 0) {
		printf("SDL failed to initialize! %s\n", SDL_GetError());
		return ret_val;
	}
	
	window = SDL_CreateWindow("Visualiseerima", 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL) {
		printf("SDL failed to create window! %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
        printf("Renderer failed to create: %s\n", SDL_GetError());
        return -1;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	surface = SDL_GetWindowSurface(window);
	if(surface == NULL) {
        printf("Surface failed to create: %s\n", SDL_GetError());
        return -1;
	}
	SDL_RenderPresent(renderer);
	SDL_Event event;
	gd->quit = false;

	// Main Graphics Loop
	while(!gd->quit) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					gd->quit = true;
					break;
			}
		}
		if(!gd->pause) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			drawGraph(gd->g, gd->pX, gd->pY, gd->nVals);
			if(gd->hasLRModel) drawLinearRegression(gd);
			SDL_RenderPresent(renderer);
		}
		else SDL_Delay(100);
	}

	// Shutdown Graphics
	SDL_Quit();
	return 0;
}

void drawLinearRegression(struct GraphicsDat *pGD) {
	//Find out where the line intersects the graph min and max
	// y = alpha + beta * x
	float xMin = pGD->g.xMin;
	float yMin = pGD->alpha + pGD->beta * xMin;
	// If point is off the graph, switch the formula
	if(yMin < pGD->g.yMin) {
		yMin = pGD->g.yMin;
		xMin = (yMin-pGD->alpha)/pGD->beta;	
	}

	float xMax = pGD->g.xMax;
	float yMax = pGD->alpha + pGD->beta * xMax;
	if(yMax > pGD->g.yMax) {
		yMax = pGD->g.yMax;
		xMax = (yMax-pGD->alpha)/pGD->beta;
	}
	//Just draw from 0 to the max on the x axis
	SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
	SDL_RenderDrawLine(renderer, 
						(int)(pGD->g.xOffset+xMin/pGD->g.xScale), 
						(int)(pGD->g.yOffset-yMin/pGD->g.yScale),
						(int)(pGD->g.xOffset+xMax/pGD->g.xScale), 
						(int)(pGD->g.yOffset-yMax/pGD->g.yScale));
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

// Graph an object, assumes xMin and yMin are negative
void drawGraph(const struct Graph g, const float* xVals, const float* yVals, const int nVals) {
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_Rect r;
	r.x = g.xPos;
	r.y = g.yPos;
	r.w = g.width;
	r.h = g.height;

	SDL_RenderDrawRect(renderer, &r);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	// X-axis
	SDL_RenderDrawLine(renderer, g.xPos, g.yOffset, g.xPos+g.width, g.yOffset);
	// Y-axis
	SDL_RenderDrawLine(renderer, g.xOffset, g.yPos, g.xOffset, g.yPos+g.height);

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	r.w = 1;
	r.h = 1;
	for(int i=0; i<nVals; i++) {
		r.x = (int)(g.xOffset+xVals[i]/g.xScale);
		r.y = (int)(g.yOffset-yVals[i]/g.yScale);

		SDL_RenderDrawRect(renderer, &r); 
		SDL_RenderFillRect(renderer, &r);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

// Graph an object, assumes xMin and yMin are negative
void drawLineGraph(const struct Graph g, const float* xVals, const float* yVals, const int nVals) {
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_Rect r;
	r.x = g.xPos;
	r.y = g.yPos;
	r.w = g.width;
	r.h = g.height;
	SDL_RenderDrawRect(renderer, &r);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	// X-axis
	SDL_RenderDrawLine(renderer, g.xPos, g.yOffset, g.xPos+g.width, g.yOffset);
	// Y-axis
	SDL_RenderDrawLine(renderer, g.xOffset, g.yPos, g.xOffset, g.yPos+g.height);
	int xLast = (int)(g.xOffset+xVals[0]/g.xScale);
	int yLast = (int)(g.yOffset-yVals[0]/g.yScale);
	for(int i=1; i<nVals; i++) {
		int x = (int)(g.xOffset+xVals[i]/g.xScale);
		int y = (int)(g.yOffset-yVals[i]/g.yScale);
		SDL_SetRenderDrawColor(renderer, 200, 0, 200, y%255);
		SDL_RenderDrawLine(renderer, x, y, xLast, yLast);
		xLast = x;
		yLast = y;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void recalcGraphParams(struct Graph* g) {
	g->xOffset = g->xPos+(g->xMin/(g->xMin-g->xMax)*g->width);
	g->yOffset = g->yPos+g->height-(g->yMin/(g->yMin-g->yMax)*g->height);
	g->xScale = (g->xMax-g->xMin)/g->width;
	g->yScale = (g->yMax-g->yMin)/g->height;
}