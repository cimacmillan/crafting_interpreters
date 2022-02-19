#include <SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void helloWorldWindow() {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    }
    window = SDL_CreateWindow(
                    "hello_sdl2",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
                    );
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    }
    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

