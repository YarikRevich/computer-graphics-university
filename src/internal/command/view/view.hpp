#pragma once

#include <args/args.hxx>

#include "../../logger/logger.hpp"
#include "../validator/validator.hpp"
#include "../../window/window.hpp"
#include "../../io/io.hpp"
#include "../../converter/converter.hpp"

/**
 * Represents command used to open viewport for the given CGU file.
*/
class View {
private:
    args::Command* command = NULL;
    args::ValueFlag<std::string>* from = NULL;
    args::Flag* debug = NULL;

public:
    View(args::ArgumentParser*);

    /**
     * Checks if the current command is called.
     * @return result of the check if the current command is called.
    */
    bool isCalled();

    /**
     * Handles command activation operation.
     * @return result of view command.
    */
    int handle();
};