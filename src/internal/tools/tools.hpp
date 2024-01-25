#pragma once

#include <cstdio>

#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#endif

#include <string>
#include <thread>

class Tools {
private:
    static std::string SPIN_CHARS;

    /**
     * Starts indefinite command line spinner.
    */
    static void startIndefiniteSpinnerRaw();
public:
    /**
     * Start thread to run the indefinite command line spinner.
    */
    static void startIndefiniteSpinner();
};