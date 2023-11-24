#include "../internal/core/command/handler/handler.hpp"

using namespace std;

int main(int argc, char **argv) {
    Handler* handler = new Handler(argc, argv);
    handler->run();

    return -1;
}