#include "state.hpp"

std::vector<int> State::paletteIndeces;

std::vector<int> State::getPaletteIndeces() {
    return paletteIndeces;
};

void State::getPaletteIndeces(std::vector<int> value) {
    State::paletteIndeces = value;
};