#include <args/args.hxx>
#include "../decode/decode.hpp"
#include "../encode/encode.hpp"
#include "../help/help.hpp"

using namespace std;

/**
 * Provides access to operate CLI command handlers.
*/
class Handler {
private:
    args::ArgumentParser* argumentParser = NULL;

    Encode* encode = NULL;
    Decode* decode = NULL;
    Help* help = NULL;

    int argc;
    char **argv;

public:
    Handler(int argc, char **argv);

    /**
     * Starts command handler execution. 
    */
    void run();
};