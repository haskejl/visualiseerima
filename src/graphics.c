#include "../include/graphics.h"

//assign constants values
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;

int graphicsHandler(void *arg) {
	struct GraphicsDat *gd = (struct GraphicsDat*)arg;
	
	// Initialize Graphics
	printf("Initializing video...\n");
	int ret_val = SDL_Init(SDL_INIT_VIDEO);
	if(ret_val < 0) {
		printf("SDL failed to initialize! %s\n", SDL_GetError());
		return ret_val;
	}
	
	window = SDL_CreateWindow("Math-Viz", 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
			SDL_RenderPresent(renderer);
		}
		else SDL_Delay(100);
	}

	// Shutdown Graphics
	SDL_Quit();
	return 0;
}

void drawPoint(const int x, const int y, const int size, const int r, const int g, const int b, const int a) {
	SDL_Rect pt;
	pt.x = x;
	pt.y = y;
	pt.w = size;
	pt.h = size;

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawRect(renderer, &pt); 
	SDL_RenderFillRect(renderer, &pt);
}

void drawSolidRect(const SDL_Rect* rect, const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawRect(renderer, rect);
	SDL_RenderFillRect(renderer, rect);
}

// Graph an object, assumes x_min and y_min are negative
void drawGraph(const struct Graph g, const float* x_vals, const float* y_vals, const int n_vals) {
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_Rect r;
	r.x = g.x_pos;
	r.y = g.y_pos;
	r.w = g.width;
	r.h = g.height;

	drawSolidRect(&r, 50, 50, 50, 50);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	// X-axis
	SDL_RenderDrawLine(renderer, g.x_pos, g.y_offset, g.x_pos+g.width, g.y_offset);
	// Y-axis
	SDL_RenderDrawLine(renderer, g.x_offset, g.y_pos, g.x_offset, g.y_pos+g.height);
	for(int i=0; i<n_vals; i++) {
		int x = (int)(g.x_offset+x_vals[i]/g.x_scale);
		int y = (int)(g.y_offset-y_vals[i]/g.y_scale);
		drawPoint(x, y, 1, 255, 0, 255, 255);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

// Graph an object, assumes x_min and y_min are negative
void drawLineGraph(const struct Graph g, const float* x_vals, const float* y_vals, const int n_vals) {
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_Rect r;
	r.x = g.x_pos;
	r.y = g.y_pos;
	r.w = g.width;
	r.h = g.height;
	drawSolidRect(&r, 50, 50, 50, 50);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	// X-axis
	SDL_RenderDrawLine(renderer, g.x_pos, g.y_offset, g.x_pos+g.width, g.y_offset);
	// Y-axis
	SDL_RenderDrawLine(renderer, g.x_offset, g.y_pos, g.x_offset, g.y_pos+g.height);
	int x_last = (int)(g.x_offset+x_vals[0]/g.x_scale);
	int y_last = (int)(g.y_offset-y_vals[0]/g.y_scale);
	for(int i=1; i<n_vals; i++) {
		int x = (int)(g.x_offset+x_vals[i]/g.x_scale);
		int y = (int)(g.y_offset-y_vals[i]/g.y_scale);
		SDL_SetRenderDrawColor(renderer, 200, 0, 200, y%255);
		SDL_RenderDrawLine(renderer, x, y, x_last, y_last);
		x_last = x;
		y_last = y;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void recalcGraphParams(struct Graph* g) {
	g->x_offset = g->x_pos+(int)(g->x_min/(g->x_min-g->x_max)*g->width);
	g->y_offset = g->y_pos+g->height-(int)(g->y_min/(g->y_min-g->y_max)*g->height);
	g->x_scale = (float)(g->x_max-g->x_min)/g->width;
	g->y_scale = (float)(g->y_max-g->y_min)/g->height;
}