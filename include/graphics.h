#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <SDL2/SDL.h>

//Structs
struct Graph {
    float xMin, xMax, yMin, yMax, xOffset, yOffset, xScale, yScale;
    int xPos, yPos, width, height;
};
struct GraphicsDat {
    struct Graph g;
    int nVals;
    float *pX, *pY;
    bool pause, quit;
    //Hacky way to include regression model
    bool hasLRModel;
    float alpha, beta;
};

//Constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

static SDL_Window* window = NULL;
static SDL_Surface* surface = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

//Start external graphics libraries central to the program, return 0 if all are successfully started
int graphicsHandler(void *arg);

void drawLinearRegression(struct GraphicsDat *pGD);

// Graph an object, assumes xMin and yMin are negative
void drawGraph(const struct Graph g, const float* x_vals, const float* y_vals, const int n_vals);
void drawLineGraph(const struct Graph g, const float* x_vals, const float* y_vals, const int n_vals);

void recalcGraphParams(struct Graph* g);

#endif