#include "../internal/logger/logger.hpp"
#include "../internal/command/handler/handler.hpp"

using namespace std;

/**
 * Entrypoint, to propogate logic to the handler.
*/
int main(int argc, char **argv) {
    Handler* handler = new Handler(argc, argv);

    if (handler->run() != EXIT_SUCCESS) {
        if (const char* msg = SDL_GetError(); strlen(msg) != 0) {
            std::cout << SDL_GetError() << std::endl;   
        } else if (Logger::IsErrorExist()) {
            std::cout << Logger::GetError() << std::endl;
        }
    }
}