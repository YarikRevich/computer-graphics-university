#pragma once

#include <args/args.hxx>
#include <string>

#include "../validator/validator.hpp"
#include "../../pipeline/pipeline.hpp"
#include "../../io/io.hpp"

/**
 * Represents command used to decode given CGU file to the request format.
*/
class Decode {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;
    args::ValueFlag<std::string>* type = NULL;
    args::Flag* debug = NULL;
    args::ValueFlag<std::string>* to = NULL;

public:
    Decode(args::ArgumentParser*);

    /**
     * Checks if the current command is called.
     * @return result of the check if the current command is called.
    */
    bool isCalled();

    /**
     * Handles command activation operation.
     * @return result of decode command.
    */
    int handle();
};