#include <args/args.hxx>

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
    */
    void handle(std::exception_ptr exception);
};