#pragma once

#include <args/args.hxx>
#include "../../window/window.hpp"

class View {
private:
    args::Command* command = NULL;

public:
    View(args::ArgumentParser*);

    bool isCalled();

    int handle();
};