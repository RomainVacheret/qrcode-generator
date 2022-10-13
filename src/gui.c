#include <SDL2/SDL.h>

#include "gui.h"

#define SIZE 20

static void set_render_color_white(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

static void set_render_color_black(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

static void display(QRCode* qrcode, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.w = SIZE;
    rect.h = SIZE;

    for(size_t i = 0; i < qrcode->size; i++) {
        for(size_t j = 0; j < qrcode->size; j++) {

            (qrcode->matrix[i * qrcode->size + j] ? 
                set_render_color_black : set_render_color_white)(renderer);

            rect.x = (i + 4) * SIZE;
            rect.y = (j + 4) * SIZE;

            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void display_qrcode(QRCode* qrcode) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "QR Code",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600,
        600,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    bool close = false;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    display(qrcode,window, renderer);

    while(!close) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
