#include <math.h>
#include <SDL2/SDL.h>

/**
 * Сontains toolset to perform codec operations.
*/
class Processor {
public:
    /**
     * Converts given surface canvas to CGU media type.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertSurfaceToCGU(SDL_Surface* surface);

    /**
     * Converts given 24 bit RGB color to 7 bit RGB single.
     * @param color - given 24 bit RGB color to be converted.
     * @return converted 7 bit RGB single.
    */
    static Uint8 convert24BitRGBTo7BitRGB(SDL_Color color);

    /**
     * Converts given 7 bit single RGB to 24 bit RGB color.
     * @param color - given 7 bit single color to be converted.
     * @return converted 24 bit RGB color.
    */
    static SDL_Color convert7BitRGBTo24BitRGB(Uint8 color);

    /**
     * Converts given 24 bit RGB color to 7 bit grey single. 
     * @param color - given 24 bit RGB color to be converted.
     * @return converted 7 bit grey single.
    */
    static Uint8 convert24BitRGBTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit RGB color.
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit RGB color.
    */
    static SDL_Color convert7BitGreyTo24BitRGB(Uint8 grey);

    /**
     * Normilizes given value according to the given min and max limiters.
     * @param value - value to be normilized.
     * @param min - min value in the range.
     * @param max - max value in the range.
     * @return normilized value.
    */
    static int normalizeValue(int value, int min, int max);
};