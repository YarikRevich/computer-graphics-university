#pragma once

#include <args/args.hxx>

#include <string>

#include "../validator/validator.hpp"
#include "../../state/state.hpp"
#include "../../converter/converter.hpp"
#include "../../io/io.hpp"

/**
 * 
*/
class Encode {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;
    args::ValueFlag<std::string>* type = NULL;
    args::ValueFlag<std::string>* conversion = NULL;
    args::Flag* dithering = NULL;
    args::ValueFlag<std::string>* to = NULL;

public:
    Encode(args::ArgumentParser*);

    /**
     * Checks if the current command is called.
     * 
     * @return result of the check if the current command is called.
    */
    bool isCalled();

    int handle();
};