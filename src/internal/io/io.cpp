#include "io.hpp"
#include <iostream>

IO::TYPES IO::getType(std::string src){
    if (src == "jpg" || src == "jpeg") {
        return IO::TYPES::JPG;
    } else if (src == "png") {
        return IO::TYPES::PNG;
    }

    return IO::TYPES::NONE;
}

SDL_Surface* IO::readFileJPEG(std::string path) {
    if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG){
        return NULL;
    }

    return IMG_Load(path.c_str());
}

SDL_Surface* IO::readFilePNG(std::string path) {
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        return NULL;
    }

    return IMG_Load(path.c_str());
};

SDL_Surface* IO::readFileCGU(std::string path) {
    return IMG_Load(path.c_str());
};

int IO::writeFileJPEG(std::string path, SDL_Surface* surface){
    return IMG_SaveJPG(surface, path.c_str(), 66);  
};

int IO::writeFilePNG(std::string path, SDL_Surface* surface){
    return IMG_SavePNG(surface, path.c_str());  
};

int IO::writeFileCGU(std::string path, SDL_Surface* surface){
    return SDL_SaveBMP(surface, path.c_str());
};