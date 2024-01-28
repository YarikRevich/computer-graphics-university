#include "tools.hpp"

std::string Tools::SPIN_CHARS = "/-\\|";

void Tools::startIndefiniteSpinnerRaw() {
#if defined(__linux__) || defined(__APPLE__)
    
    int i = 0;

    while (true) {
        putchar(SPIN_CHARS[i % SPIN_CHARS.size()]);
        fflush(stdout);
        usleep(100000);
        putchar('\b');

        i++;
    }

#endif
}

void Tools::startIndefiniteSpinner() {
#if defined(__linux__) || defined(__APPLE__)
    
    std::thread t1(startIndefiniteSpinnerRaw);
    t1.detach();

#endif
}