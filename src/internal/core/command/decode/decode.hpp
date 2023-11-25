#include <args/args.hxx>

class Decode {
private:
    args::Command* command = NULL;

public:
    Decode(args::ArgumentParser*);

    bool isCalled();
};