#include "state.hpp"

std::vector<int> State::paletteIndeces;

std::vector<Uint8> State::imageCompounds;

std::vector<int> State::getPaletteIndeces() {
    return paletteIndeces;
};

std::vector<Uint8> State::getImageCompounds() {
    return imageCompounds;
};

void State::setPaletteIndeces(std::vector<int> value) {
    State::paletteIndeces = value;
};

void State::setImageCompounds(std::vector<Uint8> value) {
    State::imageCompounds = value;
};