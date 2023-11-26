#pragma once

#include <args/args.hxx>

#include "../validator/validator.hpp"
#include "../../window/window.hpp"
#include "../../io/io.hpp"

class View {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;

public:
    View(args::ArgumentParser*);

    bool isCalled();

    int handle();
};