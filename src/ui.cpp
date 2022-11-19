#include <ui.h>
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Surface *screen;

void handle_sdl_event() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev) > 0) {
        if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE) {
            exit(0);
        }
    }
}

void *start_display(void *arg) {
    (void)arg;
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    while (1) {
        usleep(1000);
        handle_sdl_event();
    }
    return NULL;
}
