#pragma once

#include <args/args.hxx>
#include "../../io/io.hpp"

class Encode {
private:
    args::Command* command = NULL;
    // args::ArgumentParser command("This is a git-like program", "Valid commands are init and add");

public:
    Encode(args::ArgumentParser*);

    bool isCalled();

    int handle();
};