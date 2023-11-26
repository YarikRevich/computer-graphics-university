#include "io.hpp"

std::string IO::getType(IO::TYPES src){
    switch(src)
    {
        case IO::TYPES::JPG:       return "jpg";
        case IO::TYPES::PNG:       return "png";
        case IO::TYPES::CGU:       return "cgu";
    }

    return "none";
}

SDL_Surface* IO::readFileJPEG(std::string path) {
    if (IMG_Init(IMG_INIT_JPG) != EXIT_SUCCESS){
        return NULL;
    }

    return IMG_Load(path.c_str());
}

SDL_Surface* IO::readFilePNG(std::string path) {
    if (IMG_Init(IMG_INIT_PNG) != EXIT_SUCCESS){
        return NULL;
    }

    return IMG_Load(path.c_str());
};

SDL_Surface* IO::readFileCGU(std::string path) {
    return NULL;
};

int IO::writeFileJPEG(std::string path, SDL_Surface* surface){
    return IMG_SaveJPG(surface, path.c_str(), 66);  
};

int IO::writeFilePNG(std::string path, SDL_Surface* surface){
    return IMG_SavePNG(surface, path.c_str());  
};

int IO::writeFileCGU(std::string path, SDL_Surface* surface){
    return -1;
};