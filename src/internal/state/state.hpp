#pragma once

#include <vector>
#include <SDL2/SDL.h>

/**
 * Represents internal processing state. Contains data from the
 * processed operations.
*/
class State {
private:
    /**
     * Represents generated palette indeces.
    */
    static std::vector<int> paletteIndeces;

    /**
     * Represents generated image 7-bit compounds.
    */
    static std::vector<Uint8> imageCompounds;
public:
    /**
     * Retrieves palette indices.
     * @return retrieved palette indices.
    */
    static std::vector<int> getPaletteIndeces();

    /**
     * Retrieves image compounds indices.
     * @return retrieved image compounds.
    */
    static std::vector<Uint8> getImageCompounds();

    /**
     * Sets palette indices.
     * @param value - palette indices to be saved.
    */
    static void setPaletteIndeces(std::vector<int> value);

    /**
     * Sets palette indices.
     * @param value - image compounds to be saved.
    */
    static void setImageCompounds(std::vector<Uint8> value);
};