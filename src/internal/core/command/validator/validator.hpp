#include <args/args.hxx>

/**
 * Provides access to validation points of argument parser.
*/
class Validator {
private:
    args::ArgumentParser* argumentParser = NULL;
public:
    Validator(args::ArgumentParser*);

    void handle(std::exception_ptr);
};