#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <unistd.h>

#define WINDOW_HEIGHT 250
#define WINDOW_WIDTH 250

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Surface *screen;

void *start_display(void *arg);

#endif