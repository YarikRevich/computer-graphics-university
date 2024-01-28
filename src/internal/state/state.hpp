#pragma once

#include <vector>
#include <fstream>
#include <SDL2/SDL.h>

#include "../io/io.hpp"

/**
 * Represents internal processing state. Contains data from the
 * processed operations.
*/
class State {
private:
    /**
     * Represents generated palette indeces.
    */
    static std::vector<Uint8> paletteIndeces;

public:
    /**
     * Retrieves palette indices.
     * @return retrieved palette indices.
    */
    static std::vector<Uint8> getPaletteIndeces();

    /**
     * Sets palette indices.
     * @param value - palette indices to be saved.
    */
    static void setPaletteIndeces(std::vector<Uint8> value);
};