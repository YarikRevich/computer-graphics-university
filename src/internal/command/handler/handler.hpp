#pragma once

#include <args/args.hxx>
#include "../decode/decode.hpp"
#include "../encode/encode.hpp"
#include "../view/view.hpp"
#include "../help/help.hpp"
#include "../validator/validator.hpp"

using namespace std;

/**
 * Provides access to operate CLI command handlers.
*/
class Handler {
private:
    args::ArgumentParser* argumentParser = NULL;

    Encode* encode = NULL;
    Decode* decode = NULL;
    View* view = NULL;
    Help* help = NULL;
    Validator* validator = NULL;

    int argc;
    char **argv;

public:
    Handler(int argc, char **argv);

    /**
     * Starts command handler execution. 
    */
    int run();
};