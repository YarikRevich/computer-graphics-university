#include "state.hpp"

std::vector<Uint8> State::paletteIndeces;

std::vector<Uint8> State::getPaletteIndeces() {
    return paletteIndeces;
};

void State::setPaletteIndeces(std::vector<Uint8> value) {
    State::paletteIndeces = value;
};