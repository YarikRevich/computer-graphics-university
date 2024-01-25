#pragma once

#include <vector>

/**
 * Represents internal processing state. Contains data from the
 * processed operations.
*/
class State {
private:
    /**
     * Represents generate palette indeces.
    */
    static std::vector<int> paletteIndeces;

public:
    /**
     * Retrieves palette indices.
     * 
     * @return retrieved palette indices.
    */
    static std::vector<int> getPaletteIndeces();

    /**
     * Sets palette indices.
     * 
     * @param value - palette indices to be saved.
    */
    static void getPaletteIndeces(std::vector<int> value);
};