#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <SDL2/SDL.h>

//Structs
struct Graph {
    float x_min, x_max, y_min, y_max, x_offset, y_offset, x_scale, y_scale;
    int x_pos, y_pos, width, height;
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

void drawPoint(const int x, const int y, const int size, const int r, const int g, const int b, const int a);
void drawSolidRect(const SDL_Rect* rect, const int r, const int g, const int b, const int a);
void drawLinearRegression(struct GraphicsDat *pGD);

// Graph an object, assumes x_min and y_min are negative
void drawGraph(const struct Graph g, const float* x_vals, const float* y_vals, const int n_vals);
void drawLineGraph(const struct Graph g, const float* x_vals, const float* y_vals, const int n_vals);

void recalcGraphParams(struct Graph* g);

#endif