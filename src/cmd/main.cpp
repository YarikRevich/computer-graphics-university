#include "../internal/core/command/handler/handler.hpp"

using namespace std;

/**
 * Entrypoint, to propogate logic to the handler.
*/
int main(int argc, char **argv) {
    Handler* handler = new Handler(argc, argv);
    return handler->run();
}