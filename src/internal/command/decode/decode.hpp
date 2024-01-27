#pragma once

#include <args/args.hxx>
#include <string>

#include "../validator/validator.hpp"
#include "../../converter/converter.hpp"
#include "../../io/io.hpp"

class Decode {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;
    args::ValueFlag<std::string>* type = NULL;
    args::Flag* optimal = NULL;
    args::ValueFlag<std::string>* to = NULL;

public:
    Decode(args::ArgumentParser*);

    /**
     * Checks if the current command is called.
     * 
     * @return result of the check if the current command is called.
    */
    bool isCalled();

    int handle();
};