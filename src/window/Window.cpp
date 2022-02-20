#include <SDL.h>
#include <iostream>
#include <cmath>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

inline float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

inline Uint32 floatToPixel(float value) {
    return (Uint8)(clip(value, 0, 1)  * 255.0);
}

void putPixel(Uint32 pixels[], int i, float r, float g, float b, float a) {
    Uint32 red = floatToPixel(r);
    Uint32 green = floatToPixel(g);
    Uint32 blue = floatToPixel(b);
    Uint32 alpha = floatToPixel(a);
    pixels[i] = (alpha << 24) | (blue << 16) | (green << 8) | red;
}

void helloWorldWindow() {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Texture* screenTexture = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    }
    window = SDL_CreateWindow(
                    "CPlayground",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
                    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    screenTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, 
        SCREEN_WIDTH,
        SCREEN_HEIGHT
        );
    Uint32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        pixels[i] = rand();
    }
    SDL_Event e;
    bool quit = false;
    int itter = 0;
    while (!quit){
        itter++;
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
        }
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int square = 64;
            float grad = (float)((x + itter) % square) / (float) square;

            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                float yGrad = (float)((y + itter) % 48) / 48.0;
                // float randomFloat = ((float)rand()) / ((float)RAND_MAX);

                int i = (y * SCREEN_WIDTH) + x;
                putPixel(pixels, i, 0.0, grad, yGrad, 1.0);
            }
        }

        SDL_UpdateTexture(screenTexture, NULL, pixels, SCREEN_WIDTH * 4);
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL); 
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

