#include <iostream>

#include <args/args.hxx>
#include <sdl2/SDL.h>

using namespace std;

SDL_Window* window = NULL;

int main() {
    std::cout << "Hello" << std::endl;

    args::ArgumentParser parser("This is a git-like program", "Valid commands are init and add");

    return -1;
}