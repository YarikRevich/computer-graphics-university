#include "window.hpp"
#include <iostream>

int Window::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
    }

    if (SDL_GetCurrentDisplayMode(0, &this->native) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    this->window = SDL_CreateWindow(
        VIEW_WINDOW_TITLE, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        native.w/2, 
        native.h/2, 
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Window::handle(SDL_Surface* surface) {
    bool finish = false;

    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (finish) {
            break;
        }

        switch (event.type) {
            case SDL_QUIT:
                finish = true;
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    finish = true;
                    break;
                }
            }   
        }
    }

    return EXIT_SUCCESS;
}

int Window::close() {
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}