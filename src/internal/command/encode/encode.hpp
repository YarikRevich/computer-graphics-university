#pragma once

#include <args/args.hxx>

#include <string>

#include "../validator/validator.hpp"
#include "../../converter/converter.hpp"
#include "../../io/io.hpp"

class Encode {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;
    args::ValueFlag<std::string>* type = NULL;
    args::ValueFlag<std::string>* conversion = NULL;
    args::ValueFlag<std::string>* to = NULL;

public:
    Encode(args::ArgumentParser*);

    bool isCalled();

    int handle();
};