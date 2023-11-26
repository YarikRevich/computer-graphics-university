#pragma once

#include <args/args.hxx>
#include <string>

#include "../validator/validator.hpp"
#include "../../processor/processor.hpp"
#include "../../io/io.hpp"

class Decode {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;
    args::ValueFlag<std::string>* type = NULL;
    args::ValueFlag<std::string>* to = NULL;

public:
    Decode(args::ArgumentParser*);

    bool isCalled();

    int handle();
};