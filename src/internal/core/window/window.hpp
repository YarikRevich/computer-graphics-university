#include <sdl2/SDL.h>

/**
 * Provides access to display operated media.
*/
class Window {
private:
    SDL_DisplayMode native;
public:
    /**
     * Retrieves all important options for further initialization.
    */
    Window();

    /**
     * Initializes window with a specified options.
     * @return error code
    */
    int init();
};