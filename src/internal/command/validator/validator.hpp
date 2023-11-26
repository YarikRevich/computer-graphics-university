#pragma once

#include <args/args.hxx>
#include <iostream>

/**
 * Provides access to validation points of argument parser.
*/
class Validator {
private:
    args::ArgumentParser* argumentParser = NULL;

public:
    Validator(args::ArgumentParser*);

    /**
     * Handles argument parser validation.
     * @param exception - given argument parser exception to be handled.
     * @return result code of the operation.
    */
    int handleArgumentParser(std::exception_ptr exception);

    /**
     * Throws value flag required exception validation.
     * @param name - name of the value to be validated.
    */
    static void throwValueFlagRequiredException(std::string name);

    /**
     * Throws value flag invalid exception validation.
     * @param name - name of the value to be validated.
    */
    static void throwValueFlagInvalidException(std::string name);
};