#pragma once

#include <SDL2/SDL.h>

/**
 * Provides access to display operated media.
*/
class Window {
private:
    SDL_DisplayMode native;

    SDL_Window* window = NULL;
public:
    /**
     * Initializes window with a specified options.
     * @return result operation status code.
    */
    int init();

    /**
     * Handles display of the given media
     * @param surface - surface canvas to be displayed.
     * @return result operation status code.
    */
    int handle(SDL_Surface* surface);

    /**
     * Closes opened window and started operations.
     * @return result operation status code.
    */
    int close();
};