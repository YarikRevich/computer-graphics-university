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
     * @return error code
    */
    int init();

    /**
     * Handles display of the given media
    */
    int handle();

    /**
     * Closes opened window and started operations;
    */
    int close();
};