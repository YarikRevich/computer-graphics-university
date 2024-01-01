#include "tools.hpp"

std::string Tools::SPIN_CHARS = "/-\\|";

void Tools::startIndefiniteSpinnerRaw() {
    int i = 0;

    while (true) {
        putchar(SPIN_CHARS[i % SPIN_CHARS.size()]);
        fflush(stdout);
        usleep(100000);
        putchar('\b');

        i++;
    }
}

void Tools::startIndefiniteSpinner() {
    std::thread t1(startIndefiniteSpinnerRaw);
    t1.detach();
}