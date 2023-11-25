#include <args/args.hxx>
#include "../../io/io.hpp"

class Decode {
private:
    args::Command* command = NULL;

public:
    Decode(args::ArgumentParser*);

    bool isCalled();

    int handle();
};